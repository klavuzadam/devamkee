# coding: latin_1

import app
import ui

class PythonLoader(ui.BoardWithTitleBar):
    def __init__(self):
        ui.BoardWithTitleBar.__init__(self)
        self.LoadWindow()

    def __del__(self):
        ui.BoardWithTitleBar.__del__(self)

    def Close(self):
        self.Hide()

    def LoadWindow(self):
        self.SetSize(180, 280)
        self.SetCenterPosition()
        self.AddFlag("movable")
        self.AddFlag("animate")
        self.SetTitleName("Python Loader")
        self.SetCloseEvent(ui.__mem_func__(self.Close))
        self.Show()

        self.refreshBtn = ui.Button()
        self.refreshBtn.SetParent(self)
        self.refreshBtn.SetPosition(100, 250)
        self.refreshBtn.SetUpVisual('d:/ymir work/ui/game/windows/tab_button_middle_01.sub')
        self.refreshBtn.SetOverVisual('d:/ymir work/ui/game/windows/tab_button_middle_02.sub')
        self.refreshBtn.SetDownVisual('d:/ymir work/ui/game/windows/tab_button_middle_03.sub')
        self.refreshBtn.SetText("Refresh")
        self.refreshBtn.SetEvent(ui.__mem_func__(self.__Refresh))
        self.refreshBtn.Show()

        self.loadBtn = ui.Button()
        self.loadBtn.SetParent(self)
        self.loadBtn.SetPosition(30, 250)
        self.loadBtn.SetUpVisual('d:/ymir work/ui/game/windows/tab_button_middle_01.sub')
        self.loadBtn.SetOverVisual('d:/ymir work/ui/game/windows/tab_button_middle_02.sub')
        self.loadBtn.SetDownVisual('d:/ymir work/ui/game/windows/tab_button_middle_03.sub')
        self.loadBtn.SetEvent(ui.__mem_func__(self.__Load))
        self.loadBtn.SetText("Load")
        self.loadBtn.Show()

        self.scrollBar = ui.ScrollBar()
        self.scrollBar.SetParent(self)
        self.scrollBar.SetPosition(145, 50)
        self.scrollBar.SetScrollBarSize(190)
        self.scrollBar.Show()

        self.listBox = ui.ListBoxEx()
        self.listBox.SetParent(self)
        self.listBox.SetPosition(20, 50)
        self.listBox.SetScrollBar(self.scrollBar)
        self.listBox.Show()

        self.__Refresh()

    def __Refresh(self):
        self.listBox.RemoveAllItems()
        fileNameList=app.GetFileList("./test/*.py")
        for fileName in fileNameList:
            self.listBox.AppendItem(Item(fileName))

    def __Load(self):        
        selectedItem = self.listBox.GetSelectedItem()
        if selectedItem:
            execfile('./test/' + selectedItem.GetText(), {})

class Item(ui.ListBoxEx.Item):
    def __init__(self, fileName):
        ui.ListBoxEx.Item.__init__(self)
        self.canLoad = 0
        self.text = fileName
        self.textLine = ui.MakeTextLine(self)
        self.textLine.SetText(fileName[:40])

    def __del__(self):
        ui.ListBoxEx.Item.__del__(self)

    def GetText(self):
        return self.text

    def SetSize(self, width, height):
        ui.ListBoxEx.Item.SetSize(self, 6*len(self.textLine.GetText()) + 4, height)

py = PythonLoader()
