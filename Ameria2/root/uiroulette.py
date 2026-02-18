# coding: latin_1

#author: dracaryS

#static
import ui, constInfo, time

#dynamic
import app, item, net

IMG_DIR = "d:/ymir work/ui/minigame/roulette/"

class Window(ui.BoardWithTitleBar):
    def __init__(self):
        ui.BoardWithTitleBar.__init__(self)
        self.__LoadWindow()
    def Destroy(self):
        self.__children = {}

    def GetPos(self, idx):
        (_x, _y) = (12, 11)
        if idx >= 0 and idx <= 5:
            _x += idx * 44
        elif idx >= 6 and idx <= 10:
            _x += 5 * 44
            _y += (idx - 5) * 44
        elif idx >= 11 and idx <= 15:
            _x += (5 * 44) - ((idx - 10) * 44)
            _y += 5 * 44
        elif idx >= 16 and idx <= 19:
            _y += (5 * 44) - ((idx - 15) * 44)
        return (_x, _y)

    def __LoadWindow(self):
        self.Destroy()

        self.AddFlag("attach")
        self.AddFlag("float")
        self.AddFlag("movable")
        self.SetTitleName("[F11] Christmas Roulette")
        
        self.SetCloseEvent(ui.__mem_func__(self.OnPressEscapeKey))

        bg = CreateWindow(ui.ImageBox(), self, (8, 32), IMG_DIR+"main_bg.sub")
        self.__children["bg"] = bg

        btn = CreateWindow(ui.Button(), bg, (0, 0))
        btn.SetUpVisual(IMG_DIR+"spin_button_default.sub")
        btn.SetOverVisual(IMG_DIR+"spin_button_over.sub")
        btn.SetDownVisual(IMG_DIR+"spin_button_down.sub")
        btn.SetDisableVisual(IMG_DIR+"spin_button_down.sub")
        btn.SetEvent(ui.__mem_func__(self.__ClickBtn))
        btn.SetText("Spin")
        btn.SetWindowHorizontalAlignCenter()
        btn.SetWindowVerticalAlignCenter()
        self.__children["btn"] = btn

        edge = CreateWindow(ui.ExpandedImageBox(), bg, (12, 11), IMG_DIR+"slot_edge.sub")
        self.__children["edge"] = edge

        _trailerItems = [[71084, 250], [50255, 50], [25041, 1], [72346, 2], [30624, 1], [51001, 15], [6884, 15], [30400, 100], [33031, 1], [30630, 1], \
            [30627, 10], [31233, 3], [70251, 3], [31258, 1], [31257, 1], [70428, 1], [31242, 3], [31243, 3], [70411, 1], [70412, 1], [31235, 1], \
            [31236, 1], [31237, 1], [50266, 1], [51116, 1]]

        for j in xrange(20):
            shuffle_list(_trailerItems)

            (x, y) = self.GetPos(j)
            itemImg = CreateWindow(SpecialImage(), bg, (x + 3, y + 4))
            self.__children["itemImg" + str(j)] = itemImg

            for itemData in _trailerItems:
                itemImg.LoadItem(*itemData)

        for j in xrange(20):
            shuffle_list(_trailerItems)
            for x in xrange(20):
                self.__children["itemImg" + str(x)].LoadItem(_trailerItems[x][0], _trailerItems[x][1])

        self.SetSize(8 + bg.GetWidth() + 9, 32 + bg.GetHeight() + 9)
        self.SetCenterPosition()

    def PrepareAlphaMode(self):
        self.__children["status"] = False
        self.__children["edgeStatus"] = False
        self.__children["edgeAlpha"] = 1.0
        self.__children["edge"].SetAlpha(1.0)

        btn = self.__children["btn"]
        btn.Enable()
        btn.SetText("Spin")

    def Start(self, gameKey = 0, itemData = "", selectedIdx = app.GetRandom(0, 19)):
        self.PrepareAlphaMode()

        itemList = []
        itemSplitVec = itemData.split("#")
        for splitData in itemSplitVec:
            itemData = splitData.split("?")
            if len(itemData) == 2:
                itemList.append([int(itemData[0]), int(itemData[1])])

        for j in xrange(len(itemList)):
            itemImg = self.__children["itemImg" + str(j)]
            itemImg.Clear()
            itemImg.LoadItem(itemList[j][0], itemList[j][1])

        self.__children["gameKey"] = gameKey
        self.__children["status"] = True
        self.__children["currentIdx"] = 0
        self.__children["selectedIdx"] = selectedIdx
        self.__children["totalSpin"] = app.GetRandom(4, 6)
        self.__children["animationSpeed"] = 0.35 / self.__children["totalSpin"]
        self.__children["spinRotation"] = 1 if app.GetRandom(0, 1) == 0 else -1
        self.__children["currentSpin"] = 0
        self.__children["totalMoved"] = 0

        btn = self.__children["btn"]
        btn.Disable()
        btn.SetText("Spining...")

    def OnUpdate(self):
        for j in xrange(20):
            self.__children["itemImg" + str(j)].OnUpdate()

        status = self.__children["status"] if self.__children.has_key("status") else False
        if not status:
            lastAlphaUpdate = self.__children["lastAlphaUpdate"] if self.__children.has_key("lastAlphaUpdate") else 0.0
            if lastAlphaUpdate > app.GetTime():
                return
            self.__children["lastAlphaUpdate"] = app.GetTime() + 0.01
            edgeStatus = self.__children["edgeStatus"] if self.__children.has_key("edgeStatus") else False
            edgeAlpha = self.__children["edgeAlpha"] if self.__children.has_key("edgeAlpha") else 1.0
            if not edgeStatus:
                edgeAlpha -= 0.02
                if edgeAlpha <= 0.4:
                    edgeAlpha = 0.4
                    self.__children["edgeStatus"] = True
            else:
                edgeAlpha += 0.02
                if edgeAlpha >= 1.0:
                    edgeAlpha = 1.0
                    self.__children["edgeStatus"] = False
            self.__children["edgeAlpha"] = edgeAlpha
            self.__children["edge"].SetAlpha(edgeAlpha)
        else:
            nextMoveTime = self.__children["nextMoveTime"] if self.__children.has_key("nextMoveTime") else 0.0
            if nextMoveTime > app.GetTime():
                return
            self.__children["nextMoveTime"] = app.GetTime() + (self.__children["animationSpeed"] * self.__children["currentSpin"])

            currentIdx = self.__children["currentIdx"]
            totalMoved = self.__children["totalMoved"]

            #if totalMoved >= 10 and app.GetRandom(0, 1) == 1:
            #    self.__children["spinRotation"] = -1 if self.__children["spinRotation"] == 1 else 1

            currentIdx += self.__children["spinRotation"]

            if currentIdx < 0:
                currentIdx = 19

            if currentIdx > 19:
                currentIdx = 0

            if totalMoved == 19:
                totalMoved = 0
                self.__children["currentSpin"] += 1
            else:
                totalMoved += 1

            (x, y) = self.GetPos(currentIdx)
            self.__children["edge"].SetPosition(x, y)

            self.__children["totalMoved"] = totalMoved
            self.__children["currentIdx"] = currentIdx

            if self.__children["currentSpin"] >= self.__children["totalSpin"] and self.__children["selectedIdx"]  == currentIdx:
                net.SendChatPacket("/roulette stop {}".format(self.__children["gameKey"]))
                self.PrepareAlphaMode()

    def __ClickBtn(self):
        lastClickTime = self.__children["lastClickTime"] if self.__children.has_key("lastClickTime") else 0.0
        if lastClickTime > app.GetTime():
            return
        self.__children["lastClickTime"] = app.GetTime() + 0.3
        net.SendChatPacket("/roulette start")
    def Open(self):
        self.Show()
        self.SetTop()
    def Close(self, isFromButton = False):
        if isFromButton:
            if (self.__children["status"] if self.__children.has_key("status") else False):
                net.SendChatPacket("/roulette stop {}".format(self.__children["gameKey"]))
                self.__children["status"] = False
                (x, y) = self.GetPos(self.__children["selectedIdx"])
                self.__children["edge"].SetPosition(x, y)
                self.PrepareAlphaMode()
        self.Hide()
    def OnPressEscapeKey(self):
        self.Close(True)
        return True

class SpecialImage(ui.ExpandedImageBox):
    def Destroy(self):
        self.imageList = []

        self.waitingTime = 0.0
        self.sleepTime = 0.0
        self.alphaValue = 0.0
        self.increaseValue = 0.0
        self.minAlpha = 0.0
        self.maxAlpha = 0.0
        self.alphaStatus = False
        self.imageIndex = 0
        self.numberText = None

    def __init__(self):
        ui.ExpandedImageBox.__init__(self)
        self.Destroy()
        self.waitingTime = 2.0
        self.alphaValue = 0.3
        self.increaseValue = 0.05
        self.minAlpha = 0.3
        self.maxAlpha = 1.0

        self.SAFE_SetStringEvent("MOUSE_OVER_OUT", self.OverOutItem)
        self.SAFE_SetStringEvent("MOUSE_OVER_IN", self.OverInItem)

    def SetImage(self, itemVnum, itemCount):
        if self.numberText == None:
            self.numberText = CreateWindow(ui.NumberLine(), self, (15, 20))
        self.numberText.SetNumber(str(itemCount))
        self.numberText.Show()
        item.SelectItem(itemVnum)
        self.LoadImage(item.GetIconImageFileName())
        self.Show()

    def Clear(self):
        if self.numberText:
            self.numberText.Hide()
        self.imageList = []

    def LoadItem(self, itemVnum, itemCount):
        item_list = [itemVnum, itemCount]
        if item_list in self.imageList:
            return
        if not len(self.imageList):
            self.SetImage(itemVnum, itemCount)
        self.imageList.append(item_list)

    def OnUpdate(self):
        if len(self.imageList) <= 1:
            self.imageIndex=0
            return
        elif self.sleepTime > app.GetTime():
            return
        if self.alphaStatus == True:
            self.alphaValue -= self.increaseValue
            if self.alphaValue < self.minAlpha:
                self.alphaValue = self.minAlpha
                self.alphaStatus = False
                (imageIndex, itemVnum, itemCount) = self.GetNextImage(self.imageIndex+1)
                if itemVnum != 0:
                    self.SetImage(itemVnum, itemCount)
                self.imageIndex = imageIndex
        else:
            self.alphaValue += self.increaseValue
            if self.alphaValue > self.maxAlpha:
                self.alphaStatus = True
                self.sleepTime = app.GetTime()+self.waitingTime
        self.SetAlpha(self.alphaValue)
    def GetNextImage(self, listIndex):
        if listIndex >= len(self.imageList):
            return (0, self.imageList[0][0], self.imageList[0][1]) if len(self.imageList) > 0 else (0, 0, 0)
        return (listIndex, self.imageList[listIndex][0], self.imageList[listIndex][1])
    def OverOutItem(self):
        interface = constInfo.GetInterfaceInstance()
        if interface:
            if interface.tooltipItem:
                interface.tooltipItem.HideToolTip()
    def OverInItem(self):
        interface = constInfo.GetInterfaceInstance()
        if interface:
            if interface.tooltipItem:
                interface.tooltipItem.SetItemToolTip(self.imageList[self.imageIndex][0])

def shuffle_list(lst):
    seed = int(time.time() * 1000)
    for i in range(len(lst)-1, 0, -1):
        j = seed % (i + 1)
        lst[i], lst[j] = lst[j], lst[i]
        seed = (seed + i + j) % 2**32

def CreateWindow(window, parent, windowPos, windowArgument = "", windowPositionRule = "", windowSize = (-1, -1), windowFontName = -1):
    window.SetParent(parent)
    window.SetPosition(*windowPos)
    if windowSize != (-1, -1):
        window.SetSize(*windowSize)
    if windowPositionRule:
        splitList = windowPositionRule.split(":")
        if len(splitList) == 2:
            (type, mode) = (splitList[0], splitList[1])
            if type == "horizontal":
                if isinstance(window, ui.TextLine):
                    if mode == "center":
                        window.SetHorizontalAlignCenter()
                    elif mode == "right":
                        window.SetHorizontalAlignRight()
                    elif mode == "left":
                        window.SetHorizontalAlignLeft()
                else:
                    if mode == "center":
                        window.SetWindowHorizontalAlignCenter()
                    elif mode == "right":
                        window.SetWindowHorizontalAlignRight()
                    elif mode == "left":
                        window.SetWindowHorizontalAlignLeft()
            elif type == "vertical":
                if isinstance(window, ui.TextLine):
                    if mode == "center":
                        window.SetVerticalAlignCenter()
                    elif mode == "top":
                        window.SetVerticalAlignTop()
                    elif mode == "bottom":
                        window.SetVerticalAlignBottom()
                else:
                    if mode == "top":
                        window.SetWindowVerticalAlignTop()
                    elif mode == "center":
                        window.SetWindowVerticalAlignCenter()
                    elif mode == "bottom":
                        window.SetWindowVerticalAlignBottom()
    if windowArgument:
        if isinstance(window, ui.TextLine):
            if windowFontName != -1:
                window.SetFontName(windowFontName)
            window.SetText(windowArgument)
        elif isinstance(window, ui.NumberLine):
            window.SetNumber(windowArgument)
        elif isinstance(window, ui.ExpandedImageBox) or isinstance(window, ui.ImageBox):
            window.LoadImage(windowArgument if windowArgument.find("gr2") == -1 else "icon/item/27995.tga")
    window.Show()
    return window