# coding: latin_1

# by dracaryS - 11.6.22

import ui
import renderTarget
import app
import item
import dbg
import player
import nonplayer
import chr
import chat
import playersettingmodule
import localeInfo
import chrmgr
from _weakref import proxy

IMG_DIR = "d:/ymir work/ui/game/render/"
RENDER_WINDOW_SIZE = [500, 500]

ENABLE_WOLFMAN = False


def IsCanShowItems(itemVnum):
    item.SelectItem(itemVnum)
    (itemType, itemSubType) = (item.GetItemType(),item.GetItemSubType())
    if (item.ITEM_TYPE_COSTUME == itemType and (itemSubType == item.COSTUME_TYPE_HAIR or itemSubType == item.COSTUME_TYPE_BODY or itemSubType == item.COSTUME_TYPE_SKIN_SASH or itemSubType == item.COSTUME_TYPE_SASH or itemSubType == item.COSTUME_TYPE_WEAPON or itemSubType == item.COSTUME_TYPE_MOUNT or itemSubType == item.COSTUME_TYPE_PET)):
        return True
    elif item.ITEM_TYPE_ARMOR == itemType and itemSubType == item.ARMOR_BODY:
        return True
    elif item.ITEM_TYPE_WEAPON == itemType and itemSubType != item.WEAPON_ARROW:
        return True
    elif item.ITEM_TYPE_SHINING == itemType:
        return True
    if app.ENABLE_MOUNT_SKIN and item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_MOUNT_SKIN:
        return True
    return False

def get_length(x):
    return len(x[0])
def GetRefineMaxLevel(vnum):
    return 9
def getRealVnum(vnum):
    isRefineItem = False
    item.SelectItem(vnum)
    isRefineItem = False
    level = "0"
    itemname = item.GetItemName()
    pos = itemname.find("+")
    if pos != -1:
        level = itemname[pos+1:]
        if level.isdigit():
            isRefineItem = True
            vnum -= int(level)
    return (vnum,isRefineItem)

class RenderTargetWindow(ui.BoardWithTitleBar):
    children={}
    def OnPressEscapeKey(self):
        return self.Close()
    def __del__(self):
        ui.BoardWithTitleBar.__del__(self)
    def Destroy(self):
        renderWindow = self.GetRenderWindow()
        if renderWindow != None:
            renderWindow.Destroy()
        self.children={}
    def __init__(self):
        ui.BoardWithTitleBar.__init__(self)
        self.Destroy()
        self.__LoadWindow()

    def GetRenderWindow(self):
        if self.children.has_key("renderWindow"):
            return self.children["renderWindow"]
        return None

    def Close(self):
        renderWindow = self.GetRenderWindow()
        if renderWindow != None:
            renderWindow.Close()
        self.Hide()
        return True

    def Open(self, renderType = 0, vnumIndex = 11299):
        renderWindow = self.GetRenderWindow()
        if renderWindow != None:
            if renderType == 0 and IsCanShowItems(vnumIndex) == False:
                return
            renderWindow.PrepareRenderTarget(renderType, vnumIndex)
        self.Show()
        self.SetFocus()
        self.SetTop()

    def __LoadWindow(self):
        global RENDER_WINDOW_SIZE
        self.SetSize(7+RENDER_WINDOW_SIZE[0]+7, RENDER_WINDOW_SIZE[1]+30+5)
        self.SetCenterPosition()
        self.SetCloseEvent(self.Close)
        self.AddFlag("movable")
        self.AddFlag("attach")
        self.AddFlag("float")
        self.SetTitleName("Render Target")

        renderWindow = MultiFunctionalRender(self, 7, 30, RENDER_WINDOW_SIZE[0], RENDER_WINDOW_SIZE[1])
        renderWindow.Show()
        self.children["renderWindow"] = renderWindow

        searchBoard = SearchSlotBoard()
        searchBoard.SetParent(self)
        searchBoard.SetPosition(10, 35)
        searchBoard.SetSize(129,23)
        searchBoard.Show()
        self.children["searchBoard"] = searchBoard

        itemSearch = ui.EditLine()
        itemSearch.SetParent(searchBoard)
        itemSearch.SetMax(50)
        itemSearch.SetPosition(5, 5)
        itemSearch.SetSize(searchBoard.GetWidth(), searchBoard.GetHeight())
        itemSearch.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateItem)
        itemSearch.SetInfoMessage(localeInfo.WIKI_ITEM_NAME)
        itemSearch.isNeedEmpty = False
        itemSearch.OnPressEscapeKey = ui.__mem_func__(self.StartSearchItem)
        itemSearch.SetOutline()
        itemSearch.SetReturnEvent(ui.__mem_func__(self.StartSearchItem))
        itemSearch.Show()
        self.children["itemSearch"] = itemSearch

        searchBtn = ui.Button()
        searchBtn.SetParent(searchBoard)
        searchBtn.SetUpVisual(IMG_DIR+"search_btn_0.tga")
        searchBtn.SetOverVisual(IMG_DIR+"search_btn_1.tga")
        searchBtn.SetDownVisual(IMG_DIR+"search_btn_2.tga")
        searchBtn.SetEvent(self.StartSearchItem)
        searchBtn.SetPosition(itemSearch.GetWidth()-searchBtn.GetWidth(),2)
        searchBtn.Show()
        self.children["searchBtn"] = searchBtn

        searchMobBoard = SearchSlotBoard()
        searchMobBoard.SetParent(self)
        searchMobBoard.SetPosition(10, 35+searchBoard.GetHeight()+5)
        searchMobBoard.SetSize(129,23)
        searchMobBoard.Show()
        self.children["searchMobBoard"] = searchMobBoard

        mobSearch = ui.EditLine()
        mobSearch.SetParent(searchMobBoard)
        mobSearch.SetMax(50)
        mobSearch.SetPosition(5, 5)
        mobSearch.SetSize(searchMobBoard.GetWidth(), searchMobBoard.GetHeight())
        mobSearch.OnIMEUpdate = ui.__mem_func__(self.__OnValueUpdateMob)
        mobSearch.SetInfoMessage(localeInfo.WIKI_MOB_NAME)
        mobSearch.isNeedEmpty = False
        mobSearch.OnPressEscapeKey = ui.__mem_func__(self.StartSearchMob)
        mobSearch.SetOutline()
        mobSearch.SetReturnEvent(ui.__mem_func__(self.StartSearchMob))
        mobSearch.Show()
        self.children["mobSearch"] = mobSearch

        searchMobBtn = ui.Button()
        searchMobBtn.SetParent(searchMobBoard)
        searchMobBtn.SetUpVisual(IMG_DIR+"search_btn_0.tga")
        searchMobBtn.SetOverVisual(IMG_DIR+"search_btn_1.tga")
        searchMobBtn.SetDownVisual(IMG_DIR+"search_btn_2.tga")
        searchMobBtn.SetEvent(self.StartSearchMob)
        searchMobBtn.SetPosition(itemSearch.GetWidth()-searchMobBtn.GetWidth(),2)
        searchMobBtn.Show()
        self.children["searchMobBtn"] = searchMobBtn

    # Search Item #
    def ClearEditlineItem(self):
        self.children["selectedItem"]=0
        self.children["itemSearch"].SetText("")
        self.children["itemSearch"].SetInfoMessage(localeInfo.WIKI_ITEM_NAME)

    
    def __OnValueUpdateItem(self):
        itemSearch = self.children["itemSearch"]
        ui.EditLine.OnIMEUpdate(itemSearch)
        input_text_real = itemSearch.GetText()
        input_len = len(input_text_real)
        board = self.children["searchBoard"]
        totalWidth = 129
        if input_len > 0:
            totalWidth-=25
            windowHeight = self.GetWidth()-15
            textSize = itemSearch.GetTextSize()[0]
            if textSize >= totalWidth:
                if textSize >= windowHeight:
                    totalWidth = windowHeight
                else:
                    totalWidth = textSize
            totalWidth+=25
            if totalWidth >= windowHeight:
                totalWidth = windowHeight
        board.SetSize(totalWidth, 23)
        itemSearch.SetSize(totalWidth, 23)
        searchBtn = self.children["searchBtn"]
        searchBtn.SetPosition(itemSearch.GetWidth()-searchBtn.GetWidth(),2)
        if input_len == 0:
            self.ClearEditlineItem()
            return False
        if localeInfo.IsARABIC():
            input_text = input_text_real
        else:
            input_text = input_text_real.lower()
        searchBtn.Show()
        items_list = item.GetItemsByName(str(input_text))
        itemList = []
        namesList = []
        for i, itemVnum in enumerate(items_list, start=1):
            (realVnum, isRefineItem) = getRealVnum(itemVnum)
            if isRefineItem:
                realVnum += GetRefineMaxLevel(realVnum)
                if itemVnum != realVnum:
                    continue
            if not IsCanShowItems(itemVnum):
                continue
            if localeInfo.IsARABIC():
                itemName = item.GetItemName()
            else:
                itemName = item.GetItemName().lower()
            if itemName.find("+") != -1:
                itemName = itemName[:itemName.find("+")]
            tempName = list(itemName)
            for i in xrange(input_len):
                tempName[i]=list(input_text_real)[i]
            itemName = ""
            for x in xrange(len(tempName)):
                itemName+=tempName[x]
            if itemName in namesList:
                continue
            namesList.append(itemName)
            itemList.append([itemName, realVnum])
        if len(itemList) > 0:
            if len(itemList) > 1:
                itemList = sorted(itemList, key=get_length,reverse=False)
            self.children["selectedItem"] = itemList[0][1]
            itemSearch.SetInfoMessage(itemList[0][0])
        else:
            self.children["selectedItem"] = 0
            itemSearch.SetInfoMessage("")
        return True
        def OnKeyDown(self, key):
            if app.DIK_RETURN == key:
                self.StartSearchItem()
            return True
    def StartSearchItem(self):
        if not self.children.has_key("selectedItem"):
            return
        if self.children["selectedItem"] == 0:
            return
        self.Open(0, self.children["selectedItem"])
    # Search Item #
    
    # Mob Item #
    def __OnValueUpdateMob(self):
        itemSearch = self.children["mobSearch"]
        ui.EditLine.OnIMEUpdate(itemSearch)
        input_text_real = itemSearch.GetText()
        input_len = len(input_text_real)
        board = self.children["searchMobBoard"]
        totalWidth = 129
        if input_len > 0:
            totalWidth-=25
            windowHeight = self.GetWidth()-15
            textSize = itemSearch.GetTextSize()[0]
            if textSize >= totalWidth:
                if textSize >= windowHeight:
                    totalWidth = windowHeight
                else:
                    totalWidth = textSize
            totalWidth+=25
            if totalWidth >= windowHeight:
                totalWidth = windowHeight
        board.SetSize(totalWidth, 23)
        itemSearch.SetSize(totalWidth, 23)
        searchBtn = self.children["searchMobBtn"]
        searchBtn.SetPosition(itemSearch.GetWidth()-searchBtn.GetWidth(),2)
        if input_len == 0:
            self.ClearEditlineMob()
            return False
        if localeInfo.IsARABIC():
            input_text = input_text_real
        else:
            input_text = input_text_real.lower()
        searchBtn.Show()
        if input_len == 0:
            self.ClearEditlineMob()
            return False
        if localeInfo.IsARABIC():
            input_text = input_text_real
        else:
            input_text = input_text_real.lower()
        mobs_list = nonplayer.GetMobsByName(str(input_text))
        mobList = []
        namesList = []
        for i, mobVnum in enumerate(mobs_list, start=1):
            if localeInfo.IsARABIC():
                mob_name = nonplayer.GetMonsterName(mobVnum)
            else:
                mob_name = nonplayer.GetMonsterName(mobVnum).lower()
            tempName = list(mob_name)
            for i in xrange(input_len):
                tempName[i]=list(input_text_real)[i]
            mob_name = ""
            for x in xrange(len(tempName)):
                mob_name+=tempName[x]
            if mob_name in namesList:
                continue
            namesList.append(mob_name)
            mobList.append([mob_name, mobVnum])
        if len(mobList) > 0:
            if len(mobList) > 1:
                mobList = sorted(mobList, key=get_length,reverse=False)
            self.children["selectedMob"] = mobList[0][1]
            itemSearch.SetInfoMessage(mobList[0][0])
        else:
            self.children["selectedMob"] = 0
            itemSearch.SetInfoMessage("")
        return True
        def OnKeyDown(self, key):
            if app.DIK_RETURN == key:
                self.StartSearchMob()
            return True
    def StartSearchMob(self):
        if not self.children.has_key("selectedMob"):
            return
        if self.children["selectedMob"] == 0:
            return
        self.Open(1, self.children["selectedMob"])
    def ClearEditlineMob(self):
        self.children["selectedMob"]=0
        self.children["mobSearch"].SetText("")
        self.children["mobSearch"].SetInfoMessage(localeInfo.WIKI_MOB_NAME)
    # Mob Item #

class MultiFunctionalRender(ui.RenderTarget):
    def Close(self):
        renderTarget.SetVisibility(self.GetRenderIndex(), False)
        return True
    def __del__(self):
        ui.RenderTarget.__del__(self)
    def GetRenderIndex(self):
        if self.children.has_key("renderIndex"):
            return self.children["renderIndex"]
        return -1
    def CanCheckMouse(self):
        if renderTarget.IsShow(self.GetRenderIndex()) != 1:
            return False
        if self.children.has_key("isDrag"):
            return self.children["isDrag"]
        return False
    def Destroy(self):
        self.children={}
    def __init__(self, parent, x, y, width, height):
        ui.RenderTarget.__init__(self)
        self.Destroy()
        self.SetParent(parent)
        self.__LoadWindow(x, y, width, height)
    def __LoadWindow(self, x, y, width, height):
        renderIndex = renderTarget.GetFreeIndex(50, 85)
        self.SetSize(width, height)
        self.SetPosition(x, y)
        self.SetRenderTarget(renderIndex)
        self.Show()
        self.SetMouseRightButtonDownEvent(ui.__mem_func__(self.RenderMouseRightDown))
        self.SetMouseRightButtonUpEvent(ui.__mem_func__(self.RenderMouseRightUp))
        renderTarget.SetBackground(renderIndex, IMG_DIR+"preview_back.tga")
        renderTarget.SetRotation(renderIndex, False)
        renderTarget.SetScale(renderIndex, 0.3)
        self.children["renderIndex"] = renderIndex

        raceCount = 4
        if ENABLE_WOLFMAN:
            raceCount+=1

        raceList = []
        for j in xrange(raceCount):
            playerRace = ui.RadioButton()
            playerRace.SetParent(self)
            playerRace.SetUpVisual("%sface/race_%d_0.tga"%(IMG_DIR, j))
            playerRace.SetOverVisual("%sface/race_%d_1.tga"%(IMG_DIR, j))
            playerRace.SetDownVisual("%sface/race_%d_1.tga"%(IMG_DIR, j))
            playerRace.SetDisableVisual("%sface/race_%d_2.tga"%(IMG_DIR, j))
            x = ((width/2)-100)+j*(playerRace.GetWidth()+5)
            playerRace.SetPosition(x, height-playerRace.GetHeight()-5)
            playerRace.SAFE_SetEvent(self.SetRaceIndex, j)
            playerRace.Show()
            raceList.append(playerRace)
        self.children["raceList"] = raceList

        genderList = []
        for j in xrange(raceCount):
            genderType = ui.RadioButton()
            genderType.SetParent(self)
            genderType.SetUpVisual("%sgender/%d_0.tga"%(IMG_DIR, j))
            genderType.SetOverVisual("%sgender/%d_1.tga"%(IMG_DIR, j))
            genderType.SetDownVisual("%sgender/%d_1.tga"%(IMG_DIR, j))
            genderType.SetPosition(((width/2)-130) if j == 0 else (((width/2)-100)+4*(42+5)) , height-genderType.GetHeight()-30)
            genderType.SAFE_SetEvent(self.SetGenderIndex, j)
            genderType.Show()
            genderList.append(genderType)
        self.children["genderList"] = genderList

        self.children["motionIndex"] = 1
        self.children["motionList"] = [
            [1, "Wait"],
            [5, "Damage"],
            [7, "Stand Up"],
            [11, "Dead"],
            [13, "Normal Attack"],
            [33, "Skill 1"],
            [34, "Skill 2"],
            [35, "Skill 3"],
            [36, "Skill 4"],
        ]

        motionIndexList = ui.ComboBoxImage(self, IMG_DIR + "class_image.tga", 10, 10)
        motionIndexList.SetPosition(self.GetWidth()-motionIndexList.GetWidth()-10, 10)
        motionIndexList.SetParent(self)
        for data in self.children["motionList"]:
            motionIndexList.InsertItem(data[0], data[1])
        motionIndexList.SetEvent(lambda subItemNumber, argSelf=proxy(self): argSelf.__ClickMotionIndex(subItemNumber))
        motionIndexList.Show()
        self.children["motionIndexList"]=motionIndexList
        self.__ClickMotionIndex(1)

        refreshMotionIndex = ui.Button()
        refreshMotionIndex.SetParent(self)
        refreshMotionIndex.SetUpVisual(IMG_DIR+"refresh_0.tga")
        refreshMotionIndex.SetOverVisual(IMG_DIR+"refresh_1.tga")
        refreshMotionIndex.SetDownVisual(IMG_DIR+"refresh_2.tga")
        refreshMotionIndex.SetPosition(self.GetWidth()-refreshMotionIndex.GetWidth()-5-10,15)
        refreshMotionIndex.SAFE_SetEvent(self.RefreshMotionIndex)
        refreshMotionIndex.Show()
        self.children["refreshMotionIndex"]=refreshMotionIndex

        self.children["blockRace"]=[]

        self.SetGenderIndex(0, False)
        self.SetRaceIndex(0, False)

        moveText = ui.TextLine()
        moveText.SetParent(self)
        moveText.SetHorizontalAlignRight()
        moveText.SetPosition(self.GetWidth()-10, self.GetHeight()-75)
        moveText.SetText(localeInfo.RENDER_MOVE_TEXT)
        moveText.Show()
        self.children["moveText"]=moveText
        
        zoomText = ui.TextLine()
        zoomText.SetParent(self)
        zoomText.SetHorizontalAlignRight()
        zoomText.SetPosition(self.GetWidth()-10, self.GetHeight()-50)
        zoomText.SetText(localeInfo.RENDER_WHEEL_TEXT)
        zoomText.Show()
        self.children["zoomText"]=zoomText

    def RefreshMotionIndex(self):
        self.__ClickMotionIndex(self.children["motionIndex"])
    def __ClickMotionIndex(self, motionIndex, isFromButton = True):
        realData = None
        for data in self.children["motionList"]:
            if motionIndex == data[0]:
                realData = data
                break
        if realData == None:
            return
        self.children["motionIndex"] = realData[0]
        self.children["motionIndexList"].SetCurrentItem(realData[1])
        self.children["motionIndexList"].CloseListBox()
        if isFromButton:
            renderTarget.SetMotionIndex(self.GetRenderIndex(), motionIndex)

    def __ClickRadioButton(self, buttonList, buttonIndex):
        try:
            btn=buttonList[buttonIndex]
        except IndexError:
            return
        for eachButton in buttonList:
            eachButton.SetUp()
        btn.Down()
    def SetGenderIndex(self, genderIndex, isFromButton = True):
        self.__ClickRadioButton(self.children["genderList"], genderIndex)
        self.children["genderIndex"] = genderIndex

        if isFromButton:
            self.RefreshButtons()

    def SetRaceIndex(self, raceIndex, isFromButton = True):
        if raceIndex >= 4:
            raceIndex -= 4

        for j in xrange(len(self.children["raceList"])):
            if self.GetRealRace(self.children["genderIndex"])[j] in self.children["blockRace"]:
                continue
            if j == raceIndex:
                self.children["raceList"][j].Down()
            else:
                self.children["raceList"][j].SetUp()
        self.children["raceIndex"] = raceIndex
        if isFromButton:
            self.RefreshButtons()
    def RenderMouseRightUp(self):
        app.SetCursor(app.NORMAL)
        self.children["isDrag"] = False
        return True
    def RenderMouseRightDown(self):
        app.SetCursor(app.CAMERA_ROTATE)
        self.children["isDrag"] = True
        self.children["lastPos"] = app.GetCursorPosition()
        return True
    def OnMouseWheel(self, nLen):
        renderIndex = self.GetRenderIndex()
        if renderTarget.IsShow(renderIndex) != 1:
            return False
        renderTarget.Zoom(renderIndex, app.CAMERA_TO_NEGATIVE if nLen > 0 else app.CAMERA_TO_POSITIVE)
        return True

    def GetOtherSexRace(self, race):
        otherSexMapping = {
            playersettingmodule.RACE_WARRIOR_W : playersettingmodule.RACE_WARRIOR_M,
            playersettingmodule.RACE_ASSASSIN_W : playersettingmodule.RACE_ASSASSIN_M,
            playersettingmodule.RACE_SHAMAN_W :    playersettingmodule.RACE_SHAMAN_M,
            playersettingmodule.RACE_SURA_W :    playersettingmodule.RACE_SURA_M,
            playersettingmodule.RACE_WARRIOR_M :    playersettingmodule.RACE_WARRIOR_W,
            playersettingmodule.RACE_ASSASSIN_M :    playersettingmodule.RACE_ASSASSIN_W,
            playersettingmodule.RACE_SHAMAN_M :    playersettingmodule.RACE_SHAMAN_W,
            playersettingmodule.RACE_SURA_M : playersettingmodule.RACE_SURA_W,
        }
        return otherSexMapping[race]
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

    def CanEquipItem(self, raceIndex):
        ANTI_FLAG_DICT = {
            0 : item.ITEM_ANTIFLAG_WARRIOR,
            1 : item.ITEM_ANTIFLAG_ASSASSIN,
            2 : item.ITEM_ANTIFLAG_SURA,
            3 : item.ITEM_ANTIFLAG_SHAMAN,
        }
        (job,sex) = (chr.RaceToJob(raceIndex), chr.RaceToSex(raceIndex))
        (MALE, FEMALE) = (1, 0)
        if item.IsAntiFlag(ANTI_FLAG_DICT[job]):
            return 1
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
            return 2
        elif item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
            return 2
        return 0

    def GetRealRace(self, gender):
        return [0, 5, 2, 7] if gender == 0 else [4, 1, 6, 3]

    def IsAllRaceItems(self):
        (itemType, itemSubType) = (item.GetItemType(),item.GetItemSubType())
        if (item.ITEM_TYPE_COSTUME == itemType and (itemSubType == item.COSTUME_TYPE_HAIR or itemSubType == item.COSTUME_TYPE_BODY or itemSubType == item.COSTUME_TYPE_SKIN_SASH or itemSubType == item.COSTUME_TYPE_SASH)):
            return True
        elif item.ITEM_TYPE_ARMOR == itemType and itemSubType == item.ARMOR_BODY:
            return True
        return False
    def SetButtonShowStatus(self, status):
        map(lambda x : x.Show() if status == True else x.Hide(), self.children["genderList"])
        map(lambda x : x.Show() if status == True else x.Hide(), self.children["raceList"])
        
        if status:
            self.children["motionIndexList"].imagebox.Hide()
            self.children["refreshMotionIndex"].Hide()
            self.children["motionIndexList"].Hide()
        else:
            self.children["motionIndexList"].imagebox.Show()
            self.children["refreshMotionIndex"].Show()
            self.children["motionIndexList"].Show()

    def RefreshButtons(self):
        (renderType, vnumIndex) = (self.children["renderType"], self.children["vnumIndex"])
        if renderType != 0:
            self.SetButtonShowStatus(False)
            self.PrepareRenderTarget(renderType, vnumIndex, True)
            return
        else:
            self.SetButtonShowStatus(True)
            self.children["blockRace"]=[]

        (genderIndex, raceIndex) = (self.children["genderIndex"], self.children["raceIndex"])
        if genderIndex == 1:
            raceIndex+=4

        __old_race = raceIndex
        if not IsCanShowItems(vnumIndex):
            return

        isAllRaceItems = self.IsAllRaceItems()
        for j in xrange(4):
            genderRace = self.GetRealRace(genderIndex)[j]
            status = self.CanEquipItem(genderRace)
            if status == 0 or isAllRaceItems == True:
                self.children["raceList"][j].Enable()
            else:
                self.children["blockRace"].append(genderRace)
                self.children["raceList"][j].Disable()

        if raceIndex in self.children["blockRace"] or raceIndex-4 in self.children["blockRace"] or raceIndex+4 in self.children["blockRace"]:
            raceIndex = self.GetValidRace(raceIndex)

        #chat.AppendChat(1, "list:(%s) raceIndex: %d old_race: %d"%(str(self.children["blockRace"]), raceIndex, __old_race))

        self.SetRaceIndex(raceIndex-4  if raceIndex >= 4 else raceIndex, False)
        self.SetGenderIndex(1 if raceIndex >= 4 else 0, False)
        if raceIndex != __old_race:
            self.PrepareRenderTarget(renderType, vnumIndex, False)
            return
        self.PrepareRenderTarget(renderType, vnumIndex, True)

    def OnUpdate(self):
        if self.CanCheckMouse():
            [currentMousePos, lastPos] = [app.GetCursorPosition(), self.children["lastPos"]]
            totalPos = self.children["totalPos"] if self.children.has_key("totalPos") else [0, 0]

            _x = (currentMousePos[0] - lastPos[0]) + totalPos[0]
            _y = (currentMousePos[1] - lastPos[1]) + totalPos[1]

            fNewPitchVelocity = _y * 0.3
            fNewRotationVelocity = _x * 0.3
            renderTarget.RotateEyeAroundTarget(self.GetRenderIndex(), fNewPitchVelocity, fNewRotationVelocity)

            self.children["totalPos"] = [_x, _y]
            self.children["lastPos"] = currentMousePos

    def PrepareRenderTarget(self, renderType, vnumIndex, isFromRefresh = False):
        self.children["renderType"] = renderType
        self.children["vnumIndex"] = vnumIndex

        if not isFromRefresh:
            self.RefreshButtons()
            return
        renderIndex = self.GetRenderIndex()

        renderTarget.SetScale(renderIndex, 1.0)

        #renderTarget.SetVisibility(renderIndex, False)
        renderTarget.ResetModel(renderIndex)

        if renderType == 0:

            item.SelectItem(vnumIndex)
            itemType = item.GetItemType()
            itemSubType = item.GetItemSubType()

            isArmor = ((item.ITEM_TYPE_ARMOR == itemType and itemSubType == item.ARMOR_BODY) or (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_BODY))
            isWeapon = ((item.ITEM_TYPE_WEAPON == itemType and itemSubType != item.WEAPON_ARROW) or (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_WEAPON))
            isAcce = ((item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_SKIN_SASH) or (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_SASH))
            isHair = (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_HAIR)
            isMount = (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_MOUNT)

            if app.ENABLE_MOUNT_SKIN and isMount == False:
                isMount = (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_MOUNT_SKIN)

            isPet = (item.ITEM_TYPE_COSTUME == itemType and itemSubType == item.COSTUME_TYPE_PET)
            isShining = item.ITEM_TYPE_SHINING == itemType

            if isMount or isPet:
                self.PrepareRenderTarget(1, item.GetValue(0))
            else:
                #chat.AppendChat(1, "gender(%d) race(%d)"%(self.children["genderIndex"],self.children["raceIndex"]))
                renderTarget.SelectModel(renderIndex, self.GetRealRace(self.children["genderIndex"])[self.children["raceIndex"]])
                renderTarget.SetVisibility(renderIndex, True)
                renderTarget.SetWeapon(renderIndex, vnumIndex if isWeapon else 0)
                renderTarget.SetHair(renderIndex, vnumIndex if isHair else player.GetMainCharacterPart(4), False if isHair else True)
                renderTarget.SetAcce(renderIndex, vnumIndex-85000 if isAcce else player.GetMainCharacterPart(5))
                renderTarget.SetArmor(renderIndex, vnumIndex if isArmor else player.GetMainCharacterPart(0))

                if isShining:
                    slotIndex = 0
                    if itemSubType == item.SHINING_WEAPON:
                        slotIndex= 0
                    elif itemSubType == item.SHINING_ARMOR:
                        slotIndex = 1
                    elif itemSubType == item.SHINING_SPECIAL:
                        slotIndex = 4
                    renderTarget.SetShining(renderIndex, slotIndex, vnumIndex)
                    if itemSubType == item.SHINING_WEAPON:
                        renderTarget.SetWeapon(renderIndex, player.GetMainCharacterPart(1) if player.GetMainCharacterPart(1) > 0 else [329, 2509, 509, 7509][self.children["raceIndex"]])
                else:
                    for j in xrange(item.SHINING_SLOT_COUNT):
                        renderTarget.SetShining(renderIndex, j, player.GetMainCharacterShiningPart(j))
        #Mob
        elif renderType == 1:
            renderTarget.SelectModel(renderIndex, vnumIndex)
            renderTarget.SetVisibility(renderIndex, True)
            #renderTarget.SetArmor(renderIndex, 0)

            self.children["motionIndexList"].ClearItem()
            for data in self.children["motionList"]:
                if renderTarget.SetMotionIndex(renderIndex, data[0]):
                    self.children["motionIndexList"].InsertItem(data[0], data[1])

            self.__ClickMotionIndex(1)

class SearchSlotBoard(ui.Window):
    CORNER_WIDTH = 7
    CORNER_HEIGHT = 7
    LINE_WIDTH = 7
    LINE_HEIGHT = 7
    LT = 0
    LB = 1
    RT = 2
    RB = 3
    L = 0
    R = 1
    T = 2
    B = 3
    def __init__(self):
        ui.Window.__init__(self)
        self.MakeBoard()
        self.MakeBase()
    def MakeBoard(self):
        cornerPath = IMG_DIR+"board/corner_"
        linePath = IMG_DIR+"board/"
        CornerFileNames = [ cornerPath+dir+".tga" for dir in ("left_top", "left_bottom", "right_top", "right_bottom") ]
        LineFileNames = [ linePath+dir+".tga" for dir in ("left", "right", "top", "bottom") ]
        self.Corners = []
        for fileName in CornerFileNames:
            Corner = ui.ExpandedImageBox()
            Corner.AddFlag("not_pick")
            Corner.LoadImage(fileName)
            Corner.SetParent(self)
            Corner.SetPosition(0, 0)
            Corner.Show()
            self.Corners.append(Corner)
        self.Lines = []
        for fileName in LineFileNames:
            Line = ui.ExpandedImageBox()
            Line.AddFlag("not_pick")
            Line.LoadImage(fileName)
            Line.SetParent(self)
            Line.SetPosition(0, 0)
            Line.Show()
            self.Lines.append(Line)
        self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
        self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)
    def MakeBase(self):
        self.Base = ui.ExpandedImageBox()
        self.Base.AddFlag("not_pick")
        self.Base.LoadImage(IMG_DIR+"board/base.tga")
        self.Base.SetParent(self)
        self.Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
        self.Base.Show()
    def __del__(self):
        ui.Window.__del__(self)
    def Destroy(self):
        self.Base=0
        self.Corners=0
        self.Lines=0
        self.CORNER_WIDTH = 0
        self.CORNER_HEIGHT = 0
        self.LINE_WIDTH = 0
        self.LINE_HEIGHT = 0
        self.LT = 0
        self.LB = 0
        self.RT = 0
        self.RB = 0
        self.L = 0
        self.R = 0
        self.T = 0
        self.B = 0
    def SetSize(self, width, height):
        width = max(self.CORNER_WIDTH*2, width)
        height = max(self.CORNER_HEIGHT*2, height)
        ui.Window.SetSize(self, width, height)
        self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
        self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
        self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
        self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
        self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)
        verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
        horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
        self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
        self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
        self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
        self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
        if self.Base:
            self.Base.SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)

