# coding: latin_1

import app
import ime
import grp
import snd
import wndMgr
import item
import skill
import localeInfo
import dbg
# MARK_BUG_FIX
import guild
# END_OF_MARK_BUG_FIX
import constInfo
import math

if app.NEW_PET_SYSTEM:
	import petskill

if app.USE_OFFLINESHOP_SEARCH_V2:
	from operator import truediv
	import chat

from _weakref import proxy

def GenerateColor(r, g, b, a = 1.0):
	r = float(r) / 255.0
	g = float(g) / 255.0
	b = float(b) / 255.0
	return grp.GenerateColor(r, g, b, a)

BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
DARK_COLOR = grp.GenerateColor(0.2, 0.2, 0.2, 1.0)
BRIGHT_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)

if localeInfo.IsCANADA():
	SELECT_COLOR = grp.GenerateColor(0.9, 0.03, 0.01, 0.4)
else:
	SELECT_COLOR = grp.GenerateColor(0.0, 0.0, 0.5, 0.3)

WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.5)
HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)

createToolTipWindowDict = {}
def RegisterCandidateWindowClass(codePage, candidateWindowClass):
	EditLine.candidateWindowClassDict[codePage]=candidateWindowClass
def RegisterToolTipWindow(type, createToolTipWindow):
	createToolTipWindowDict[type]=createToolTipWindow

app.SetDefaultFontName(localeInfo.UI_DEF_FONT)

## Window Manager Event List##
##############################
## "OnMouseLeftButtonDown"
## "OnMouseLeftButtonUp"
## "OnMouseLeftButtonDoubleClick"
## "OnMouseRightButtonDown"
## "OnMouseRightButtonUp"
## "OnMouseRightButtonDoubleClick"
## "OnMouseDrag"
## "OnSetFocus"
## "OnKillFocus"
## "OnMouseOverIn"
## "OnMouseOverOut"
## "OnRender"
## "OnUpdate"
## "OnKeyDown"
## "OnKeyUp"
## "OnTop"
## "OnIMEUpdate" ## IME Only
## "OnIMETab"	## IME Only
## "OnIMEReturn" ## IME Only
##############################
## Window Manager Event List##


class __mem_func__:
	class __noarg_call__:
		def __init__(self, cls, obj, func):
			self.cls=cls
			self.obj=proxy(obj)
			self.func=proxy(func)

		def __call__(self, *arg):
			return self.func(self.obj)

	class __arg_call__:
		def __init__(self, cls, obj, func):
			self.cls=cls
			self.obj=proxy(obj)
			self.func=proxy(func)

		def __call__(self, *arg):
			return self.func(self.obj, *arg)

	def __init__(self, mfunc):
		if mfunc.im_func.func_code.co_argcount>1:
			self.call=__mem_func__.__arg_call__(mfunc.im_class, mfunc.im_self, mfunc.im_func)
		else:
			self.call=__mem_func__.__noarg_call__(mfunc.im_class, mfunc.im_self, mfunc.im_func)

	def __call__(self, *arg):
		return self.call(*arg)


class Window(object):
	def NoneMethod(cls):
		pass

	NoneMethod = classmethod(NoneMethod)

	def __init__(self, layer = "UI"):
		self.hWnd = None
		self.parentWindow = 0
		if app.ENABLE_FISH_GAME:
			self.onMoveDoneFunc=None
		self.onMouseLeftButtonUpEvent = None
		self.onRunMouseWheelEvent = None
		if app.ENABLE_MOUSEWHEEL_EVENT:
			self.onMouseWheelScrollEvent=None

		self.RegisterWindow(layer)
		self.mouseRightButtonUpEvent = None
		self.mouseRightButtonUpArgs = None
		self.mouseOverInEvent = None
		self.mouseOverInArgs = None
		self.mouseLeftButtonUpEvent = None
		self.mouseLeftButtonUpArgs = None
		self.mouseOverOutEvent = None
		self.mouseOverOutArgs = None
		if app.ENABLE_WIKI:
			self.exPos = (0,0)
			self.itsRendered = False
			self.itsNeedDoubleRender = False
			self.sortIndex = 0
		self.Hide()
		if app.ENABLE_SEND_TARGET_INFO:
			self.mouseLeftButtonDownEvent = None
			self.mouseLeftButtonDownArgs = None
			self.mouseLeftButtonUpEvent = None
			self.mouseLeftButtonUpArgs = None
			self.mouseLeftButtonDoubleClickEvent = None
			self.mouseRightButtonDownEvent = None
			self.mouseRightButtonDownArgs = None
			self.moveWindowEvent = None
			self.renderEvent = None
			self.renderArgs = None

			self.overInEvent = None
			self.overInArgs = None

			self.overOutEvent = None
			self.overOutArgs = None

			self.baseX = 0
			self.baseY = 0

			self.SetWindowName("NONAME_Window")
			
	def __del__(self):
		wndMgr.Destroy(self.hWnd)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.Register(self, layer)

	def Destroy(self):
		pass

	def GetWindowHandle(self):
		return self.hWnd

	def AddFlag(self, style):
		if style == "animation" or style == "animate":
			return
		wndMgr.AddFlag(self.hWnd, style)

	def IsRTL(self):
		return wndMgr.IsRTL(self.hWnd)

	def SetWindowName(self, Name):
		wndMgr.SetName(self.hWnd, Name)

	def GetWindowName(self):
		return wndMgr.GetName(self.hWnd)

	def SetMouseLeftButtonUpEvent(self, event, *args):
		self.mouseLeftButtonUpEvent = event
		self.mouseLeftButtonUpArgs = args

	def SetMouseOverInEvent(self, event, *args):
		self.mouseOverInEvent = event
		self.mouseOverInArgs = args

	def SetMouseOverOutEvent(self, event, *args):
		self.mouseOverOutEvent = event
		self.mouseOverOutArgs = args
	
	if app.ENABLE_ANIMATED_WINDOW:
		def RemoveAnimatedFlag(self):
			wndMgr.RemoveAnimatedFlag(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetParent(self, parent):
			if parent:
				wndMgr.SetParent(self.hWnd, parent.hWnd)
			else:
				wndMgr.SetParent(self.hWnd, 0)
	
		def SetAttachParent(self, parent):
			wndMgr.SetAttachParent(self.hWnd, parent.hWnd)
	else:
		def SetParent(self, parent):
			wndMgr.SetParent(self.hWnd, parent.hWnd)

	def SetParentProxy(self, parent):
		self.parentWindow=proxy(parent)
		wndMgr.SetParent(self.hWnd, parent.hWnd)

	def GetParentProxy(self):
		return self.parentWindow

	def SetPickAlways(self):
		wndMgr.SetPickAlways(self.hWnd)

	def SetWindowHorizontalAlignLeft(self):
		wndMgr.SetWindowHorizontalAlign(self.hWnd, wndMgr.HORIZONTAL_ALIGN_LEFT)

	def SetWindowHorizontalAlignCenter(self):
		wndMgr.SetWindowHorizontalAlign(self.hWnd, wndMgr.HORIZONTAL_ALIGN_CENTER)

	def SetWindowHorizontalAlignRight(self):
		wndMgr.SetWindowHorizontalAlign(self.hWnd, wndMgr.HORIZONTAL_ALIGN_RIGHT)

	def SetWindowVerticalAlignTop(self):
		wndMgr.SetWindowVerticalAlign(self.hWnd, wndMgr.VERTICAL_ALIGN_TOP)

	def SetWindowVerticalAlignCenter(self):
		wndMgr.SetWindowVerticalAlign(self.hWnd, wndMgr.VERTICAL_ALIGN_CENTER)

	def SetWindowVerticalAlignBottom(self):
		wndMgr.SetWindowVerticalAlign(self.hWnd, wndMgr.VERTICAL_ALIGN_BOTTOM)

	def SetTop(self):
		wndMgr.SetTop(self.hWnd)

	def Show(self, force_show = False):
		wndMgr.Show(self.hWnd, force_show)

	def Hide(self, force_hide = False):
		wndMgr.Hide(self.hWnd, force_hide)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetVisible(self, is_show):
			if is_show:
				self.Show()
			else:
				self.Hide()
	def Lock(self):
		wndMgr.Lock(self.hWnd)

	def Unlock(self):
		wndMgr.Unlock(self.hWnd)

	def IsShow(self):
		return wndMgr.IsShow(self.hWnd)

	def UpdateRect(self):
		wndMgr.UpdateRect(self.hWnd)

	def SetSize(self, width, height):
		wndMgr.SetWindowSize(self.hWnd, width, height)

	def GetWidth(self):
		return wndMgr.GetWindowWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetWindowHeight(self.hWnd)

	def GetLocalPosition(self):
		return wndMgr.GetWindowLocalPosition(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def GetLeft(self):
			x, y = self.GetLocalPosition()
			return x
	
		def GetGlobalLeft(self):
			x, y = self.GetGlobalPosition()
			return x
	
		def GetTop(self):
			x, y = self.GetLocalPosition()
			return y
	
		def GetGlobalTop(self):
			x, y = self.GetGlobalPosition()
			return y
	
		def GetRight(self):
			return self.GetLeft() + self.GetWidth()
	
		def GetBottom(self):
			return self.GetTop() + self.GetHeight()

	def GetGlobalPosition(self):
		return wndMgr.GetWindowGlobalPosition(self.hWnd)

	def GetMouseLocalPosition(self):
		return wndMgr.GetMouseLocalPosition(self.hWnd)

	def GetRect(self):
		return wndMgr.GetWindowRect(self.hWnd)
	if app.ENABLE_SEND_TARGET_INFO:
		def SetLeft(self, x):
			wndMgr.SetWindowPosition(self.hWnd, x, self.GetTop())
	def SetPosition(self, x, y, flag = False):
		if flag == True:
			self.exPos = (x,y)
		wndMgr.SetWindowPosition(self.hWnd, x, y)

	def SetCenterPosition(self, x = 0, y = 0):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)
	
	if app.ENABLE_SEND_TARGET_INFO:
		def SavePosition(self):
			self.baseX = self.GetLeft()
			self.baseY = self.GetTop()
	
		def UpdatePositionByScale(self, scale):
			self.SetPosition(self.baseX * scale, self.baseY * scale)
			
	def IsFocus(self):
		return wndMgr.IsFocus(self.hWnd)

	def SetFocus(self):
		wndMgr.SetFocus(self.hWnd)

	def KillFocus(self):
		wndMgr.KillFocus(self.hWnd)

	def GetChildCount(self):
		return wndMgr.GetChildCount(self.hWnd)
	def SetMouseRightButtonUpEvent(self, event, *args):
		self.mouseRightButtonUpEvent = event
		self.mouseRightButtonUpArgs = args

	def OnMouseRightButtonUp(self):
		if self.mouseRightButtonUpEvent:
			apply(self.mouseRightButtonUpEvent, self.mouseRightButtonUpArgs)
	# def IsIn(self):
		# return wndMgr.IsIn(self.hWnd)
	def IsIn(self, checkChilds = False):
		return wndMgr.IsIn(self.hWnd, checkChilds)
	def GetRenderBox(self):
		return wndMgr.GetRenderBox(self.hWnd)
	def SetInsideRender(self, val):
		wndMgr.SetInsideRender(self.hWnd, val)
	def IsInPosition(self):
		xMouse, yMouse = wndMgr.GetMousePosition()
		x, y = self.GetGlobalPosition()
		return xMouse >= x and xMouse < x + self.GetWidth() and yMouse >= y and yMouse < y + self.GetHeight()

	if app.ENABLE_SEND_TARGET_INFO:
		def IsInPosition(self):
			xMouse, yMouse = wndMgr.GetMousePosition()
			x, y = self.GetGlobalPosition()
			return xMouse >= x and xMouse < x + self.GetWidth() and yMouse >= y and yMouse < y + self.GetHeight()
	
		def SetMouseLeftButtonDownEvent(self, event, *args):
			self.mouseLeftButtonDownEvent = event
			self.mouseLeftButtonDownArgs = args
	
		def OnMouseLeftButtonDown(self):
			if self.mouseLeftButtonDownEvent:
				apply(self.mouseLeftButtonDownEvent, self.mouseLeftButtonDownArgs)

	# if app.ENABLE_SEND_TARGET_INFO:
		# def SetOnMouseLeftButtonUpEvent(self, event, *args):
			# self.mouseLeftButtonUpEvent = event
			# self.mouseLeftButtonUpArgs = args
	# else:
	def SetOnMouseLeftButtonUpEvent(self, event):
		self.onMouseLeftButtonUpEvent = event
	
	if app.ENABLE_SEND_TARGET_INFO: # app.ENABLE_VOICE_CHAT is dependent on SAFE_SetOverIn/OutEvent
		def SetMouseLeftButtonDoubleClickEvent(self, event):
			self.mouseLeftButtonDoubleClickEvent = event
	
		def OnMouseLeftButtonDoubleClick(self):
			if self.mouseLeftButtonDoubleClickEvent:
				self.mouseLeftButtonDoubleClickEvent()
	
		def SetMouseRightButtonDownEvent(self, event, *args):
			self.mouseRightButtonDownEvent = event
			self.mouseRightButtonDownArgs = args
	
		def OnMouseRightButtonDown(self):
			if self.mouseRightButtonDownEvent:
				apply(self.mouseRightButtonDownEvent, self.mouseRightButtonDownArgs)
	
		def SetMoveWindowEvent(self, event):
			self.moveWindowEvent = event
	
		def OnMoveWindow(self, x, y):
			if self.moveWindowEvent:
				self.moveWindowEvent(x, y)
	
		def SAFE_SetOverInEvent(self, func, *args):
			self.overInEvent = __mem_func__(func)
			self.overInArgs = args
	
		def SetOverInEvent(self, func, *args):
			self.overInEvent = func
			self.overInArgs = args
	
		def SAFE_SetOverOutEvent(self, func, *args):
			self.overOutEvent = __mem_func__(func)
			self.overOutArgs = args
	
		def SetOverOutEvent(self, func, *args):
			self.overOutEvent = func
			self.overOutArgs = args
	
		def OnMouseOverIn(self):
			if self.overInEvent:
				apply(self.overInEvent, self.overInArgs)
	
		def OnMouseOverOut(self):
			if self.overOutEvent:
				apply(self.overOutEvent, self.overOutArgs)
	
		def SAFE_SetRenderEvent(self, event, *args):
			self.renderEvent = __mem_func__(event)
			self.renderArgs = args
	
		def ClearRenderEvent(self):
			self.renderEvent = None
			self.renderArgs = None
	
		def OnRender(self):
			if self.renderEvent:
				apply(self.renderEvent, self.renderArgs)

	def OnMouseLeftButtonUp(self):
		if self.onMouseLeftButtonUpEvent:
			self.onMouseLeftButtonUpEvent()

	def OnRunMouseWheel(self, nLen):
		#pass
		if not self.onRunMouseWheelEvent:
			return False
		apply(self.onRunMouseWheelEvent, (bool(nLen < 0), ))
		return True
		
	def SetOnRunMouseWheelEvent(self, event):
		#pass
		self.onRunMouseWheelEvent = __mem_func__(event)

	if app.ENABLE_MOUSEWHEEL_EVENT:
		def SetMouseWheelScrollEvent(self, event):
			self.onMouseWheelScrollEvent = event
			wndMgr.SetScrollable(self.hWnd)
		
		
		def OnMouseWheelScroll(self, mode = "UP"): #mode could be value "UP" and "DOWN"
			print("OnMouseWheelScroll")
			if self.onMouseWheelScrollEvent:
				self.onMouseWheelScrollEvent(mode)

	if app.USE_CLIP_MASK:
		def SetClippingMaskRect(self, left, top, right, bottom):
			wndMgr.SetClippingMaskRect(self.hWnd, left, top, right, bottom)

		def SetClippingMaskWindow(self, clipping_mask_window):
			wndMgr.SetClippingMaskWindow(self.hWnd, clipping_mask_window.hWnd)

class ListBoxEx(Window):

	class Item(Window):
		def __init__(self):
			Window.__init__(self)

		def __del__(self):
			Window.__del__(self)

		def SetParent(self, parent):
			Window.SetParent(self, parent)
			self.parent=proxy(parent)

		def OnMouseLeftButtonDown(self):
			self.parent.SelectItem(self)

		def OnRender(self):
			if self.parent.GetSelectedItem()==self:
				self.OnSelectedRender()

		def OnSelectedRender(self):
			x, y = self.GetGlobalPosition()
			grp.SetColor(grp.GenerateColor(0.0, 0.0, 0.7, 0.7))
			grp.RenderBar(x, y, self.GetWidth(), self.GetHeight())

	def __init__(self):
		Window.__init__(self)

		self.viewItemCount=10
		self.basePos=0
		self.itemHeight=16
		self.itemStep=20
		self.selItem=0
		self.itemList=[]
		self.onSelectItemEvent = lambda *arg: None

		if localeInfo.IsARABIC():
			self.itemWidth=130
		else:
			self.itemWidth=100

		self.scrollBar=None
		self.__UpdateSize()

	def __del__(self):
		Window.__del__(self)

	if app.ENABLE_SWITCHBOT:
		def GetItems(self):
			return self.itemList

	def __UpdateSize(self):
		height=self.itemStep*self.__GetViewItemCount()

		self.SetSize(self.itemWidth, height)

	def IsEmpty(self):
		if len(self.itemList)==0:
			return 1
		return 0

	def SetItemStep(self, itemStep):
		self.itemStep=itemStep
		self.__UpdateSize()

	def SetItemSize(self, itemWidth, itemHeight):
		self.itemWidth=itemWidth
		self.itemHeight=itemHeight
		self.__UpdateSize()

	def SetViewItemCount(self, viewItemCount):
		self.viewItemCount=viewItemCount

	def SetSelectEvent(self, event):
		self.onSelectItemEvent = event

	def SetBasePos(self, basePos):
		for oldItem in self.itemList[self.basePos:self.basePos+self.viewItemCount]:
			oldItem.Hide()

		self.basePos=basePos

		pos=basePos
		for newItem in self.itemList[self.basePos:self.basePos+self.viewItemCount]:
			(x, y)=self.GetItemViewCoord(pos, newItem.GetWidth())
			newItem.SetPosition(x, y)
			newItem.Show()
			pos+=1

	

	def GetItemIndex(self, argItem):
		return self.itemList.index(argItem)

	def GetSelectedItem(self):
		return self.selItem

	def SelectIndex(self, index):

		if index >= len(self.itemList) or index < 0:
			self.selItem = None
			return

		try:
			self.selItem=self.itemList[index]
		except:
			pass

	def SelectItem(self, selItem):
		self.selItem=selItem
		self.onSelectItemEvent(selItem)

	def RemoveAllItems(self):
		self.selItem=None
		self.itemList=[]

		if self.scrollBar:
			self.scrollBar.SetPos(0)
			
	def RemoveItem(self, delItem):
		if delItem==self.selItem:
			self.selItem=None

		self.itemList.remove(delItem)

	def AppendItem(self, newItem):
		newItem.SetParent(self)
		newItem.SetSize(self.itemWidth, self.itemHeight)

		pos=len(self.itemList)
		if self.__IsInViewRange(pos):
			(x, y)=self.GetItemViewCoord(pos, newItem.GetWidth())
			newItem.SetPosition(x, y)
			newItem.Show()
		else:
			newItem.Hide()
		if app.ENABLE_MOUSEWHEEL_EVENT:
			newItem.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)
		self.itemList.append(newItem)

	def AppendItemWithIndex(self, index, newItem):
		newItem.SetParent(self)
		newItem.SetSize(self.itemWidth, self.itemHeight)
		#newItem.OnMouseWheel = self.OnMouseWheel
		
		pos=len(self.itemList)
		if self.__IsInViewRange(pos):
			(x, y)=self.GetItemViewCoord(pos, newItem.GetWidth())
			newItem.SetPosition(x, y)
			newItem.Show()
		else:
			newItem.Hide()
			
		if app.ENABLE_MOUSEWHEEL_EVENT:
			newItem.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)

		self.itemList.insert(index,newItem)

	def SetScrollBar(self, scrollBar):
		scrollBar.SetScrollEvent(__mem_func__(self.__OnScroll))
		self.scrollBar=scrollBar

		if app.ENABLE_MOUSEWHEEL_EVENT:
			self.SetMouseWheelScrollEvent(self.OnMouseWheelScroll_ScrollBar)

	if app.ENABLE_MOUSEWHEEL_EVENT:
		def OnMouseWheelScroll_ScrollBar(self,mode):
			#chat.AppendChat(chat.CHAT_TYPE_INFO, "--")
			if mode == "UP":
				self.scrollBar.OnUp()
			else:
				self.scrollBar.OnDown()

			#eventDct = { "UP" : lambda : self.scrollBar.SetPos(self.curPos - (self.scrollStep/4)) , "DOWN" : lambda: self.scrollBar.SetPos(self.curPos + (self.scrollStep/4)) }
			#if mode in eventDct:
			#	eventDct[mode]()

	def __OnScroll(self):
		self.SetBasePos(int(self.scrollBar.GetPos()*self.__GetScrollLen()))

	def __GetScrollLen(self):
		scrollLen=self.__GetItemCount()-self.__GetViewItemCount()
		if scrollLen<0:
			return 0

		return scrollLen

	def __GetViewItemCount(self):
		return self.viewItemCount

	def __GetItemCount(self):
		return len(self.itemList)

	def GetItemViewCoord(self, pos, itemWidth):
		if localeInfo.IsARABIC():
			return (self.GetWidth()-itemWidth-10, (pos-self.basePos)*self.itemStep)
		else:
			return (0, (pos-self.basePos)*self.itemStep)

	def __IsInViewRange(self, pos):
		if pos<self.basePos:
			return 0
		if pos>=self.basePos+self.viewItemCount:
			return 0
		return 1
		
if app.ENABLE_SEND_TARGET_INFO:
	class ListBoxExNew(Window):
		class Item(Window):
			def __init__(self):
				Window.__init__(self)

				self.realWidth = 0
				self.realHeight = 0

				self.removeTop = 0
				self.removeBottom = 0

				self.SetWindowName("NONAME_ListBoxExNew_Item")

			def __del__(self):
				Window.__del__(self)

			def SetParent(self, parent):
				Window.SetParent(self, parent)
				self.parent=proxy(parent)

			def SetSize(self, width, height):
				self.realWidth = width
				self.realHeight = height
				Window.SetSize(self, width, height)

			def SetRemoveTop(self, height):
				self.removeTop = height
				self.RefreshHeight()

			def SetRemoveBottom(self, height):
				self.removeBottom = height
				self.RefreshHeight()

			def SetCurrentHeight(self, height):
				Window.SetSize(self, self.GetWidth(), height)

			def GetCurrentHeight(self):
				return Window.GetHeight(self)

			def ResetCurrentHeight(self):
				self.removeTop = 0
				self.removeBottom = 0
				self.RefreshHeight()

			def RefreshHeight(self):
				self.SetCurrentHeight(self.GetHeight() - self.removeTop - self.removeBottom)

			def GetHeight(self):
				return self.realHeight

		def __init__(self, stepSize, viewSteps):
			Window.__init__(self)

			self.viewItemCount=10
			self.basePos=0
			self.baseIndex=0
			self.maxSteps=0
			self.viewSteps = viewSteps
			self.stepSize = stepSize
			self.itemList=[]

			self.scrollBar=None

			self.SetWindowName("NONAME_ListBoxEx")

		def __del__(self):
			Window.__del__(self)

		def IsEmpty(self):
			if len(self.itemList)==0:
				return 1
			return 0

		def __CheckBasePos(self, pos):
			self.viewItemCount = 0

			start_pos = pos

			height = 0
			while height < self.GetHeight():
				if pos >= len(self.itemList):
					return start_pos == 0
				height += self.itemList[pos].GetHeight()
				pos += 1
				self.viewItemCount += 1
			return height == self.GetHeight()

		def SetBasePos(self, basePos, forceRefresh = TRUE):
			if forceRefresh == FALSE and self.basePos == basePos:
				return

			for oldItem in self.itemList[self.baseIndex:self.baseIndex+self.viewItemCount]:
				oldItem.ResetCurrentHeight()
				oldItem.Hide()

			self.basePos=basePos

			baseIndex = 0
			while basePos > 0:
				basePos -= self.itemList[baseIndex].GetHeight() / self.stepSize
				if basePos < 0:
					self.itemList[baseIndex].SetRemoveTop(self.stepSize * abs(basePos))
					break
				baseIndex += 1
			self.baseIndex = baseIndex

			stepCount = 0
			self.viewItemCount = 0
			while baseIndex < len(self.itemList):
				stepCount += self.itemList[baseIndex].GetCurrentHeight() / self.stepSize
				self.viewItemCount += 1
				if stepCount > self.viewSteps:
					self.itemList[baseIndex].SetRemoveBottom(self.stepSize * (stepCount - self.viewSteps))
					break
				elif stepCount == self.viewSteps:
					break
				baseIndex += 1

			y = 0
			for newItem in self.itemList[self.baseIndex:self.baseIndex+self.viewItemCount]:
				newItem.SetPosition(0, y)
				newItem.Show()
				y += newItem.GetCurrentHeight()

		def GetItemIndex(self, argItem):
			return self.itemList.index(argItem)

		def GetSelectedItem(self):
			return self.selItem

		def GetSelectedItemIndex(self):
			return self.selItemIdx

		def RemoveAllItems(self):
			self.itemList=[]
			self.maxSteps=0

			if self.scrollBar:
				self.scrollBar.SetPos(0)

		def RemoveItem(self, delItem):
			self.maxSteps -= delItem.GetHeight() / self.stepSize
			self.itemList.remove(delItem)

		def AppendItem(self, newItem):
			if newItem.GetHeight() % self.stepSize != 0:
				import dbg
				dbg.TraceError("Invalid AppendItem height %d stepSize %d" % (newItem.GetHeight(), self.stepSize))
				return

			self.maxSteps += newItem.GetHeight() / self.stepSize
			newItem.SetParent(self)
			self.itemList.append(newItem)

		def SetScrollBar(self, scrollBar):
			scrollBar.SetScrollEvent(__mem_func__(self.__OnScroll))
			self.scrollBar = scrollBar

		def __OnScroll(self):
			if self.scrollBar:
				self.SetBasePos(int(self.scrollBar.GetPos()*self.__GetScrollLen()), FALSE)

		def __GetScrollLen(self):
			scrollLen=self.maxSteps-self.viewSteps
			if scrollLen<0:
				return 0

			return scrollLen

		def __GetViewItemCount(self):
			return self.viewItemCount

		def __GetItemCount(self):
			return len(self.itemList)

		def GetViewItemCount(self):
			return self.viewItemCount

		def GetItemCount(self):
			return len(self.itemList)
	
class CandidateListBox(ListBoxEx):

	HORIZONTAL_MODE = 0
	VERTICAL_MODE = 1

	class Item(ListBoxEx.Item):
		def __init__(self, text):
			ListBoxEx.Item.__init__(self)

			self.textBox=TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			self.textBox.Show()

		def __del__(self):
			ListBoxEx.Item.__del__(self)

	def __init__(self, mode = HORIZONTAL_MODE):
		ListBoxEx.__init__(self)
		self.itemWidth=32
		self.itemHeight=32
		self.mode = mode

	def __del__(self):
		ListBoxEx.__del__(self)

	def SetMode(self, mode):
		self.mode = mode

	def AppendItem(self, newItem):
		ListBoxEx.AppendItem(self, newItem)

	def GetItemViewCoord(self, pos):
		if self.mode == self.HORIZONTAL_MODE:
			return ((pos-self.basePos)*self.itemStep, 0)
		elif self.mode == self.VERTICAL_MODE:
			return (0, (pos-self.basePos)*self.itemStep)


class TextLine(Window):
	def __init__(self, font = None):
		Window.__init__(self)
		self.max = 0
		#self.SetFontName(constInfo.GetFont())

		if font == None:
			self.SetFontName(localeInfo.UI_DEF_FONT)
		else:
			self.SetFontName(font)

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterTextLine(self, layer)

	def SetMax(self, max):
		wndMgr.SetMax(self.hWnd, max)

	def SetLimitWidth(self, width):
		wndMgr.SetLimitWidth(self.hWnd, width)

	def SetMultiLine(self):
		wndMgr.SetMultiLine(self.hWnd, True)

	def SetHorizontalAlignArabic(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_ARABIC)

	def SetHorizontalAlignLeft(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_LEFT)

	def SetHorizontalAlignRight(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_RIGHT)

	def SetHorizontalAlignCenter(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_CENTER)

	def SetVerticalAlignTop(self):
		wndMgr.SetVerticalAlign(self.hWnd, wndMgr.TEXT_VERTICAL_ALIGN_TOP)

	def SetVerticalAlignBottom(self):
		wndMgr.SetVerticalAlign(self.hWnd, wndMgr.TEXT_VERTICAL_ALIGN_BOTTOM)

	def SetVerticalAlignCenter(self):
		wndMgr.SetVerticalAlign(self.hWnd, wndMgr.TEXT_VERTICAL_ALIGN_CENTER)

	def SetSecret(self, Value=True):
		wndMgr.SetSecret(self.hWnd, Value)

	def SetOutline(self, Value=True):
		wndMgr.SetOutline(self.hWnd, Value)

	def SetFeather(self, value=True):
		wndMgr.SetFeather(self.hWnd, value)

	def SetFontName(self, fontName):
		wndMgr.SetFontName(self.hWnd, fontName)

	def SetDefaultFontName(self):
		wndMgr.SetFontName(self.hWnd, localeInfo.UI_DEF_FONT)
		
	def GetCharSize(self):
		return wndMgr.GetCharSize(self.hWnd)

	def SetFontColor(self, red, green, blue):
		wndMgr.SetFontColor(self.hWnd, red, green, blue)

	def SetPackedFontColor(self, color):
		wndMgr.SetFontColor(self.hWnd, color)

	def SetText(self, text):
		wndMgr.SetText(self.hWnd, text)

	def SetAllAlign(self):
		self.SetHorizontalAlignCenter()
		self.SetVerticalAlignCenter()
		self.SetWindowHorizontalAlignCenter()
		self.SetWindowVerticalAlignCenter()

	def GetText(self):
		return wndMgr.GetText(self.hWnd)

	def GetTextSize(self):
		return wndMgr.GetTextSize(self.hWnd)
		
	if app.ENABLE_IKASHOP_RENEWAL:
		def FitText(self):
			self.SetSize(*self.GetTextSize())

class EmptyCandidateWindow(Window):
	def __init__(self):
		Window.__init__(self)

	def __del__(self):
		Window.__init__(self)

	def Load(self):
		pass

	def SetCandidatePosition(self, x, y, textCount):
		pass

	def Clear(self):
		pass

	def Append(self, text):
		pass

	def Refresh(self):
		pass

	def Select(self):
		pass

class EditLine(TextLine):
	candidateWindowClassDict = {}

	def __init__(self):
		TextLine.__init__(self)

		self.eventReturn = Window.NoneMethod
		self.eventEscape = Window.NoneMethod
		self.eventKeyUp = Window.NoneMethod
		self.eventTab = None
		self.numberMode = False
		self.useIME = True
		self.CanClick = None

		self.bCodePage = False

		self.infoMsg = ""
		self.backText=None
		self.isNeedEmpty=True

		if app.ENABLE_IKASHOP_RENEWAL or app.USE_OFFLINESHOP_SEARCH_V2:
			self.updateEvent = None

		self.candidateWindowClass = None
		self.candidateWindow = None
		self.SetCodePage(app.GetDefaultCodePage())

		self.readingWnd = ReadingWnd()
		self.readingWnd.Hide()

		if app.USE_OFFLINESHOP_SEARCH_V2:
			self.overLay = TextLine()
			self.overLay.SetParent(self)
			self.overLay.SetPosition(0, 0)
			self.overLay.SetPackedFontColor(WHITE_COLOR)
			self.overLay.Hide()

	def __del__(self):
		TextLine.__del__(self)

		self.eventReturn = Window.NoneMethod
		self.eventEscape = Window.NoneMethod
		self.eventTab = None


	def SetCodePage(self, codePage):
		candidateWindowClass=EditLine.candidateWindowClassDict.get(codePage, EmptyCandidateWindow)
		self.__SetCandidateClass(candidateWindowClass)

	def __SetCandidateClass(self, candidateWindowClass):
		if self.candidateWindowClass==candidateWindowClass:
			return

		self.candidateWindowClass = candidateWindowClass
		self.candidateWindow = self.candidateWindowClass()
		self.candidateWindow.Load()
		self.candidateWindow.Hide()

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterTextLine(self, layer)

	def SAFE_SetReturnEvent(self, event):
		self.eventReturn = __mem_func__(event)

	def SetReturnEvent(self, event):
		self.eventReturn = event

	def SetEscapeEvent(self, event):
		self.eventEscape = event

	def SetKeyUpEvent(self, event):
		self.eventKeyUp = event

	def SetTabEvent(self, event):
		self.eventTab = event

	def CanEdit(self, flag):
		self.CanClick = flag

	if app.ENABLE_IKASHOP_RENEWAL or app.USE_OFFLINESHOP_SEARCH_V2:
		def SetUpdateEvent(self, event):
			self.updateEvent = event

	def SetMax(self, max):
		self.max = max
		wndMgr.SetMax(self.hWnd, self.max)
		ime.SetMax(self.max)
		self.SetUserMax(self.max)

	def SetUserMax(self, max):
		self.userMax = max
		ime.SetUserMax(self.userMax)

	def SetNumberMode(self):
		self.numberMode = True

	def SetInfoMessage(self, msg):
		self.infoMsg = msg
		if self.backText == None:
			self.backText = TextLine()
			self.backText.SetParent(self)
			self.backText.SetPosition(0,0)
			self.backText.SetFontColor(128,128,128)

		self.backText.SetText(msg)

		if self.isNeedEmpty:
			if len(self.GetText()) > 0:
				self.backText.Hide()
			else:
				self.backText.Show()
		else:
			self.backText.Show()

	#def AddExceptKey(self, key):
	#	ime.AddExceptKey(key)

	#def ClearExceptKey(self):
	#	ime.ClearExceptKey()

	def SetIMEFlag(self, flag):
		self.useIME = flag

	if app.USE_OFFLINESHOP_SEARCH_V2:
		def SetOverlayText(self, text):
			self.overLay.SetText(text)
			self.__RefreshOverlay()

		def GetOverlayText(self):
			return self.overLay.GetText()

		if not app.ENABLE_IKASHOP_RENEWAL:
			def SetUpdateEvent(self, event):
				self.eventUpdate = event

		def GetDisplayText(self):
			if len(self.GetText()):
				return self.GetText()
			else:
				return self.overLay.GetText()

		def __RefreshOverlay(self):
			if len(self.GetText()):
				self.overLay.Hide()
			else:
				self.overLay.Show()

		def IsShowCursor(self):
			return wndMgr.IsShowCursor(self.hWnd)

	def SetText(self, text):
		wndMgr.SetText(self.hWnd, text)

		if self.IsFocus():
			ime.SetText(text)

		if app.USE_OFFLINESHOP_SEARCH_V2:
			self.__RefreshOverlay()

	def Enable(self):
		wndMgr.ShowCursor(self.hWnd)

	def Disable(self):
		wndMgr.HideCursor(self.hWnd)

	def SetEndPosition(self):
		ime.MoveEnd()

	def OnSetFocus(self):
		Text = self.GetText()
		ime.SetText(Text)
		ime.SetMax(self.max)
		ime.SetUserMax(self.userMax)
		ime.SetCursorPosition(-1)
		if self.numberMode:
			ime.SetNumberMode()
		else:
			ime.SetStringMode()
		ime.EnableCaptureInput()
		if self.useIME:
			ime.EnableIME()
		else:
			ime.DisableIME()
		wndMgr.ShowCursor(self.hWnd, True)

		if app.ENABLE_IKASHOP_RENEWAL:
			if self.updateEvent:
				self.updateEvent(True)

	def OnKillFocus(self):
		self.SetText(ime.GetText(self.bCodePage))
		self.OnIMECloseCandidateList()
		self.OnIMECloseReadingWnd()
		ime.DisableIME()
		ime.DisableCaptureInput()
		wndMgr.HideCursor(self.hWnd)

		if app.ENABLE_IKASHOP_RENEWAL:
			if self.updateEvent:
				self.updateEvent(False)

	def OnIMEChangeCodePage(self):
		self.SetCodePage(ime.GetCodePage())

	def OnIMEOpenCandidateList(self):
		self.candidateWindow.Show()
		self.candidateWindow.Clear()
		self.candidateWindow.Refresh()

		gx, gy = self.GetGlobalPosition()
		self.candidateWindow.SetCandidatePosition(gx, gy, len(self.GetText()))

		return True

	def OnIMECloseCandidateList(self):
		self.candidateWindow.Hide()
		return True

	def OnIMEOpenReadingWnd(self):
		gx, gy = self.GetGlobalPosition()
		textlen = len(self.GetText())-2
		reading = ime.GetReading()
		readinglen = len(reading)
		self.readingWnd.SetReadingPosition( gx + textlen*6-24-readinglen*6, gy )
		self.readingWnd.SetText(reading)
		if ime.GetReadingError() == 0:
			self.readingWnd.SetTextColor(0xffffffff)
		else:
			self.readingWnd.SetTextColor(0xffff0000)
		self.readingWnd.SetSize(readinglen * 6 + 4, 19)
		self.readingWnd.Show()
		return True

	def OnIMECloseReadingWnd(self):
		self.readingWnd.Hide()
		return True

	def OnIMEUpdate(self):
		snd.PlaySound("sound/ui/type.wav")
		TextLine.SetText(self, ime.GetText(self.bCodePage))

		if self.backText:
			if self.isNeedEmpty:
				if len(self.GetText()) > 0:
					self.backText.Hide()
				else:
					self.backText.Show()
			else:
				self.backText.Show()

		if app.ENABLE_IKASHOP_RENEWAL or app.USE_OFFLINESHOP_SEARCH_V2:
			if app.USE_OFFLINESHOP_SEARCH_V2:
				self.__RefreshOverlay()

			if self.updateEvent:
				self.updateEvent(self.IsFocus())

	def OnIMETab(self):
		if self.eventTab:
			self.eventTab()
			return True

		return False

	def OnIMEReturn(self):
		snd.PlaySound("sound/ui/click.wav")
		self.eventReturn()

		return True

	def OnPressEscapeKey(self):
		if self.IsFocus():
			self.eventEscape()
			return True
		return False

	def OnKeyDown(self, key):
		if app.DIK_F1 == key:
			return False
		if app.DIK_F2 == key:
			return False
		if app.DIK_F3 == key:
			return False
		if app.DIK_F4 == key:
			return False
		if app.DIK_LALT == key:
			return False
		if app.DIK_SYSRQ == key:
			return False
		if app.DIK_LCONTROL == key:
			return False
		if app.DIK_V == key:
			if app.IsPressed(app.DIK_LCONTROL):
				ime.PasteTextFromClipBoard()

		return True

	def OnKeyUp(self, key):
		if self.eventKeyUp != Window.NoneMethod:
			self.eventKeyUp()
			return True

		if app.DIK_F1 == key:
			return False
		if app.DIK_F2 == key:
			return False
		if app.DIK_F3 == key:
			return False
		if app.DIK_F4 == key:
			return False
		if app.DIK_LALT == key:
			return False
		if app.DIK_SYSRQ == key:
			return False
		if app.DIK_LCONTROL == key:
			return False

		return True

	def OnIMEKeyDown(self, key):
		# Left
		if app.VK_LEFT == key:
			ime.MoveLeft()
			return True
		# Right
		if app.VK_RIGHT == key:
			ime.MoveRight()
			return True

		# Home
		if app.VK_HOME == key:
			ime.MoveHome()
			return True
		# End
		if app.VK_END == key:
			ime.MoveEnd()
			return True

		# Delete
		if app.VK_DELETE == key:
			ime.Delete()
			TextLine.SetText(self, ime.GetText(self.bCodePage))
			return True

		return True

	#def OnMouseLeftButtonDown(self):
	#	self.SetFocus()
	def OnMouseLeftButtonDown(self):
		if False == self.IsIn():
			return False

		if False == self.CanClick:
			return

		self.SetFocus()
		PixelPosition = wndMgr.GetCursorPosition(self.hWnd)
		ime.SetCursorPosition(PixelPosition)

class MarkBox(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterMarkBox(self, layer)

	def Load(self):
		wndMgr.MarkBox_Load(self.hWnd)

	def SetScale(self, scale):
		wndMgr.MarkBox_SetScale(self.hWnd, scale)

	def SetIndex(self, guildID):
		MarkID = guild.GuildIDToMarkID(guildID)
		wndMgr.MarkBox_SetImageFilename(self.hWnd, guild.GetMarkImageFilenameByMarkID(MarkID))
		wndMgr.MarkBox_SetIndex(self.hWnd, guild.GetMarkIndexByMarkID(MarkID))

	def SetAlpha(self, alpha):
		wndMgr.MarkBox_SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)
		
class ImageBoxSungMahi(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		
		self.eventDict = {
			"MOUSE_LEFT_BUTTON_UP" : None, 
			"MOUSE_LEFT_BUTTON_DOWN" : None, 
			"MOUSE_RIGHT_BUTTON_UP" : None, 
			"MOUSE_RIGHT_BUTTON_DOWN" : None, 
			"MOUSE_OVER_IN" : None, 
			"MOUSE_OVER_OUT" : None
		}

		self.eventFunc = {
			"MOUSE_LEFT_BUTTON_UP" : None, 
			"MOUSE_LEFT_BUTTON_DOWN" : None, 
			"MOUSE_RIGHT_BUTTON_UP" : None, 
			"MOUSE_RIGHT_BUTTON_DOWN" : None, 
			"MOUSE_OVER_IN" : None, 
			"MOUSE_OVER_OUT" : None
		}
		self.eventArgs = {
			"MOUSE_LEFT_BUTTON_UP" : None, 
			"MOUSE_LEFT_BUTTON_DOWN" : None, 
			"MOUSE_RIGHT_BUTTON_UP" : None, 
			"MOUSE_RIGHT_BUTTON_DOWN" : None, 
			"MOUSE_OVER_IN" : None, 
			"MOUSE_OVER_OUT" : None
		}
		
	def __del__(self):
		Window.__del__(self)	
		self.eventFunc = None
		self.eventArgs = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterImageBox(self, layer)

	def LoadImage(self, imageName):
		self.name=imageName
		wndMgr.LoadImage(self.hWnd, imageName)

	def UnloadImage(self):
		wndMgr.UnloadImage(self.hWnd)
		
	def SetScale(self, xScale, yScale):
		wndMgr.SetScale(self.hWnd, xScale, yScale)
		
	def SetAlpha(self, alpha):
		wndMgr.SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)
		
	def SetColor(self, r, g, b, a):
		wndMgr.SetDiffuseColor(self.hWnd, r, g, b, a)

	def SetImageSize(self,w,h):
		wndMgr.SetImageSize(self.hWnd,w,h)

	def GetWidth(self):
		return wndMgr.GetWidth(self.hWnd)
			
	def GetHeight(self):
		return wndMgr.GetHeight(self.hWnd)
		
	def SetEvent(self, func, *args) :
		result = self.eventFunc.has_key(args[0])		
		if result :
			self.eventFunc[args[0]] = func
			self.eventArgs[args[0]] = args
		else :
			print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]
			
	def SAFE_SetEvent(self, func, *args):
		result = self.eventFunc.has_key(args[0])		
		if result :
			self.eventFunc[args[0]] = __mem_func__(func)
			self.eventArgs[args[0]] = args
		else :
			print "[ERROR] ui.py SAFE_SetEvent, Can`t Find has_key : %s" % args[0]

	def OnMouseLeftButtonUp(self):
		if self.eventFunc["MOUSE_LEFT_BUTTON_UP"] :
			apply(self.eventFunc["MOUSE_LEFT_BUTTON_UP"], self.eventArgs["MOUSE_LEFT_BUTTON_UP"])
		if self.eventDict["MOUSE_LEFT_BUTTON_UP"]:
			apply(self.eventDict["MOUSE_LEFT_BUTTON_UP"], self.eventArgs["MOUSE_LEFT_BUTTON_UP"])
			
	def OnMouseLeftButtonDown(self):
		if self.eventFunc["MOUSE_LEFT_BUTTON_DOWN"] :
			apply(self.eventFunc["MOUSE_LEFT_BUTTON_DOWN"], self.eventArgs["MOUSE_LEFT_BUTTON_DOWN"])
		if self.eventDict["MOUSE_LEFT_BUTTON_DOWN"]:
			apply(self.eventDict["MOUSE_LEFT_BUTTON_DOWN"], self.eventArgs["MOUSE_LEFT_BUTTON_DOWN"])

	def OnMouseRightButtonUp(self):
		if self.eventFunc["MOUSE_RIGHT_BUTTON_UP"] :
			apply(self.eventFunc["MOUSE_RIGHT_BUTTON_UP"], self.eventArgs["MOUSE_RIGHT_BUTTON_UP"])
		if self.eventDict["MOUSE_RIGHT_BUTTON_UP"]:
			apply(self.eventDict["MOUSE_RIGHT_BUTTON_UP"], self.eventArgs["MOUSE_RIGHT_BUTTON_UP"])
			
	def OnMouseRightButtonDown(self):
		if self.eventFunc["MOUSE_RIGHT_BUTTON_DOWN"] :
			apply(self.eventFunc["MOUSE_RIGHT_BUTTON_DOWN"], self.eventArgs["MOUSE_RIGHT_BUTTON_DOWN"])
		if self.eventDict["MOUSE_RIGHT_BUTTON_DOWN"]:
			apply(self.eventDict["MOUSE_RIGHT_BUTTON_DOWN"], self.eventArgs["MOUSE_RIGHT_BUTTON_DOWN"])
			
	def OnMouseOverIn(self) :
		if self.eventFunc["MOUSE_OVER_IN"] :
			apply(self.eventFunc["MOUSE_OVER_IN"], self.eventArgs["MOUSE_OVER_IN"])
		if self.eventDict["MOUSE_OVER_IN"]:
			apply(self.eventDict["MOUSE_OVER_IN"], self.eventArgs["MOUSE_OVER_IN"])

	def OnMouseOverOut(self) :
		if self.eventFunc["MOUSE_OVER_OUT"] :
			apply(self.eventFunc["MOUSE_OVER_OUT"], self.eventArgs["MOUSE_OVER_OUT"])
			
		if self.eventDict["MOUSE_OVER_OUT"]:
			apply(self.eventDict["MOUSE_OVER_OUT"], self.eventArgs["MOUSE_OVER_OUT"])
			
	def SAFE_SetStringEvent(self, event, func, *args):
		self.eventDict[event]=__mem_func__(func)
		self.eventArgs[event]=args


class ImageBox(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.eventDict = {}
		self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
		self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterImageBox(self, layer)

	def ImageClear(self):
		wndMgr.ImageClear(self.hWnd)

	def LoadImageInstance(self, image):
		wndMgr.LoadImageInstance(self.hWnd, image)

	def LoadImage(self, imageName):
		self.name = imageName
		wndMgr.LoadImage(self.hWnd, imageName)

		if len(self.eventDict)!=0:
			print "LOAD IMAGE", self, self.eventDict

	def UnloadImage(self):
		wndMgr.UnloadImage(self.hWnd)

	# def SetDiffuseColor(self, r, g, b, a):
		# wndMgr.SetDiffuseColor(self.hWnd, r, g, b, a)
		
	if app.ENABLE_FISHING_GAME:
		def SetDiffuseColor(self, r, g, b, alpha = 1.0):
			wndMgr.SetDiffuseColor(self.hWnd, r, g, b, alpha)

	def SetClipRect(self, left, top, right, bottom, isVertical = False):
		wndMgr.SetClipRect(self.hWnd, left, top, right, bottom, isVertical)

	def SetAlpha(self, alpha):
		wndMgr.SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)

	def GetWidth(self):
		return wndMgr.GetWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetHeight(self.hWnd)

	if app.dracaryS_DUNGEON_LIB:
		def SetCoolTime(self, time):
			wndMgr.SetCoolTimeImageBox(self.hWnd, time)
		def SetStartCoolTime(self, time):
			wndMgr.SetStartCoolTimeImageBox(self.hWnd, time)

	def SAFE_SetStringEvent(self, event, func, *args):
		self.eventDict[event] =__mem_func__(func)
		self.eventArgs[event] = args
		
	def SetMouseOverInEvent(self, func, *args):
		self.eventDict["MOUSE_OVER_IN"] = func
		self.eventArgs["MOUSE_OVER_IN"] = args

	def SetMouseOverOutEvent(self, func, *args):
		self.eventDict["MOUSE_OVER_OUT"] = func
		self.eventArgs["MOUSE_OVER_OUT"] = args

	def SetEvent(self, func, *args):
		result = self.eventFunc.has_key(args[0])
		if result:
			self.eventFunc[args[0]] = func
			self.eventArgs[args[0]] = args
		else:
			print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]

	def OnMouseLeftButtonUp(self):
		if app.USE_OFFLINESHOP_FIXES:
			if self.eventFunc["mouse_click"]:
				apply(self.eventFunc["mouse_click"], self.eventArgs["mouse_click"])
				return

			if app.ENABLE_IKASHOP_RENEWAL:
				super(ImageBox, self).OnMouseLeftButtonUp()
			else:
				Window.OnMouseLeftButtonUp(self)
		else:
			if app.ENABLE_IKASHOP_RENEWAL:
				super(ImageBox, self).OnMouseLeftButtonUp()

			if self.eventFunc["mouse_click"]:
				apply(self.eventFunc["mouse_click"], self.eventArgs["mouse_click"])

			Window.OnMouseLeftButtonUp(self)

	def OnMouseOverIn(self):
		if self.eventFunc["mouse_over_in"]:
			apply(self.eventFunc["mouse_over_in"], self.eventArgs["mouse_over_in"])

		if self.eventDict.has_key("MOUSE_OVER_IN"):
			if self.eventArgs["MOUSE_OVER_IN"]:
				apply(self.eventDict["MOUSE_OVER_IN"], self.eventArgs["MOUSE_OVER_IN"])
			else:
				self.eventDict["MOUSE_OVER_IN"]()

	def OnMouseOverOut(self):
		if self.eventFunc["mouse_over_out"]:
			apply(self.eventFunc["mouse_over_out"], self.eventArgs["mouse_over_out"])

		if self.eventDict.has_key("MOUSE_OVER_OUT"):
			if self.eventArgs["MOUSE_OVER_OUT"]:
				apply(self.eventDict["MOUSE_OVER_OUT"], self.eventArgs["MOUSE_OVER_OUT"])
			else:
				self.eventDict["MOUSE_OVER_OUT"]()

	if app.ENABLE_NEW_TARGET_UI:
		def DisplayProcent(self, percent):
			wndMgr.DisplayImageProcent(self.hWnd, percent)

if app.ENABLE_MINI_GAME_CATCH_KING:
	class ImageBox2(Window):
		def __init__(self, layer = "UI"):
			Window.__init__(self, layer)

			self.eventDict = {}
			self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
			self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
			self.argDict={}

		def __del__(self):
			Window.__del__(self)

		def RegisterWindow(self, layer):
			self.hWnd = wndMgr.RegisterImageBox(self, layer)

		def LoadImage(self, imageName):
			self.name = imageName
			wndMgr.LoadImage(self.hWnd, imageName)

			if len(self.eventDict)!=0:
				print "LOAD IMAGE", self, self.eventDict

		def SetAlpha(self, alpha):
			wndMgr.SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)

		def GetWidth(self):
			return wndMgr.GetWidth(self.hWnd)

		def GetHeight(self):
			return wndMgr.GetHeight(self.hWnd)

		def OnMouseOverIn(self):
			try:
				apply(self.eventDict["mouse_over_in"], self.argDict["mouse_over_in"])
			except KeyError:
				pass

		def OnMouseOverOut(self):
			try:
				apply(self.eventDict["mouse_over_out"], self.argDict["mouse_over_out"])
			except KeyError:
				pass

		def OnMouseLeftButtonUp(self):
			try:
				apply(self.eventDict["MOUSE_LEFT_UP"], self.argDict["MOUSE_LEFT_UP"])
			except KeyError:
				pass

		def OnMouseLeftButtonDown(self):
			try:
				apply(self.eventDict["MOUSE_LEFT_DOWN"], self.argDict["MOUSE_LEFT_DOWN"])
			except KeyError:
				pass

		def SAFE_SetStringEvent(self, event, func,isa=False, *args):
			if not isa:
				self.eventDict[event]=__mem_func__(func)
				self.argDict[event]=args
			else:
				self.eventDict[event]=func

		def SAFE_SetMouseClickEvent(self, func, *args):
			self.eventDict["MOUSE_LEFT_DOWN"]=__mem_func__(func)
			self.argDict["MOUSE_LEFT_DOWN"]=args

		def SetEvent(self, func, *args) :
			result = self.eventFunc.has_key(args[0])
			if result :
				self.eventFunc[args[0]] = func
				self.eventArgs[args[0]] = args
			else :
				print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]

		def OnMouseOverIn(self) :
			if self.eventFunc["mouse_over_in"] :
				apply(self.eventFunc["mouse_over_in"], self.eventArgs["mouse_over_in"])
			else:
				try:
					self.eventDict["mouse_over_in"]()
				except KeyError:
					pass

		def OnMouseOverOut(self) :
			if self.eventFunc["mouse_over_out"] :
				apply(self.eventFunc["mouse_over_out"], self.eventArgs["mouse_over_out"])
			else :
				try:
					self.eventDict["mouse_over_out"]()
				except KeyError:
					pass

class ExpandedImageBox(ImageBox):
	def __init__(self, layer = "UI"):
		ImageBox.__init__(self, layer)
		if app.NEW_SELECT_CHARACTER:
			self.eventDict={}
			self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
			self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
			self.eventDict2={}
		self.xScaleImage = 0
		self.yScaleImage = 0

	def __del__(self):
		ImageBox.__del__(self)
		if app.NEW_SELECT_CHARACTER:
			self.eventFunc = None
			self.eventArgs = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterExpandedImageBox(self, layer)

	def SetScale(self, xScale, yScale, IsTemporary = False):
		if not IsTemporary:
			self.xScaleImage = xScale
			self.yScaleImage = yScale
	
		wndMgr.SetScale(self.hWnd, xScale, yScale)

	def SetOrigin(self, x, y):
		wndMgr.SetOrigin(self.hWnd, x, y)

	def SetRotation(self, rotation):
		wndMgr.SetRotation(self.hWnd, rotation)

	def SetRenderingMode(self, mode):
		wndMgr.SetRenderingMode(self.hWnd, mode)

	# [0.0, 1.0] ������ ����ŭ �ۼ�Ʈ�� �׸��� �ʴ´�.
	def SetRenderingRect(self, left, top, right, bottom):
		wndMgr.SetRenderingRect(self.hWnd, left, top, right, bottom)


	def SetPercentage(self, curValue, maxValue):
		if maxValue:
			self.SetRenderingRect(0.0, 0.0, -1.0 + float(curValue) / float(maxValue), 0.0)
		else:
			self.SetRenderingRect(0.0, 0.0, 0.0, 0.0)

	def GetWidth(self):
		return wndMgr.GetWindowWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetWindowHeight(self.hWnd)

	if app.NEW_SELECT_CHARACTER:
		def SetEvent(self, func, *args) :
			result = self.eventFunc.has_key(args[0])
			if result :
				self.eventFunc[args[0]] = func
				self.eventArgs[args[0]] = args
			else :
				print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]

if app.ENABLE_NEW_TARGET_UI:
	class ThinBoardTarget(Window):
		CORNER_WIDTH = 16
		CORNER_HEIGHT = 16
		LINE_WIDTH = 16
		LINE_HEIGHT = 16
		BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.45)
		LT = 0
		LB = 1
		RT = 2
		RB = 3
		L = 0
		R = 1
		T = 2
		B = 3
		def __init__(self, layer = "UI"):
			Window.__init__(self, layer)
			CornerFileNames = [ "d:/ymir work/ui/pattern/thinboardnew/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
			LineFileNames = [ "d:/ymir work/ui/pattern/thinboardnew/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]
			self.Corners = []
			for fileName in CornerFileNames:
				Corner = ExpandedImageBox()
				Corner.AddFlag("attach")
				Corner.AddFlag("not_pick")
				Corner.LoadImage(fileName)
				Corner.SetParent(self)
				Corner.SetPosition(0, 0)
				Corner.Show()
				self.Corners.append(Corner)
			self.Lines = []
			for fileName in LineFileNames:
				Line = ExpandedImageBox()
				Line.AddFlag("attach")
				Line.AddFlag("not_pick")
				Line.LoadImage(fileName)
				Line.SetParent(self)
				Line.SetPosition(0, 0)
				Line.Show()
				self.Lines.append(Line)
			Base = Bar()
			Base.SetParent(self)
			Base.AddFlag("attach")
			Base.AddFlag("not_pick")
			Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
			Base.SetColor(self.BOARD_COLOR)
			Base.Show()
			self.Base = Base

			self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
			self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

		def __del__(self):
			Window.__del__(self)

		def ShowCorner(self, corner):
			self.Corners[corner].Show()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def HideCorners(self, corner):
			self.Corners[corner].Hide()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def ShowLine(self, line):
			self.Lines[line].Show()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def HideLine(self, line):
			self.Lines[line].Hide()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def SetSize(self, width, height):

			width = max(self.CORNER_WIDTH*2, width)
			height = max(self.CORNER_HEIGHT*2, height)
			Window.SetSize(self, width, height)

			if self.Corners[self.LT].IsShow():
				self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
				self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)
			else:
				self.Lines[self.L].SetPosition(0, 0)
				self.Lines[self.T].SetPosition(0, 0)

			self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
			self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
			self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)

			if self.Corners[self.RT].IsShow():
				self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
			else:
				self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, 0)
			if self.Corners[self.LB].IsShow():
				self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)
			else:
				self.Lines[self.B].SetPosition(0, height - self.CORNER_HEIGHT)

			cornerCount = {}
			cornerCount[self.L] = self.Corners[self.LB].IsShow() + self.Corners[self.LT].IsShow()
			cornerCount[self.R] = self.Corners[self.RB].IsShow() + self.Corners[self.RT].IsShow()
			cornerCount[self.T] = self.Corners[self.LT].IsShow() + self.Corners[self.RT].IsShow()
			cornerCount[self.B] = self.Corners[self.LB].IsShow() + self.Corners[self.RB].IsShow()

			self.Lines[self.L].SetRenderingRect(0, 0, 0, float(height - self.LINE_HEIGHT - self.CORNER_HEIGHT * cornerCount[self.L]) / self.LINE_HEIGHT)
			self.Lines[self.R].SetRenderingRect(0, 0, 0, float(height - self.LINE_HEIGHT - self.CORNER_HEIGHT * cornerCount[self.R]) / self.LINE_HEIGHT)
			self.Lines[self.T].SetRenderingRect(0, 0, float(width - self.LINE_WIDTH - self.CORNER_WIDTH * cornerCount[self.T]) / self.LINE_WIDTH, 0)
			self.Lines[self.B].SetRenderingRect(0, 0, float(width - self.LINE_WIDTH - self.CORNER_WIDTH * cornerCount[self.B]) / self.LINE_WIDTH, 0)

			lineCount = []
			lineCount.append(self.Lines[self.T].IsShow() + self.Lines[self.B].IsShow())
			lineCount.append(self.Lines[self.L].IsShow() + self.Lines[self.R].IsShow())

			if self.Lines[self.L].IsShow():
				self.Base.SetPosition(self.CORNER_WIDTH, self.Base.GetTop())
			else:
				self.Base.SetPosition(0, self.Base.GetTop())
			if self.Lines[self.T].IsShow():
				self.Base.SetPosition(self.Base.GetLeft(), self.CORNER_HEIGHT)
			else:
				self.Base.SetPosition(self.Base.GetLeft(), 0)

			self.Base.SetSize(width - self.CORNER_WIDTH*lineCount[1], height - self.CORNER_HEIGHT*lineCount[0])

		def ShowInternal(self):
			self.Base.Show()
			for wnd in self.Lines:
				wnd.Show()
			for wnd in self.Corners:
				wnd.Show()

		def HideInternal(self):
			self.Base.Hide()
			for wnd in self.Lines:
				wnd.Hide()
			for wnd in self.Corners:
				wnd.Hide()

class AniImageBox(Window):
	if app.ENABLE_FISH_GAME:
		def SetRotation(self, rotation):
			wndMgr.SetRotation(self.hWnd, rotation)
		def GetRotation(self):
			return wndMgr.GetRotation(self.hWnd)
		def MoveStart(self):
			wndMgr.MoveStart(self.hWnd)
		def MoveStop(self):
			wndMgr.MoveStop(self.hWnd)
		def SetMovePos(self, x, y):
			wndMgr.SetMovePos(self.hWnd, x, y)
		def SetMoveSpeed(self, speed):
			wndMgr.SetMoveSpeed(self.hWnd, speed)
		def OnMoveDone(self):
			if self.onMoveDoneFunc:
				self.onMoveDoneFunc()

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.end_frame_event = None
		self.key_frame_event = None
		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.endFrameArgs = None
			self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
			self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}

	def __del__(self):
		Window.__del__(self)
		self.end_frame_event = None
		self.key_frame_event = None
		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.endFrameArgs = None
			self.eventFunc = None
			self.eventArgs = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterAniImageBox(self, layer)

	def SetDelay(self, delay):
		wndMgr.SetDelay(self.hWnd, delay)
		
	def SetDiffuseColor(self, r, g, b, alpha = 1.0):
		wndMgr.SetDiffuseColor(self.hWnd, r, g, b, alpha)

	def AppendImage(self, filename):
		wndMgr.AppendImage(self.hWnd, filename)

	def SetPercentage(self, curValue, maxValue):
		wndMgr.SetRenderingRect(self.hWnd, 0.0, 0.0, -1.0 + float(curValue) / float(maxValue), 0.0)

	def ResetFrame(self):
		wndMgr.ResetFrame(self.hWnd)
		
	if app.ENABLE_FISHING_GAME:
		def SetRotation(self, rotation):
			wndMgr.SetRotation(self.hWnd, rotation)

	if app.ENABLE_MINI_GAME_CATCH_KING:
		def SetOnEndFrame(self, event):
			self.end_frame_event = event

		def SetEndFrameEvent(self, event, *args):
			self.end_frame_event = event
			self.endFrameArgs = args

		def OnEndFrame(self):
			if self.endFrameArgs == None:
				if self.end_frame_event:
					apply(self.end_frame_event)
			else:
				if self.end_frame_event:
					apply(self.end_frame_event, self.endFrameArgs)

		def SetScale(self, xScale, yScale):
			wndMgr.SetSlotScale(self.hWnd, xScale, yScale)

		def SetEvent(self, func, *args) :
			result = self.eventFunc.has_key(args[0])
			if result:
				self.eventFunc[args[0]] = func
				self.eventArgs[args[0]] = args
			else :
				print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]

		def OnKeyFrame(self, cur_frame):
			if self.key_frame_event:
				self.key_frame_event(cur_frame)

		def SetKeyFrameEvent(self, event):
			self.key_frame_event = event

		def OnMouseOverIn(self) :
			if self.eventFunc["mouse_over_in"] :
				apply(self.eventFunc["mouse_over_in"], self.eventArgs["mouse_over_in"])

		def OnMouseOverOut(self) :
			if self.eventFunc["mouse_over_out"] :
				apply(self.eventFunc["mouse_over_out"], self.eventArgs["mouse_over_out"])
	else:
		def SetOnEndFrame(self, event):
			self.end_frame_event = event

		def OnEndFrame(self):
			if self.end_frame_event:
				self.end_frame_event()

if app.ENABLE_MINI_GAME_CATCH_KING:
	class AniImageBox2(Window):
		def __init__(self, layer = "UI"):
			Window.__init__(self, layer)
			self.endFrameEvent = None
			self.endFrameArgs = None
			self.keyFrameEvent = None

		def __del__(self):
			Window.__del__(self)
			self.endFrameEvent = None
			self.endFrameArgs = None
			self.keyFrameEvent = None

		def RegisterWindow(self, layer):
			self.hWnd = wndMgr.RegisterAniImageBox(self, layer)

		def SetDelay(self, delay):
			wndMgr.SetDelay(self.hWnd, delay)

		def AppendImage(self, filename):
			wndMgr.AppendImage(self.hWnd, filename)

		def AppendImageScale(self, filename, scale_x, scale_y):
			wndMgr.AppendImageScale(self.hWnd, filename, scale_x, scale_y)

		def ResetFrame(self):
			wndMgr.ResetFrame(self.hWnd)

		def SetEndFrameEvent(self, event, *args):
			self.endFrameEvent = event
			self.endFrameArgs = args

		def SetScale(self, xScale, yScale):
			wndMgr.SetSlotScale(self.hWnd, xScale, yScale)

		def SetPercentage(self, curValue, maxValue):
			wndMgr.SetRenderingRect(self.hWnd, 0.0, 0.0, -1.0 + float(curValue) / float(maxValue), 0.0)

		def SetOnEndFrame(self, event):
			self.endFrameEvent = event

		def SetKeyFrameEvent(self, event):
			self.keyFrameEvent = event

		def OnKeyFrame(self, curFrame):
			if self.keyFrameEvent:
				self.keyFrameEvent(curFrame)

		def OnEndFrame(self):
			if self.endFrameEvent:
				apply(self.endFrameEvent, self.endFrameArgs)

class CheckBox(Window):
	def __init__(self):
		Window.__init__(self)
		
		self.backGroundImage = None
		self.checkImage = None

		self.eventFunc = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
		self.eventArgs = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
	
		self.CreateElements()
		
	def __del__(self):
		Window.__del__(self)
		
		self.backGroundImage = None
		self.checkImage = None
		
		self.eventFunc = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
		self.eventArgs = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
		
	def CreateElements(self):
		self.backGroundImage = ImageBox()
		self.backGroundImage.SetParent(self)
		self.backGroundImage.AddFlag("not_pick")
		self.backGroundImage.LoadImage("d:/ymir work/ui/game/refine/checkbox.tga")
		self.backGroundImage.Show()
		
		self.checkImage = ImageBox()
		self.checkImage.SetParent(self)
		self.checkImage.AddFlag("not_pick")
		self.checkImage.SetPosition(0, -4)
		self.checkImage.LoadImage("d:/ymir work/ui/game/refine/checked.tga")
		self.checkImage.Hide()
		
		self.textInfo = TextLine()
		self.textInfo.SetParent(self)
		self.textInfo.SetPosition(20, -2)
		self.textInfo.Show()
		
		self.SetSize(self.backGroundImage.GetWidth() + self.textInfo.GetTextSize()[0], self.backGroundImage.GetHeight() + self.textInfo.GetTextSize()[1])
		
	def SetTextInfo(self, info):
		if self.textInfo:
			self.textInfo.SetText(info)
			
		self.SetSize(self.backGroundImage.GetWidth() + self.textInfo.GetTextSize()[0], self.backGroundImage.GetHeight() + self.textInfo.GetTextSize()[1])
		
	def SetCheckStatus(self, flag):
		if flag:
			self.checkImage.Show()
		else:
			self.checkImage.Hide()
	
	def GetCheckStatus(self):
		if self.checkImage:
			return self.checkImage.IsShow()
			
		return False
		
	def SetEvent(self, func, *args) :
		result = self.eventFunc.has_key(args[0])		
		if result :
			self.eventFunc[args[0]] = func
			self.eventArgs[args[0]] = args
		else :
			print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]
		
	def OnMouseLeftButtonUp(self):
		if self.checkImage:
			if self.checkImage.IsShow():
				self.checkImage.Hide()

				if self.eventFunc["ON_UNCKECK"]:
					apply(self.eventFunc["ON_UNCKECK"], self.eventArgs["ON_UNCKECK"])
			else:
				self.checkImage.Show()

				if self.eventFunc["ON_CHECK"]:
					apply(self.eventFunc["ON_CHECK"], self.eventArgs["ON_CHECK"])

class Button(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.eventFunc = None
		self.eventArgs = None

		self.ButtonText = None
		self.ToolTipText = None
		self.showtooltipevent = None
		self.showtooltiparg = None
		self.hidetooltipevent = None
		self.hidetooltiparg = None
		if app.ENABLE_NEW_PET_SYSTEM:
			self.fl = None
		
		if app.ENABLE_MINI_GAME_CATCH_KING:
			self.overOutFunc = None
			self.overOutArgs = None
			self.overFunc = None
			self.overArgs = None

		self.eventOverInFunc = None
		self.eventOverInArgs = None

		self.eventOverOutFunc = None
		self.eventOverOutArgs = None

	def __del__(self):
		Window.__del__(self)

		self.eventFunc = None
		self.eventArgs = None

	if app.ENABLE_NEW_GAMEOPTION:
		def SetListText(self, text, x = 8):
			if not self.ButtonText:
				textLine = TextLine()
				textLine.SetParent(self)
				textLine.SetPosition(x, self.GetHeight()/2)
				textLine.SetVerticalAlignCenter()
				textLine.SetHorizontalAlignLeft()
				textLine.Show()
				self.ButtonText = textLine
			self.ButtonText.SetText(text)
	
	if app.ENABLE_NEW_GAMEOPTION:
		def SetRenderingRect(self, left, top, right, bottom):
			wndMgr.SetRenderingRect(self.hWnd, left, top, right, bottom)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterButton(self, layer)

	def SetUpVisual(self, filename):
		wndMgr.SetUpVisual(self.hWnd, filename)

	def SetOverVisual(self, filename):
		wndMgr.SetOverVisual(self.hWnd, filename)

	def SetDownVisual(self, filename):
		wndMgr.SetDownVisual(self.hWnd, filename)

	def SetDisableVisual(self, filename):
		wndMgr.SetDisableVisual(self.hWnd, filename)

	def GetUpVisualFileName(self):
		return wndMgr.GetUpVisualFileName(self.hWnd)

	def GetOverVisualFileName(self):
		return wndMgr.GetOverVisualFileName(self.hWnd)

	def GetDownVisualFileName(self):
		return wndMgr.GetDownVisualFileName(self.hWnd)

	def SetEventOverIn(self, func, *args):
		self.eventOverInFunc = func
		self.eventOverInArgs = args

	def SetEventOverOut(self, func, *args):
		self.eventOverOutFunc = func
		self.eventOverOutArgs = args

	if app.ENABLE_NEW_PET_SYSTEM:
		def Flash(self, state = ""):
			if state == "":
				if self.fl == None:
					self.fl = True
				else:
					self.fl = not self.fl
			else:
				self.fl = state
			wndMgr.Flash(self.hWnd, self.fl)
	else:
		def Flash(self):
			wndMgr.Flash(self.hWnd)

	def Enable(self):
		wndMgr.Enable(self.hWnd)

	def Disable(self):
		wndMgr.Disable(self.hWnd)

	def Down(self):
		wndMgr.Down(self.hWnd)

	def FlashEx(self):
		wndMgr.FlashEx(self.hWnd)
		
	def SetUp(self):
		wndMgr.SetUp(self.hWnd)

	def SAFE_SetEvent(self, func, *args):
		self.eventFunc = __mem_func__(func)
		self.eventArgs = args

	def SetEvent(self, func, *args):
		self.eventFunc = func
		self.eventArgs = args
	
	def SetShowToolTipEvent(self, func, *args):
		self.showtooltipevent = func
		self.showtooltiparg = args

	def SetHideToolTipEvent(self, func, *args):
		self.hidetooltipevent = func
		self.hidetooltiparg = args

	def SetTextAddPos(self, text, x_add = 0, y_add = 0):
		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth() / 2 + x_add, self.GetHeight() / 2 + y_add)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine
		self.ButtonText.SetText(text)

	if app.ENABLE_RANKING:
		def SetTextPosition(self, x, y):
			if self.ButtonText:
				self.ButtonText.SetHorizontalAlignLeft()
				self.ButtonText.SetPosition(x, y)

	def SetTextColor(self, color):
		if not self.ButtonText:
			return
		self.ButtonText.SetPackedFontColor(color)

	def DelText(self):
		if self.ButtonText:
			self.ButtonText.Hide()
			self.ButtonText = None

	def SetText(self, text, height = 4):

		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

	if constInfo.EXTRA_UI_FEATURE:
		def GetText(self):
			if not self.ButtonText:
				return ""
			return self.ButtonText.GetText()

	def SetFormToolTipText(self, type, text, x, y):
		if not self.ToolTipText:
			toolTip=createToolTipWindowDict[type]()
			if app.__DUNGEON_INFO__:
				toolTip.SetWindowName("not_render_tooltip")
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(text)

	def SetToolTipWindow(self, toolTip):
		self.ToolTipText=toolTip
		self.ToolTipText.SetParentProxy(self)

	def SetToolTipText(self, text, x=0, y = -19):
		self.SetFormToolTipText("TEXT", text, x, y)

	def CallEvent(self):
		snd.PlaySound("sound/ui/click.wav")

		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)

	def ShowToolTip(self):
		if self.eventOverInFunc: # renewal overin
			apply(self.eventOverInFunc, self.eventOverInArgs)		
		else:
			if self.ToolTipText:
				self.ToolTipText.Show()
		
		if self.showtooltipevent:
			apply(self.showtooltipevent, self.showtooltiparg)
			
		

	def HideToolTip(self):
		if self.eventOverOutFunc: # renewal overout
			apply(self.eventOverOutFunc, self.eventOverOutArgs)
	
		if self.ToolTipText:
			self.ToolTipText.Hide()
		
		if self.hidetooltipevent:
			apply(self.hidetooltipevent, self.hidetooltiparg)

	def IsDown(self):
		return wndMgr.IsDown(self.hWnd)

	if app.ENABLE_MINI_GAME_CATCH_KING:
		def OnMouseOverIn(self):
			if self.overFunc:
				apply(self.overFunc, self.overArgs)

		def OnMouseOverOut(self):
			if self.overOutFunc:
				apply(self.overOutFunc, self.overOutArgs)

		def SetOverEvent(self, func, *args):
			self.overFunc = func
			self.overArgs = args

		def SetOverOutEvent(self, func, *args):
			self.overOutFunc = func
			self.overOutArgs = args

		def GetButtonImageWidth(self):
			return wndMgr.GetButtonImageWidth(self.hWnd)

		def GetButtonImageHeight(self):
			return wndMgr.GetButtonImageHeight(self.hWnd)

	if app.USE_CHEQUE_CURRENCY or app.ENABLE_MINI_GAME_CATCH_KING:
		def SetButtonScale(self, x, y):
			wndMgr.SetButtonScale(self.hWnd, x, y)

class RadioButton(Button):
	def __init__(self):
		Button.__init__(self)
		if app.NEW_SELECT_CHARACTER:
			self.TextChild = []

	def __del__(self):
		Button.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterRadioButton(self, layer)

	if app.NEW_SELECT_CHARACTER:
		def Over(self):
			wndMgr.Over(self.hWnd)

		def AppendTextLineAllClear(self):
			self.TextChild = []

		def SetAppendTextChangeText(self, idx, text):
			if not len(self.TextChild) :
				return

			self.TextChild[idx].SetText(text)

		def SetAppendTextColor(self, idx, color) :
			if not len(self.TextChild) :
				return

			self.TextChild[idx].SetPackedFontColor(color)

		def AppendTextLine(self, text
			, font_size = localeInfo.UI_DEF_FONT
			, font_color = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
			, text_sort = "center"
			, pos_x = None
			, pos_y = None) :
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetFontName(font_size)
			textLine.SetPackedFontColor(font_color)
			textLine.SetText(text)
			textLine.Show()

			if not pos_x and not pos_y :
				textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			else :
				textLine.SetPosition(pos_x, pos_y)

			textLine.SetVerticalAlignCenter()
			if "center" == text_sort :
				textLine.SetHorizontalAlignCenter()
			elif "right" == text_sort :
				textLine.SetHorizontalAlignRight()
			elif "left" == text_sort :
				textLine.SetHorizontalAlignLeft()

			self.TextChild.append(textLine)

class RadioButtonGroup:
	def __init__(self):
		self.buttonGroup = []
		self.selectedBtnIdx = -1

	def __del__(self):
		for button, ue, de in self.buttonGroup:
			button.__del__()

	def Show(self):
		for (button, selectEvent, unselectEvent) in self.buttonGroup:
			button.Show()

	def Hide(self):
		for (button, selectEvent, unselectEvent) in self.buttonGroup:
			button.Hide()

	def SetText(self, idx, text):
		if idx >= len(self.buttonGroup):
			return
		(button, selectEvent, unselectEvent) = self.buttonGroup[idx]
		button.SetText(text)

	def OnClick(self, btnIdx):
		if btnIdx == self.selectedBtnIdx:
			return
		(button, selectEvent, unselectEvent) = self.buttonGroup[self.selectedBtnIdx]
		if unselectEvent:
			unselectEvent()
		button.SetUp()

		self.selectedBtnIdx = btnIdx
		(button, selectEvent, unselectEvent) = self.buttonGroup[btnIdx]
		if selectEvent:
			selectEvent()

		button.Down()

	def AddButton(self, button, selectEvent, unselectEvent):
		i = len(self.buttonGroup)
		button.SetEvent(lambda : self.OnClick(i))
		self.buttonGroup.append([button, selectEvent, unselectEvent])
		button.SetUp()

	def Create(rawButtonGroup):
		radioGroup = RadioButtonGroup()
		for (button, selectEvent, unselectEvent) in rawButtonGroup:
			radioGroup.AddButton(button, selectEvent, unselectEvent)

		radioGroup.OnClick(0)

		return radioGroup

	Create=staticmethod(Create)

class ToggleButton(Button):
	def __init__(self):
		Button.__init__(self)

		self.eventUp = None
		self.eventDown = None

		self.eventUpArgs = None
		self.eventDownArgs = None

	def __del__(self):
		Button.__del__(self)

		self.eventUp = None
		self.eventDown = None

	def SetToggleUpEvent(self, event, *args):
		self.eventUp = event
		self.eventUpArgs = args

	def SetToggleDownEvent(self, event, *args):
		self.eventDown = event
		self.eventDownArgs = args

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterToggleButton(self, layer)

	def OnToggleUp(self):
		if self.eventUp:
			if self.eventUpArgs:
				apply(self.eventUp, self.eventUpArgs)
			else:
				self.eventUp()

	def OnToggleDown(self):
		if self.eventDown:
			if self.eventDownArgs:
				apply(self.eventDown, self.eventDownArgs)
			else:
				self.eventDown()

class DragButton(Button):
	def __init__(self):
		Button.__init__(self)
		self.AddFlag("movable")
		self.AddFlag("animate")

		self.callbackEnable = True
		self.eventMove = lambda: None

	def __del__(self):
		Button.__del__(self)

		self.eventMove = lambda: None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterDragButton(self, layer)

	def SetMoveEvent(self, event):
		self.eventMove = event

	def SetRestrictMovementArea(self, x, y, width, height):
		wndMgr.SetRestrictMovementArea(self.hWnd, x, y, width, height)

	def TurnOnCallBack(self):
		self.callbackEnable = True

	def TurnOffCallBack(self):
		self.callbackEnable = False

	def OnMove(self):
		if self.callbackEnable:
			self.eventMove()

class NumberLine(Window):

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterNumberLine(self, layer)

	def SetHorizontalAlignCenter(self):
		wndMgr.SetNumberHorizontalAlignCenter(self.hWnd)

	def SetHorizontalAlignRight(self):
		wndMgr.SetNumberHorizontalAlignRight(self.hWnd)

	def SetPath(self, path):
		wndMgr.SetPath(self.hWnd, path)

	def SetNumber(self, number):
		wndMgr.SetNumber(self.hWnd, number)
class ResizableTextValue(Window):

	BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
	LINE_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
	
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		
		self.isBackground = TRUE
		self.LineText = None
		self.ToolTipText = None
		
		self.width = 0
		self.height = 0
		self.lines = []
		
	def __del__(self):
		Window.__del__(self)
		
	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		
	def SetToolTipText(self, tooltiptext, x = 0, y = 0):
		if not self.ToolTipText:		
			toolTip=createToolTipWindowDict["TEXT"]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y-20)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(tooltiptext)
		
	def SetText(self, text):
		if not self.LineText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, (self.GetHeight()/2)-1)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetOutline()
			textLine.Show()
			self.LineText = textLine

		self.LineText.SetText(text)
		
	def SetTextColor(self, color):
		if not self.LineText:
			return
		self.LineText.SetPackedFontColor(color)
		
	def GetText(self):
		if not self.LineText:
			return
		return self.LineText.GetText()
		
	def SetLineColor(self, color):
		self.LINE_COLOR = color
		
	def SetLine(self, line_value):
		self.lines.append(line_value)
		
	def SetBackgroundColor(self, color):
		self.BACKGROUND_COLOR = color
		
	def SetNoBackground(self):
		self.isBackground = FALSE
	
	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()
		
		widthRender = self.width
		heightRender = self.height
		if self.isBackground:
			grp.SetColor(self.BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(self.LINE_COLOR)
		if 'top' in self.lines:
			grp.RenderLine(xRender, yRender, widthRender, 0)
		if 'left' in self.lines:
			grp.RenderLine(xRender, yRender, 0, heightRender)
		if 'bottom' in self.lines:
			grp.RenderLine(xRender, yRender+heightRender, widthRender+1, 0)
		if 'right' in self.lines:	
			grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

class CoolButton(Window):
	
	BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
	DARK_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
	
	WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
	HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)
	
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.eventFunc = None
		self.eventArgs = None

		self.ButtonText = None
		self.ToolTipText = None
		
		self.EdgeColor = None
		self.isOver = FALSE
		self.isSelected = FALSE
		
		self.width = 0
		self.height = 0		

	def __del__(self):
		Window.__del__(self)

		self.eventFunc = None
		self.eventArgs = None

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		
	def SetEvent(self, func, *args):
		self.eventFunc = func
		self.eventArgs = args

	def SetTextColor(self, color):
		if not self.ButtonText:
			return
		self.ButtonText.SetPackedFontColor(color)
		
	def SetEdgeColor(self, color):
		self.EdgeColor = color

	def SetText(self, text):
		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetOutline()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

	def SetToolTipText(self, text, x=0, y = -19):
		if not self.ToolTipText:		
			toolTip=createToolTipWindowDict["TEXT"]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(text)

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()
			
	def SetTextPosition(self, width):
		self.ButtonText.SetPosition(width, self.GetHeight()/2)
		self.ButtonText.SetHorizontalAlignLeft()
		
	def Enable(self):
		wndMgr.Enable(self.hWnd)

	def Disable(self):
		wndMgr.Disable(self.hWnd)
		
	def OnMouseLeftButtonDown(self):
		self.isSelected = TRUE
		
	def OnMouseLeftButtonUp(self):
		self.isSelected = FALSE
		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)

	def OnUpdate(self):
		if self.IsIn():
			self.isOver = TRUE
			self.ShowToolTip()
		else:
			self.isOver = FALSE
			self.HideToolTip()

	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()
		
		widthRender = self.width
		heightRender = self.height
		grp.SetColor(self.BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		if self.EdgeColor:
			grp.SetColor(self.EdgeColor)
		else:
			grp.SetColor(self.DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(self.HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

			if self.isSelected:
				grp.SetColor(self.WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)
			
class ResizableButtonWithImage(Window):
	
	BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
	DARK_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
	
	WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
	HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)
	
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.eventFunc = None
		self.eventArgs = None

		self.ButtonText = None
		self.ToolTipText = None
		self.ButtonImage = None
		
		self.isOver = FALSE
		self.isSelected = FALSE
		
		self.width = 0
		self.height = 0		

	def __del__(self):
		Window.__del__(self)

		self.eventFunc = None
		self.eventArgs = None

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		
	def SetEvent(self, func, *args):
		self.eventFunc = func
		self.eventArgs = args

	def SetTextColor(self, color):
		if not self.ButtonText:
			return
		self.ButtonText.SetPackedFontColor(color)

	def SetText(self, text):
		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(12, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetWindowHorizontalAlignCenter()
			textLine.SetOutline()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)
		self.ButtonText.SetHorizontalAlignCenter()
		self.ButtonText.SetWindowHorizontalAlignCenter()

	def SetToolTipText(self, text, x=0, y = -19):
		if not self.ToolTipText:		
			toolTip=createToolTipWindowDict["TEXT"]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(text)
		
	def SetImage(self, img):
		if not self.ButtonImage:
			image = ExpandedImageBox()
			image.SetParent(self)
			image.SetPosition(6, self.GetHeight()/2)
			image.Show()
			self.ButtonImage = image
		self.ButtonImage.LoadImage(img)
		self.ButtonImage.SetPosition(6, ((self.GetHeight() - self.ButtonImage.GetHeight())/2)+1)
		
	def SetTextPosition(self, x, y = 0, align = FALSE):
		if y == 0:
			self.ButtonText.SetPosition(x, self.GetHeight()/2)
		else:
			self.ButtonText.SetPosition(x, y)
		if align:
			self.ButtonText.SetWindowHorizontalAlignLeft()
			self.ButtonText.SetHorizontalAlignLeft()

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()
		
	def OnMouseLeftButtonDown(self):
		self.isSelected = TRUE
		
	def OnMouseLeftButtonUp(self):
		self.isSelected = FALSE
		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)

	def OnUpdate(self):
		if self.IsIn():
			self.isOver = TRUE
			self.ShowToolTip()
		else:
			self.isOver = FALSE
			self.HideToolTip()

	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()
		
		widthRender = self.width
		heightRender = self.height
		grp.SetColor(self.BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(self.DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(self.HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

			if self.isSelected:
				grp.SetColor(self.WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 2, self.width - 3, heightRender - 3)

###################################################################################################
## PythonScript Element
###################################################################################################

class Box(Window):

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBox(self, layer)

	def SetColor(self, color):
		wndMgr.SetColor(self.hWnd, color)

class Bar(Window):

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBar(self, layer)

	def SetColor(self, color):
		wndMgr.SetColor(self.hWnd, color)

class Line(Window):

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterLine(self, layer)

	def SetColor(self, color):
		wndMgr.SetColor(self.hWnd, color)

class SlotBar(Window):

	def __init__(self):
		Window.__init__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBar3D(self, layer)

## Same with SlotBar
class Bar3D(Window):

	def __init__(self):
		Window.__init__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBar3D(self, layer)

	def SetColor(self, left, right, center):
		wndMgr.SetColor(self.hWnd, left, right, center)

class SlotWindow(Window):

	def __init__(self):
		Window.__init__(self)

		self.StartIndex = 0

		self.eventSelectEmptySlot = None
		self.eventSelectItemSlot = None
		self.eventUnselectEmptySlot = None
		self.eventUnselectItemSlot = None
		self.eventUseSlot = None
		self.eventOverInItem = None
		self.eventOverOutItem = None
		self.eventPressedSlotButton = None
		self.eventOverInItem2 = None
		self.eventOverInItem3 = None
		self.eventOnOverInEmptySlot = None
		self.eventOnOverOutEmptySlot = None

		self.eventOverIn = None
		self.eventOverOut = None
		
	def __del__(self):
		Window.__del__(self)

		self.eventSelectEmptySlot = None
		self.eventSelectItemSlot = None
		self.eventUnselectEmptySlot = None
		self.eventUnselectItemSlot = None
		self.eventUseSlot = None
		self.eventOverInItem = None
		self.eventOverOutItem = None
		self.eventPressedSlotButton = None
		self.eventOverInItem2 = None
		self.eventOverInItem3 = None
		self.eventOnOverInEmptySlot = None
		self.eventOnOverOutEmptySlot = None

		self.eventOverIn = None
		self.eventOverOut = None
		
	def SetOverInEvent2(self, event):
		self.eventOverIn = event

	def SetOverOutEvent2(self, event):
		self.eventOverOut = event

	def OnOverIn(self, slotNumber):
		if self.eventOverIn:
			self.eventOverIn(slotNumber)

	def OnOverOut(self):
		if self.eventOverOut:
			self.eventOverOut()
		
	def SetOverInItemEvent2(self, event):
		self.eventOverInItem2 = event
		
	def SetOverInItemEvent3(self, event):
		self.eventOverInItem3 = event
		
	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterSlotWindow(self, layer)

	def SetSlotStyle(self, style):
		wndMgr.SetSlotStyle(self.hWnd, style)


	def HasSlot(self, slotIndex):
		return wndMgr.HasSlot(self.hWnd, slotIndex)

	def SetSlotBaseImage(self, imageFileName, r, g, b, a):
		wndMgr.SetSlotBaseImage(self.hWnd, imageFileName, r, g, b, a)
	def SetSlotBaseImageScale(self, imageFileName, r, g, b, a, sx, sy):
		wndMgr.SetSlotBaseImageScale(self.hWnd, imageFileName, r, g, b, a, sx, sy)
	def SetCoverButton(self,\
						slotIndex,\
						upName="d:/ymir work/ui/public/slot_cover_button_01.sub",\
						overName="d:/ymir work/ui/public/slot_cover_button_02.sub",\
						downName="d:/ymir work/ui/public/slot_cover_button_03.sub",\
						disableName="d:/ymir work/ui/public/slot_cover_button_04.sub",\
						LeftButtonEnable = False,\
						RightButtonEnable = True):
		wndMgr.SetCoverButton(self.hWnd, slotIndex, upName, overName, downName, disableName, LeftButtonEnable, RightButtonEnable)

	def EnableCoverButton(self, slotIndex):
		wndMgr.EnableCoverButton(self.hWnd, slotIndex)

	def DisableCoverButton(self, slotIndex):
		wndMgr.DisableCoverButton(self.hWnd, slotIndex)

	def SetAlwaysRenderCoverButton(self, slotIndex, bAlwaysRender = True):
		wndMgr.SetAlwaysRenderCoverButton(self.hWnd, slotIndex, bAlwaysRender)
	def GetItemIndex(self, slotNumber):
		return wndMgr.GetItemIndex(self.hWnd, slotNumber)

	def AppendSlotButton(self, upName, overName, downName):
		wndMgr.AppendSlotButton(self.hWnd, upName, overName, downName)

	def ShowSlotButton(self, slotNumber):
		wndMgr.ShowSlotButton(self.hWnd, slotNumber)

	def HideAllSlotButton(self):
		wndMgr.HideAllSlotButton(self.hWnd)

	def AppendRequirementSignImage(self, filename):
		wndMgr.AppendRequirementSignImage(self.hWnd, filename)

	def ShowRequirementSign(self, slotNumber):
		wndMgr.ShowRequirementSign(self.hWnd, slotNumber)

	def HideRequirementSign(self, slotNumber):
		wndMgr.HideRequirementSign(self.hWnd, slotNumber)

	if app.ENABLE_SASH_SYSTEM:
		def ActivateSlot(self, slotNumber, r = 1.0, g = 1.0, b = 1.0, a = 1.0):
			wndMgr.ActivateEffect(self.hWnd, slotNumber, r, g, b, a)
			# wndMgr.SetSlotFlashEffect(self.hWnd, slotNumber, True)

		def DeactivateSlot(self, slotNumber):
			wndMgr.DeactivateEffect(self.hWnd, slotNumber)

		def ActivateSlotOld(self, slotNumber):
			wndMgr.ActivateSlot(self.hWnd, slotNumber)
			wndMgr.SetSlotFlashEffect(self.hWnd, slotNumber, True)

		def DeactivateSlotOld(self, slotNumber):
			wndMgr.DeactivateSlot(self.hWnd, slotNumber)
	else:
		def ActivateSlot(self, slotNumber):
			wndMgr.ActivateSlot(self.hWnd, slotNumber)

		def DeactivateSlot(self, slotNumber):
			wndMgr.DeactivateSlot(self.hWnd, slotNumber)

	def ShowSlotBaseImage(self, slotNumber):
		wndMgr.ShowSlotBaseImage(self.hWnd, slotNumber)

	def HideSlotBaseImage(self, slotNumber):
		wndMgr.HideSlotBaseImage(self.hWnd, slotNumber)

	def SAFE_SetButtonEvent(self, button, state, event):
		if "LEFT"==button:
			if "EMPTY"==state:
				self.eventSelectEmptySlot=__mem_func__(event)
			elif "EXIST"==state:
				self.eventSelectItemSlot=__mem_func__(event)
			elif "ALWAYS"==state:
				self.eventSelectEmptySlot=__mem_func__(event)
				self.eventSelectItemSlot=__mem_func__(event)
		elif "RIGHT"==button:
			if "EMPTY"==state:
				self.eventUnselectEmptySlot=__mem_func__(event)
			elif "EXIST"==state:
				self.eventUnselectItemSlot=__mem_func__(event)
			elif "ALWAYS"==state:
				self.eventUnselectEmptySlot=__mem_func__(event)
				self.eventUnselectItemSlot=__mem_func__(event)

	def SetSelectEmptySlotEvent(self, empty):
		self.eventSelectEmptySlot = empty

	def SetSelectItemSlotEvent(self, item):
		self.eventSelectItemSlot = item

	def SetUnselectEmptySlotEvent(self, empty):
		self.eventUnselectEmptySlot = empty

	def SetUnselectItemSlotEvent(self, item):
		self.eventUnselectItemSlot = item

	def SetUseSlotEvent(self, use):
		self.eventUseSlot = use

	def SetOverInItemEvent(self, event):
		self.eventOverInItem = event

	def SetOverOutItemEvent(self, event):
		self.eventOverOutItem = event

	def SetPressedSlotButtonEvent(self, event):
		self.eventPressedSlotButton = event

	def GetSlotCount(self):
		return wndMgr.GetSlotCount(self.hWnd)

	def SetUseMode(self, flag):
		"True�϶��� ItemToItem �� �������� �����ش�"
		wndMgr.SetUseMode(self.hWnd, flag)

	def SetUsableItem(self, flag):
		"True�� ���� ����Ų �������� ItemToItem ���� �����ϴ�"
		if type(flag) == int:
			pass
		else:
			wndMgr.SetUsableItem(self.hWnd, flag)

	## Slot
	def SetSlotCoolTime(self, slotIndex, coolTime, elapsedTime = 0.0):
		wndMgr.SetSlotCoolTime(self.hWnd, slotIndex, coolTime, elapsedTime)

	def StoreSlotCoolTime(self, key, slotIndex, coolTime, elapsedTime = 0.0):
		wndMgr.StoreSlotCoolTime(self.hWnd, key, slotIndex, coolTime, elapsedTime)

	def RestoreSlotCoolTime(self, key):
		wndMgr.RestoreSlotCoolTime(self.hWnd, key)

	if app.WJ_ENABLE_TRADABLE_ICON:
		def SetCanMouseEventSlot(self, slotIndex):
			wndMgr.SetCanMouseEventSlot(self.hWnd, slotIndex)

		def SetCantMouseEventSlot(self, slotIndex):
			wndMgr.SetCantMouseEventSlot(self.hWnd, slotIndex)

		def SetUsableSlotOnTopWnd(self, slotIndex):
			wndMgr.SetUsableSlotOnTopWnd(self.hWnd, slotIndex)

		def SetUnusableSlotOnTopWnd(self, slotIndex):
			wndMgr.SetUnusableSlotOnTopWnd(self.hWnd, slotIndex)

	def SetUnusableSlot(self, slotIndex):
		wndMgr.SetUnusableSlot(self.hWnd, slotIndex)

	def DisableSlot(self, slotIndex):
		wndMgr.DisableSlot(self.hWnd, slotIndex)

	def EnableSlot(self, slotIndex):
		wndMgr.EnableSlot(self.hWnd, slotIndex)

	def LockSlot(self, slotIndex):
		wndMgr.LockSlot(self.hWnd, slotIndex)

	def UnlockSlot(self, slotIndex):
		wndMgr.UnlockSlot(self.hWnd, slotIndex)

	def RefreshSlot(self):
		wndMgr.RefreshSlot(self.hWnd)

	def ClearSlot(self, slotNumber):
		wndMgr.ClearSlot(self.hWnd, slotNumber)

	def ClearAllSlot(self):
		wndMgr.ClearAllSlot(self.hWnd)

	def GetSlotIndexList(self):
		return wndMgr.GetSlotIndexList(self.hWnd)

	def SetGridSpecial(self, w, h):
		wndMgr.SetGridSpecial(self.hWnd, w, h)
	
	def GetEmptyGrid(self, item_size):
		return wndMgr.GetEmptyGrid(self.hWnd, item_size)
	
	def ClearGrid(self):
		wndMgr.ClearGrid(self.hWnd)
	
	def PutItemGrid(self, pos, size):
		wndMgr.PutItemGrid(self.hWnd, pos, size)
		
	def ClearItemGrid(self, pos, size):
		wndMgr.ClearItemGrid(self.hWnd, pos, size)

	def AppendSlot(self, index, x, y, width, height):
		wndMgr.AppendSlot(self.hWnd, index, x, y, width, height)

	def SetSlot(self, slotIndex, itemIndex, width, height, icon, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		wndMgr.SetSlot(self.hWnd, slotIndex, itemIndex, width, height, icon, diffuseColor)
		
	def SetSlotScale(self, slotIndex, itemIndex, width, height, icon, sx, sy, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		wndMgr.SetSlotScale(self.hWnd, slotIndex, itemIndex, width, height, icon, diffuseColor, sx, sy)
		
	def SetSlotCount(self, slotNumber, count):
		wndMgr.SetSlotCount(self.hWnd, slotNumber, count)

	def SetSlotCountNew(self, slotNumber, grade, count):
		wndMgr.SetSlotCountNew(self.hWnd, slotNumber, grade, count)
	def SetCardSlot(self, renderingSlotNumber, CardIndex, cardIcon, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		if 0 == CardIndex or None == CardIndex:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		item.SelectItem(CardIndex)
		(width, height) = item.GetItemSize()
		
		wndMgr.SetCardSlot(self.hWnd, renderingSlotNumber, CardIndex, width, height, cardIcon, diffuseColor)
		
	def SetItemSlot(self, renderingSlotNumber, ItemIndex, ItemCount = 0, diffuseColor = (1.0, 1.0, 1.0, 1.0),id=0):
		if 0 == ItemIndex or None == ItemIndex:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		item.SelectItem(ItemIndex)
		itemIcon = item.GetIconImage()

		item.SelectItem(ItemIndex)
		(width, height) = item.GetItemSize()	
		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, ItemIndex, width, height, itemIcon, diffuseColor)	
		wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, ItemCount)
		wndMgr.SetSlotID(self.hWnd, renderingSlotNumber, id)

	def SetSkillSlot(self, renderingSlotNumber, skillIndex, skillLevel):

		skillIcon = skill.GetIconImage(skillIndex)

		if 0 == skillIcon:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon)
		wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, skillLevel)
	if app.NEW_PET_SYSTEM:
		def SetPetSkillSlot(self, renderingSlotNumber, skillIndex, skillLevel, sx = 1.0, sy = 1.0):

			skillIcon = petskill.GetIconImage(skillIndex)

			if 0 == skillIcon:
				wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
				return
			petskill.SetSkillSlot(renderingSlotNumber, skillIndex)
			if sx != 1.0:
				wndMgr.SetSlotScale(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon, (1.0, 1.0, 1.0, 1.0), sx, sy)
			else:
				wndMgr.SetSlot(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon)
				wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, skillLevel)

	if app.ENABLE_NEW_PET_SYSTEM:
		def SetPetSkillSlot(self, renderingSlotNumber, skillIndex, skillLevel):
			if 0 == skillIndex:
				wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
				return

			if skillIndex == 99:
				skillIcon = skill.GetIconImageNewEx("d:/ymir work/ui/pet/skill_button/skill_enable_button.sub")
			else:
				skillIcon = skill.GetIconImageNewEx("d:/ymir work/ui/skill/pet_new/%d.sub"%skillIndex)
			if 0 == skillIcon:
				wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
				return
			wndMgr.SetSlot(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon)
			if skillIndex != 0:
				wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, skillLevel)

	def SetSkillSlotNew(self, renderingSlotNumber, skillIndex, skillGrade, skillLevel):

		skillIcon = skill.GetIconImageNew(skillIndex, skillGrade)

		if 0 == skillIcon:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon)

	def SetEmotionSlot(self, renderingSlotNumber, emotionIndex):
		import player
		icon = player.GetEmotionIconImage(emotionIndex)

		if 0 == icon:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, emotionIndex, 1, 1, icon)

	## Event
	def OnSelectEmptySlot(self, slotNumber):
		if self.eventSelectEmptySlot:
			self.eventSelectEmptySlot(slotNumber)

	def OnSelectItemSlot(self, slotNumber):
		if self.eventSelectItemSlot:
			self.eventSelectItemSlot(slotNumber)

	def OnUnselectEmptySlot(self, slotNumber):
		if self.eventUnselectEmptySlot:
			self.eventUnselectEmptySlot(slotNumber)

	def OnUnselectItemSlot(self, slotNumber):
		if self.eventUnselectItemSlot:
			self.eventUnselectItemSlot(slotNumber)

	def OnUseSlot(self, slotNumber):
		if self.eventUseSlot:
			self.eventUseSlot(slotNumber)

	def OnOverInItem(self, slotNumber,vnum=0,itemID=0):
		if self.eventOverInItem:
			self.eventOverInItem(slotNumber)
		if self.eventOverInItem2 and vnum>0:
			self.eventOverInItem2(vnum)
		if self.eventOverInItem3 and itemID>0:
			self.eventOverInItem3(itemID)

	def OnOverOutItem(self):
		if self.eventOverOutItem:
			self.eventOverOutItem()

	def OnPressedSlotButton(self, slotNumber):
		if self.eventPressedSlotButton:
			self.eventPressedSlotButton(slotNumber)

	def GetStartIndex(self):
		return 0

	def SetOverOutEmptySlotEvent(self, event):
		self.eventOnOverOutEmptySlot = event

	def SetOverInEmptySlotEvent(self, event):
		self.eventOnOverInEmptySlot = event

	if app.ENABLE_DSS_ACTIVE_EFFECT_BUTTON:
		def SetRenderSlot(self, renderingSlotNumber, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
			wndMgr.SetSlot(self.hWnd, renderingSlotNumber, 1, 1, 1, 0, diffuseColor)
			wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, 0)

class GridSlotWindow(SlotWindow):

	def __init__(self):
		SlotWindow.__init__(self)

		self.startIndex = 0

	def __del__(self):
		SlotWindow.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterGridSlotWindow(self, layer)

	def ArrangeSlot(self, StartIndex, xCount, yCount, xSize, ySize, xBlank, yBlank):

		self.startIndex = StartIndex

		wndMgr.ArrangeSlot(self.hWnd, StartIndex, xCount, yCount, xSize, ySize, xBlank, yBlank)
		self.startIndex = StartIndex

	def GetStartIndex(self):
		return self.startIndex

class TitleBar(Window):

	BLOCK_WIDTH = 32
	BLOCK_HEIGHT = 23

	def __init__(self):
		Window.__init__(self)
		self.AddFlag("attach")

	def __del__(self):
		Window.__del__(self)

	def MakeTitleBar(self, width, color):

		## ���� Color�� ����ϰ� ���� ����

		width = max(64, width)

		imgLeft = ImageBox()
		imgCenter = ExpandedImageBox()
		imgRight = ImageBox()
		imgLeft.AddFlag("not_pick")
		imgCenter.AddFlag("not_pick")
		imgRight.AddFlag("not_pick")
		imgLeft.SetParent(self)
		imgCenter.SetParent(self)
		imgRight.SetParent(self)

		if localeInfo.IsARABIC():
			imgLeft.LoadImage("locale/ae/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("locale/ae/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("locale/ae/ui/pattern/titlebar_right.tga")
		else:
			imgLeft.LoadImage("d:/ymir work/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("d:/ymir work/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("d:/ymir work/ui/pattern/titlebar_right.tga")

		imgLeft.Show()
		imgCenter.Show()
		imgRight.Show()

		btnClose = Button()
		btnClose.SetParent(self)
		btnClose.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		btnClose.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		btnClose.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		btnClose.SetToolTipText(localeInfo.UI_CLOSE, 0, -23)
		btnClose.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight
		self.btnClose = btnClose

		self.SetWidth(width)

	def SetWidth(self, width):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
		self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
		self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)

		if localeInfo.IsARABIC():
			self.btnClose.SetPosition(3, 3)
		else:
			self.btnClose.SetPosition(width - self.btnClose.GetWidth() - 3, 3)

		self.SetSize(width, self.BLOCK_HEIGHT)

	def SetCloseEvent(self, event):
		self.btnClose.SetEvent(event)

class HorizontalBar(Window):

	BLOCK_WIDTH = 32
	BLOCK_HEIGHT = 17

	def __init__(self):
		Window.__init__(self)
		self.AddFlag("attach")
		self.ButtonText = None

	def __del__(self):
		Window.__del__(self)
		self.ButtonText = None

	def Create(self, width):

		width = max(96, width)

		imgLeft = ImageBox()
		imgLeft.SetParent(self)
		imgLeft.AddFlag("not_pick")
		imgLeft.LoadImage("d:/ymir work/ui/pattern/horizontalbar_left.tga")
		imgLeft.Show()

		imgCenter = ExpandedImageBox()
		imgCenter.SetParent(self)
		imgCenter.AddFlag("not_pick")
		imgCenter.LoadImage("d:/ymir work/ui/pattern/horizontalbar_center.tga")
		imgCenter.Show()

		imgRight = ImageBox()
		imgRight.SetParent(self)
		imgRight.AddFlag("not_pick")
		imgRight.LoadImage("d:/ymir work/ui/pattern/horizontalbar_right.tga")
		imgRight.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight
		self.SetWidth(width)

	def SetWidth(self, width):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
		self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
		self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)
		self.SetSize(width, self.BLOCK_HEIGHT)

	def SetText(self, text):
		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

class Gauge(Window):
	SLOT_WIDTH = 16
	SLOT_HEIGHT = 7

	GAUGE_TEMPORARY_PLACE = 12
	GAUGE_WIDTH = 16

	def __init__(self):
		Window.__init__(self)
		self.width = 0
	def __del__(self):
		Window.__del__(self)

	def MakeGauge(self, width, color):

		self.width = max(48, width)

		imgSlotLeft = ImageBox()
		imgSlotLeft.SetParent(self)
		imgSlotLeft.LoadImage("d:/ymir work/ui/pattern/gauge_slot_left.tga")
		imgSlotLeft.Show()

		imgSlotRight = ImageBox()
		imgSlotRight.SetParent(self)
		imgSlotRight.LoadImage("d:/ymir work/ui/pattern/gauge_slot_right.tga")
		imgSlotRight.Show()
		imgSlotRight.SetPosition(width - self.SLOT_WIDTH, 0)

		imgSlotCenter = ExpandedImageBox()
		imgSlotCenter.SetParent(self)
		imgSlotCenter.LoadImage("d:/ymir work/ui/pattern/gauge_slot_center.tga")
		imgSlotCenter.Show()
		imgSlotCenter.SetRenderingRect(0.0, 0.0, float((width - self.SLOT_WIDTH*2) - self.SLOT_WIDTH) / self.SLOT_WIDTH, 0.0)
		imgSlotCenter.SetPosition(self.SLOT_WIDTH, 0)
  
		imgGauge = ExpandedImageBox()
		imgGauge.SetParent(self)
		imgGauge.LoadImage("d:/ymir work/ui/pattern/gauge_" + color + ".tga")
		imgGauge.Show()
		imgGauge.SetRenderingRect(0.0, 0.0, 0.0, 0.0)
		imgGauge.SetPosition(self.GAUGE_TEMPORARY_PLACE, 0)

		imgSlotLeft.AddFlag("attach")
		imgSlotCenter.AddFlag("attach")
		imgSlotRight.AddFlag("attach")

		self.imgLeft = imgSlotLeft
		self.imgCenter = imgSlotCenter
		self.imgRight = imgSlotRight
		self.imgGauge = imgGauge

		self.curValue = 100
		self.maxValue = 100
  
		self.currentGaugeColor = color

		self.SetSize(width, self.SLOT_HEIGHT)

	def SetColor(self, color):
		if (self.currentGaugeColor == color):
			return

		self.currentGaugeColor = color
		self.imgGauge.LoadImage("d:/ymir work/ui/pattern/gauge_" + color + ".tga")
		self.SetPercentage(self.curValue, self.maxValue)
  
	def SetPercentage(self, curValue, maxValue):
		self.curValue2 = curValue
		self.maxValue = maxValue

		# PERCENTAGE_MAX_VALUE_ZERO_DIVISION_ERROR
		if maxValue > 0.0:
			percentage = min(1.0, float(curValue) / float(maxValue))
		else:
			percentage = 0.0
		# END_OF_PERCENTAGE_MAX_VALUE_ZERO_DIVISION_ERROR

		gaugeSize = -1.0 + float(self.width - self.GAUGE_TEMPORARY_PLACE * 2) * percentage / self.GAUGE_WIDTH
		self.imgGauge.SetRenderingRect(0.0, 0.0, gaugeSize, 0.0)

	def GetPercentage(self):
		return (self.curValue2, self.maxValue,)

class DynamicGauge (Gauge):
	dynamicGaugePerc =  None
	newGaugePerc =  0
	def  __init__ ( self ):
		Gauge. __init__ ( self )
	def  __del__ ( self ):
		Gauge. __del__ ( self )

	def MakeGauge ( self , width, color1, color2):
		Gauge.MakeGauge ( self , width, color2)

		imgGauge2 = ExpandedImageBox ()
		imgGauge2.SetParent ( self )
		imgGauge2.LoadImage ( "d:/ymir work/ui/pattern/gauge_"  + color1 +  ".tga" )
		imgGauge2.Show ()
		imgGauge2.SetRenderingRect (0.0,0.0,0.0,0.0)
		imgGauge2.SetPosition ( self .GAUGE_TEMPORARY_PLACE,0)

		self .imgGauge2 = imgGauge2
	def SetPercentage ( self , curValue, maxValue):
		# PERCENTAGE_MAX_VALUE_ZERO_DIVISION_ERROR
		if maxValue > 0.0:
			percentage = min (1.0, float (curValue) / float (maxValue))
		else :
			percentage = 0.0
		# END_OF_PERCENTAGE_MAX_VALUE_ZERO_DIVISION_ERROR
		gaugeSize =  -1.0 + float ( self .width -  self .GAUGE_TEMPORARY_PLACE * 2 ) * percentage /  self .GAUGE_WIDTH
		
		if  self .dynamicGaugePerc ==  None :
			self .imgGauge.SetRenderingRect (0.0,0.0, ( -1.0 + float ( self .width -  self .GAUGE_TEMPORARY_PLACE * 2 ) * percentage /  self .GAUGE_WIDTH),0.0)
			self .dynamicGaugePerc = percentage
		elif  self .dynamicGaugePerc +0.2  <  self .newGaugePerc:
			self .imgGauge.SetRenderingRect (0.0,0.0, ( -1.0 + float ( self .width -  self .GAUGE_TEMPORARY_PLACE * 2 ) *  self .newGaugePerc /  self .GAUGE_WIDTH),0.0)
			self .dynamicGaugePerc =  self .newGaugePerc
		
		self .newGaugePerc = percentage
		self .imgGauge2.SetRenderingRect (0.0,0.0, gaugeSize,0.0)
		if percentage == 0:
			self .imgGauge.Hide ()
		else :
			self .imgGauge.Show ()
	
	def OnUpdate ( self ):
		if  self .dynamicGaugePerc >  self .newGaugePerc:
			self .dynamicGaugePerc =  self .dynamicGaugePerc - 0.005
			self .imgGauge.SetRenderingRect (0.0,0.0, ( -1.0 + float ( self .width -  self .GAUGE_TEMPORARY_PLACE * 2 ) *  self .dynamicGaugePerc /  self .GAUGE_WIDTH),0.0)
		elif  self .dynamicGaugePerc <  self .newGaugePerc:
			self .dynamicGaugePerc =  self .newGaugePerc
			self .imgGauge.SetRenderingRect (0.0,0.0, ( -1.0 + float ( self .width -  self .GAUGE_TEMPORARY_PLACE * 2 ) *  self .dynamicGaugePerc /  self .GAUGE_WIDTH),0.0)
			
			
class StartDungeon(Window):
	START_TOTAL = 5
	RUTA_IMGS = "System/Dungeon/new_dungeon/"

	def __init__(self):
		Window.__init__(self)
		self.CreateStart()

	def __del__(self):
		Window.__del__(self)

	def CreateStart(self):
		self.start_text = TextLine()
		self.start_text.SetParent(self)
		self.start_text.SetPosition(0,0)
		self.start_text.SetText("Dificultad: ")
		self.start_text.Show()

		self.start_empty = ExpandedImageBox()
		self.start_empty.SetParent(self)
		self.start_empty.SetPosition(65,-1)
		self.start_empty.LoadImage(self.RUTA_IMGS+"star_1.tga")
		self.start_empty.SetRenderingRect(0.0, 0.0, float(self.START_TOTAL-1), 0.0)
		self.start_empty.Show()

		self.start_full = ExpandedImageBox()
		self.start_full.SetParent(self)
		self.start_full.SetPosition(65,-1)
		self.start_full.LoadImage(self.RUTA_IMGS+"star_2.tga")
		self.start_full.SetRenderingRect(0.0, 0.0, 0.0, 0.0)
		self.start_full.Hide()

	def PercentStartDungeon(self,percent):
		start_max =  min(100,percent)
		number = float(start_max)/float(25)
		if number > 0.0:
			self.start_full.SetRenderingRect(0.0, 0.0, number, 0.0)
			self.start_full.Show()
		else:
			self.start_full.Hide()

class Board(Window):
	CORNER_WIDTH = 32
	CORNER_HEIGHT = 32
	LINE_WIDTH = 128
	LINE_HEIGHT = 128

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	BASE_PATH = "d:/ymir work/ui/pattern"
	IMAGES = {
		'CORNER' : {
			0 : "Board_Corner_LeftTop",
			1 : "Board_Corner_LeftBottom",
			2 : "Board_Corner_RightTop",
			3 : "Board_Corner_RightBottom"
		},
		'BAR' : {
			0 : "Board_Line_Left",
			1 : "Board_Line_Right",
			2 : "Board_Line_Top",
			3 : "Board_Line_Bottom"
		},
		'FILL' : "Board_Base"
	}

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.skipMaxCheck = False
		self.MakeBoard()

	def MakeBoard(self):
		CornerFileNames = []
		LineFileNames = []
		for imageDictKey in (['CORNER', 'BAR']):
			for x in xrange(len(self.IMAGES[imageDictKey])):
				if imageDictKey == "CORNER":
					CornerFileNames.append("%s/%s.tga" % (self.BASE_PATH, self.IMAGES[imageDictKey][x]))
				elif imageDictKey == "BAR":
					LineFileNames.append("%s/%s.tga" % (self.BASE_PATH, self.IMAGES[imageDictKey][x]))
		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

		self.Base = ExpandedImageBox()
		self.Base.AddFlag("not_pick")
		self.Base.LoadImage("%s/%s.tga" % (self.BASE_PATH, self.IMAGES['FILL']))
		self.Base.SetParent(self)
		self.Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Base.Show()

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		if not self.skipMaxCheck:
			width = max(self.CORNER_WIDTH*2, width)
			height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)
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

class BoardWithTitleBar(Board):
	def __init__(self):
		Board.__init__(self)

		titleBar = TitleBar()
		titleBar.SetParent(self)
		titleBar.MakeTitleBar(0, "red")
		titleBar.SetPosition(8, 7)
		titleBar.Show()

		titleName = TextLine()
		titleName.SetParent(titleBar)
		titleName.SetPosition(0, 4)
		titleName.SetWindowHorizontalAlignCenter()
		titleName.SetHorizontalAlignCenter()
		titleName.Show()

		self.titleBar = titleBar
		self.titleName = titleName

		self.SetCloseEvent(self.Hide)

	def __del__(self):
		Board.__del__(self)
		self.titleBar = None
		self.titleName = None

	def SetSize(self, width, height):
		self.titleBar.SetWidth(width - 15)
		#self.pickRestrictWindow.SetSize(width, height - 30)
		Board.SetSize(self, width, height)
		self.titleName.UpdateRect()

	def SetTitleColor(self, color):
		self.titleName.SetPackedFontColor(color)

	def SetTitleName(self, name):
		self.titleName.SetText(name)

	def SetCloseEvent(self, event):
		self.titleBar.SetCloseEvent(event)
		
if app.ENABLE_SUNG_MAHI_TOWER:
	class BoarderA(Board):
		CORNER_WIDTH = 16
		CORNER_HEIGHT = 16
		LINE_WIDTH = 16
		LINE_HEIGHT = 16
		
		IMAGES = {
			'CORNER' : {
				0 : "border_a_left_top",
				1 : "border_a_left_bottom",
				2 : "border_a_right_top",
				3 : "border_a_right_bottom"
			},
			'BAR' : {
				0 : "border_a_left",
				1 : "border_a_right",
				2 : "border_a_top",
				3 : "border_a_bottom"
			},
			'FILL' : "border_a_center"
		}
		
		def __init__(self):
			Board.__init__(self)

		def __del__(self):
			Board.__del__(self)

		def SetSize(self, width, height):
			Board.SetSize(self, width, height)
		
class ThinBoardGold(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		CornerFileNames = [ "d:/ymir work/ui/pattern/thinboardgold/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop_gold", "LeftBottom_gold","RightTop_gold", "RightBottom_gold"]]
		LineFileNames = [ "d:/ymir work/ui/pattern/thinboardgold/ThinBoard_Line_"+dir+".tga" for dir in ["Left_gold", "Right_gold", "Top_gold", "Bottom_gold"]]
		
		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = ExpandedImageBox()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.LoadImage("d:/ymir work/ui/pattern/thinboardgold/thinboard_bg_gold.tga")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

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

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()



class ThinBoardCircle(Window):
	CORNER_WIDTH = 4
	CORNER_HEIGHT = 4
	LINE_WIDTH = 4
	LINE_HEIGHT = 4
	BOARD_COLOR = grp.GenerateColor(255.0, 255.0, 255.0, 1.0)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/thinboardcircle/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop_circle","LeftBottom_circle","RightTop_circle","RightBottom_circle"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/thinboardcircle/ThinBoard_Line_"+dir+".tga" for dir in ["Left_circle","Right_circle","Top_circle","Bottom_circle"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

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
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def SetAlpha(self, alpha):
		color  = grp.GenerateColor(0.0, 0.0, 0.0, alpha)
		self.Base.SetColor(color)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard(Window):

	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.47)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	#if app.USE_OFFLINESHOP_SEARCH_V2:
	def __init__(self, layer = "UI", v = 1):
		Window.__init__(self, layer)

		if app.USE_OFFLINESHOP_SEARCH_V2 and v == 2:
			CornerFileNames = [ "d:/ymir work/ui/pattern/v2_thinboard_corner_" + dir + ".tga" for dir in ["lefttop", "leftbottom", "righttop", "rightbottom"] ]
			LineFileNames = [ "d:/ymir work/ui/pattern/v2_thinboard_line_" + dir + ".tga" for dir in ["left", "right", "top", "bottom"] ]
		elif app.USE_OFFLINESHOP_SEARCH_V2 and v == 3:
			CornerFileNames = [ "d:/ymir work/ui/pattern/v3_thinboard_corner_" + dir + ".tga" for dir in ["lefttop", "leftbottom", "righttop", "rightbottom"] ]
			LineFileNames = [ "d:/ymir work/ui/pattern/v3_thinboard_line_" + dir + ".tga" for dir in ["left", "right", "top", "bottom"] ]
		else:
			CornerFileNames = [ "d:/ymir work/ui/pattern/thinboard_corner_" + dir + ".tga" for dir in ["lefttop", "leftbottom", "righttop", "rightbottom"] ]
			LineFileNames = [ "d:/ymir work/ui/pattern/thinboard_line_" + dir + ".tga" for dir in ["left", "right", "top", "bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)
		
	if app.ENABLE_SEND_TARGET_INFO:
		def ShowCorner(self, corner):
			self.Corners[corner].Show()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def HideCorners(self, corner):
			self.Corners[corner].Hide()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def ShowLine(self, line):
			self.Lines[line].Show()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def HideLine(self, line):
			self.Lines[line].Hide()
			self.SetSize(self.GetWidth(), self.GetHeight())

	def SetAlpha(self, alpha):#incearca
		color  = grp.GenerateColor(0.0, 0.0, 0.0, alpha)
		self.Base.SetColor(color)
		for wnd in self.Lines:
			wnd.SetAlpha(alpha)
		for wnd in self.Corners:
			wnd.SetAlpha(alpha)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

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
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

if app.ENABLE_SWITCHBOT_WORLDARD:
	class ScrollBarNew1(Window):
		MIDDLE_BAR_POS = 4
		SCROLLBAR_BUTTON_HEIGHT = 0
		SCROLLBAR_WIDTH = 8
		SCROLLBAR_MIDDLE_HEIGHT = 33

		class MiddleBar(DragButton):
			def __init__(self):
				DragButton.__init__(self)
				self.AddFlag("movable")

			def MakeImage(self):
				bar = ImageBox()
				bar.SetParent(self)
				bar.LoadImage("d:/ymir work/ui/game/switchbot/design/scroll_button_1.tga")
				bar.SetPosition(0, 0)
				bar.AddFlag("not_pick")
				bar.Show()

				self.bar = bar

			def SetSize(self, height):
				height = max(12, height)
				DragButton.SetSize(self, 10, height)

				height -= 4*3

		def __init__(self):
			Window.__init__(self)

			self.pageSize = 1
			self.curPos = 0.0
			self.eventScroll = lambda *arg: None
			self.lockFlag = False
			self.scrollStep = 0.20


			self.CreateScrollBar()

		def __del__(self):
			Window.__del__(self)

		def CreateScrollBar(self):
			barSlot = ImageBox()
			barSlot.SetParent(self)
			barSlot.AddFlag("not_pick")
			barSlot.LoadImage("d:/ymir work/ui/game/switchbot/design/scroll_background.tga")
			barSlot.Show()

			middleBar = self.MiddleBar()
			middleBar.SetParent(self)
			middleBar.SetMoveEvent(__mem_func__(self.OnMove))
			middleBar.Show()
			middleBar.MakeImage()
			middleBar.SetSize(33)

			self.middleBar = middleBar
			self.barSlot = barSlot

			self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()


		def Destroy(self):
			self.middleBar = None
			self.eventScroll = lambda *arg: None

		def SetScrollEvent(self, event):
			self.eventScroll = event

		def GetPos(self):
			return self.curPos

		def SetPos(self, pos):
			pos = max(0.0, pos)
			pos = min(1.0, pos)

			newPos = float(self.pageSize) * pos
			self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos))
			self.OnMove()


		def SetScrollBarSize(self, height):
			self.pageSize = height- self.SCROLLBAR_MIDDLE_HEIGHT
			self.SetSize(self.SCROLLBAR_WIDTH, height)
			self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, 0, 8, 156)
			self.middleBar.SetPosition(self.MIDDLE_BAR_POS, 0)

			self.UpdateBarSlot()

		def UpdateBarSlot(self):
			self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)

		def SetScrollStep(self, step):
			self.scrollStep = step

		def GetScrollStep(self):
			return self.scrollStep

		def OnUp(self):
			self.SetPos(self.curPos-self.scrollStep)

		def OnDown(self):
			self.SetPos(self.curPos+self.scrollStep)

		def OnMove(self):

			if self.lockFlag:
				return

			if 0 == self.pageSize:
				return

			(xLocal, yLocal) = self.middleBar.GetLocalPosition()
			self.curPos = float(yLocal) / float(self.pageSize)

			self.eventScroll()

		def OnMouseLeftButtonDown(self):
			(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
			pickedPos = yMouseLocalPosition  - self.SCROLLBAR_MIDDLE_HEIGHT/2
			newPos = float(pickedPos) / float(self.pageSize)
			self.SetPos(newPos)

		def LockScroll(self):
			self.lockFlag = True

		def UnlockScroll(self):
			self.lockFlag = False

class ScrollBarNew(Window):
	MIDDLE_BAR_POS = 4
	SCROLLBAR_BUTTON_HEIGHT = 0
	SCROLLBAR_WIDTH = 8
	SCROLLBAR_MIDDLE_HEIGHT = 33

	class MiddleBar(DragButton):
		def __init__(self):
			DragButton.__init__(self)
			self.AddFlag("movable")
			self.AddFlag("animate")

		def MakeImage(self):
			bar = ImageBox()
			bar.SetParent(self)
			bar.LoadImage("tabla_bonus/scroll_button_1.tga")
			bar.SetPosition(0, 0)
			bar.AddFlag("not_pick")
			bar.Show()

			self.bar = bar

		def SetSize(self, height):
			height = max(12, height)
			DragButton.SetSize(self, 10, height)

			height -= 4*3

	def __init__(self):
		Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = lambda *arg: None
		self.lockFlag = False
		self.scrollStep = 0.20


		self.CreateScrollBar()

	def __del__(self):
		Window.__del__(self)

	def CreateScrollBar(self):
		barSlot = ImageBox()
		barSlot.SetParent(self)
		barSlot.AddFlag("not_pick")
		barSlot.LoadImage("tabla_bonus/scroll_bar.tga")
		barSlot.Show()

		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.MakeImage()
		middleBar.SetSize(33)

		self.middleBar = middleBar
		self.barSlot = barSlot

		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()


	def Destroy(self):
		self.middleBar = None
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def GetPos(self):
		return self.curPos

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos))
		self.OnMove()


	def SetScrollBarSize(self, height):
		self.pageSize = height- self.SCROLLBAR_MIDDLE_HEIGHT
		self.SetSize(self.SCROLLBAR_WIDTH, height)
		self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, 0, 8, 293)
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, 0)

		self.UpdateBarSlot()

	def UpdateBarSlot(self):
		self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)

	def SetScrollStep(self, step):
		self.scrollStep = step

	def GetScrollStep(self):
		return self.scrollStep

	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def OnMove(self):

		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal) / float(self.pageSize)

		self.eventScroll()

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		pickedPos = yMouseLocalPosition  - self.SCROLLBAR_MIDDLE_HEIGHT/2
		newPos = float(pickedPos) / float(self.pageSize)
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = True

	def UnlockScroll(self):
		self.lockFlag = False


class ScrollBarItemShop(Window):
	MIDDLE_BAR_POS = 4
	SCROLLBAR_BUTTON_HEIGHT = 0
	SCROLLBAR_WIDTH = 8
	SCROLLBAR_MIDDLE_HEIGHT = 33

	class MiddleBar(DragButton):
		def __init__(self):
			DragButton.__init__(self)
			self.AddFlag("movable")
			self.AddFlag("animate")

		def MakeImage(self):
			bar = ImageBox()
			bar.SetParent(self)
			bar.LoadImage("Modulo1/itemshop_rework/scroll_button_1.tga")
			bar.SetPosition(0, 0)
			bar.AddFlag("not_pick")
			bar.Show()

			self.bar = bar

		def SetSize(self, height):
			height = max(12, height)
			DragButton.SetSize(self, 10, height)

			height -= 4*3

	def __init__(self):
		Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = lambda *arg: None
		self.lockFlag = False
		self.scrollStep = 0.20


		self.CreateScrollBar()

	def __del__(self):
		Window.__del__(self)

	def CreateScrollBar(self):
		barSlot = ImageBox()
		barSlot.SetParent(self)
		barSlot.AddFlag("not_pick")
		barSlot.LoadImage("Modulo1/itemshop_rework/scroll_bar.tga")
		barSlot.Show()

		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.MakeImage()
		middleBar.SetSize(33)

		self.middleBar = middleBar
		self.barSlot = barSlot

		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()


	def Destroy(self):
		self.middleBar = None
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def GetPos(self):
		return self.curPos

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos))
		self.OnMove()


	def SetScrollBarSize(self, height):
		self.pageSize = height- self.SCROLLBAR_MIDDLE_HEIGHT
		self.SetSize(self.SCROLLBAR_WIDTH, height)
		self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, 0, 8, 462)
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, 0)

		self.UpdateBarSlot()

	def UpdateBarSlot(self):
		self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)

	def SetScrollStep(self, step):
		self.scrollStep = step

	def GetScrollStep(self):
		return self.scrollStep

	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def OnMove(self):

		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal) / float(self.pageSize)

		self.eventScroll()

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		pickedPos = yMouseLocalPosition  - self.SCROLLBAR_MIDDLE_HEIGHT/2
		newPos = float(pickedPos) / float(self.pageSize)
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = True

	def UnlockScroll(self):
		self.lockFlag = False

class NewScrollBar(Window):

	SCROLLBAR_WIDTH = 13
	SCROLLBAR_MIDDLE_HEIGHT = 1
	SCROLLBAR_BUTTON_WIDTH = 17
	SCROLLBAR_BUTTON_HEIGHT = 17
	SCROLL_BTN_XDIST = 1
	SCROLL_BTN_YDIST = 2

	class MiddleBar(DragButton):
		def __init__(self):
			DragButton.__init__(self)
			self.AddFlag("movable")

			self.SetWindowName("NONAME_ScrollBar_MiddleBar")

		def MakeImage(self):
			top = ImageBox()
			top.SetParent(self)
			top.LoadImage("d:/ymir work/ui/pattern/slimscroll/ScrollBar_Middle_Top_bottom.tga")
			top.AddFlag("not_pick")
			top.SetPosition(0, 0)
			top.Show()
			topScale = ExpandedImageBox()
			topScale.SetParent(self)
			topScale.SetPosition(0, top.GetHeight())
			topScale.LoadImage("d:/ymir work/ui/pattern/slimscroll/ScrollBar_Middle_TopScale.tga")
			topScale.AddFlag("not_pick")
			topScale.Show()

			bottom = ImageBox()
			bottom.SetParent(self)
			bottom.LoadImage("d:/ymir work/ui/pattern/slimscroll/ScrollBar_Middle_Top_bottom.tga")
			bottom.AddFlag("not_pick")
			bottom.Show()
			bottomScale = ExpandedImageBox()
			bottomScale.SetParent(self)
			bottomScale.LoadImage("d:/ymir work/ui/pattern/slimscroll/ScrollBar_Middle_TopScale.tga")
			bottomScale.AddFlag("not_pick")
			bottomScale.Show()

			middle = ExpandedImageBox()
			middle.SetParent(self)
			middle.LoadImage("d:/ymir work/ui/pattern/slimscroll/ScrollBar_Middle_Middle.tga")
			middle.AddFlag("not_pick")
			middle.Show()

			self.top = top
			self.topScale = topScale
			self.bottom = bottom
			self.bottomScale = bottomScale
			self.middle = middle

		def SetSize(self, height):
			minHeight = self.top.GetHeight() + self.bottom.GetHeight() + self.middle.GetHeight()
			height = max(minHeight, height)
			DragButton.SetSize(self, 10, height)

			scale = (height - minHeight) / 2
			extraScale = 0
			if (height - minHeight) % 2 == 1:
				extraScale = 1

			self.topScale.SetRenderingRect(0, 0, 0, scale - 1)
			self.middle.SetPosition(0, self.top.GetHeight() + scale)
			self.bottomScale.SetPosition(0, self.middle.GetBottom())
			self.bottomScale.SetRenderingRect(0, 0, 0, scale - 1 + extraScale)
			self.bottom.SetPosition(0, height - self.bottom.GetHeight())

	def __init__(self):
		Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = None
		self.eventArgs = None
		self.lockFlag = False

		self.CreateScrollBar()
		self.SetScrollBarSize(0)

		self.scrollStep = 0.20
		self.SetWindowName("NONAME_ScrollBar")

	def __del__(self):
		Window.__del__(self)

	def CreateScrollBar(self):
		middleImage = ExpandedImageBox()
		middleImage.SetParent(self)
		middleImage.AddFlag("not_pick")
		middleImage.SetPosition(0, 1)
		middleImage.LoadImage("d:/ymir work/ui/pattern/slimscroll/SlimScrollBar_Middle.tga")
		middleImage.Show()
		self.middleImage = middleImage

		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.MakeImage()
		middleBar.SetSize(0) # set min height
		self.middleBar = middleBar

	def Destroy(self):
		self.eventScroll = None
		self.eventArgs = None

	def SetScrollEvent(self, event, *args):
		self.eventScroll = event
		self.eventArgs = args

	def SetMiddleBarSize(self, pageScale):
		self.middleBar.SetSize(int(pageScale * float(self.GetHeight() - self.SCROLL_BTN_YDIST*2)))
		realHeight = self.GetHeight() - self.SCROLL_BTN_YDIST*2 - self.middleBar.GetHeight()
		self.pageSize = realHeight

	def SetScrollBarSize(self, height):
		self.SetSize(self.SCROLLBAR_WIDTH, height)

		self.pageSize = height - self.SCROLL_BTN_YDIST*2 - self.middleBar.GetHeight()

		middleImageScale = float((height - self.SCROLL_BTN_YDIST*2) - self.middleImage.GetHeight()) / float(self.middleImage.GetHeight())
		self.middleImage.SetRenderingRect(0, 0, 0, middleImageScale)

		self.middleBar.SetRestrictMovementArea(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST, \
			self.middleBar.GetWidth(), height - self.SCROLL_BTN_YDIST * 2)
		self.middleBar.SetPosition(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST)

	def SetScrollStep(self, step):
		self.scrollStep = step

	def GetScrollStep(self):
		return self.scrollStep

	def GetPos(self):
		return self.curPos

	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def SetPos(self, pos, moveEvent = True):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.SCROLL_BTN_XDIST, int(newPos) + self.SCROLL_BTN_YDIST)
		if moveEvent == True:
			self.OnMove()

	def OnMove(self):
		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal - self.SCROLL_BTN_YDIST) / float(self.pageSize)

		if self.eventScroll:
			apply(self.eventScroll, self.eventArgs)

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		newPos = float(yMouseLocalPosition) / float(self.GetHeight())
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = True

	def UnlockScroll(self):
		self.lockFlag = False


class ScrollBar(Window):

	SCROLLBAR_WIDTH = 17
	SCROLLBAR_MIDDLE_HEIGHT = 9
	SCROLLBAR_BUTTON_WIDTH = 17
	SCROLLBAR_BUTTON_HEIGHT = 17
	MIDDLE_BAR_POS = 5
	MIDDLE_BAR_UPPER_PLACE = 3
	MIDDLE_BAR_DOWNER_PLACE = 4
	TEMP_SPACE = MIDDLE_BAR_UPPER_PLACE + MIDDLE_BAR_DOWNER_PLACE

	class MiddleBar(DragButton):
		def __init__(self):
			DragButton.__init__(self)
			self.AddFlag("movable")
			self.AddFlag("animate")
			#self.AddFlag("restrict_x")

		def MakeImage(self):
			top = ImageBox()
			top.SetParent(self)
			top.LoadImage("d:/ymir work/ui/pattern/ScrollBar_Top.tga")
			top.SetPosition(0, 0)
			top.AddFlag("not_pick")
			top.Show()
			bottom = ImageBox()
			bottom.SetParent(self)
			bottom.LoadImage("d:/ymir work/ui/pattern/ScrollBar_Bottom.tga")
			bottom.AddFlag("not_pick")
			bottom.Show()

			middle = ExpandedImageBox()
			middle.SetParent(self)
			middle.LoadImage("d:/ymir work/ui/pattern/ScrollBar_Middle.tga")
			middle.SetPosition(0, 4)
			middle.AddFlag("not_pick")
			middle.Show()

			self.top = top
			self.bottom = bottom
			self.middle = middle

		def SetSize(self, height):
			height = max(12, height)
			DragButton.SetSize(self, 10, height)
			self.bottom.SetPosition(0, height-4)

			height -= 4*3
			self.middle.SetRenderingRect(0, 0, 0, float(height)/4.0)

	def __init__(self):
		Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = lambda *arg: None
		self.lockFlag = False
		self.scrollStep = 0.20


		self.CreateScrollBar()

	def __del__(self):
		Window.__del__(self)

	def CreateScrollBar(self):
		barSlot = Bar3D()
		barSlot.SetParent(self)
		barSlot.AddFlag("not_pick")
		barSlot.Show()

		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.MakeImage()
		middleBar.SetSize(12)

		upButton = Button()
		upButton.SetParent(self)
		upButton.SetEvent(__mem_func__(self.OnUp))
		upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_up_button_01.sub")
		upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_up_button_02.sub")
		upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_up_button_03.sub")
		upButton.Show()

		downButton = Button()
		downButton.SetParent(self)
		downButton.SetEvent(__mem_func__(self.OnDown))
		downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_down_button_01.sub")
		downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_down_button_02.sub")
		downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_down_button_03.sub")
		downButton.Show()

		self.upButton = upButton
		self.downButton = downButton
		self.middleBar = middleBar
		self.barSlot = barSlot

		self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
		self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()

	def Destroy(self):
		self.middleBar = None
		self.upButton = None
		self.downButton = None
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def SetMiddleBarSize(self, pageScale):
		realHeight = self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2
		self.SCROLLBAR_MIDDLE_HEIGHT = int(pageScale * float(realHeight))
		self.middleBar.SetSize(self.SCROLLBAR_MIDDLE_HEIGHT)
		self.pageSize = (self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2) - self.SCROLLBAR_MIDDLE_HEIGHT - (self.TEMP_SPACE)

	def SetScrollBarSize(self, height):
		self.pageSize = (height - self.SCROLLBAR_BUTTON_HEIGHT*2) - self.SCROLLBAR_MIDDLE_HEIGHT - (self.TEMP_SPACE)
		self.SetSize(self.SCROLLBAR_WIDTH, height)
		self.upButton.SetPosition(0, 0)
		self.downButton.SetPosition(0, height - self.SCROLLBAR_BUTTON_HEIGHT)
		self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, self.SCROLLBAR_BUTTON_HEIGHT + self.MIDDLE_BAR_UPPER_PLACE, self.MIDDLE_BAR_POS+2, height - self.SCROLLBAR_BUTTON_HEIGHT*2 - self.TEMP_SPACE)
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, 0)

		self.UpdateBarSlot()

	def UpdateBarSlot(self):
		self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)
		self.barSlot.SetSize(self.GetWidth() - 2, self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2 - 2)
	if app.ENABLE_IKASHOP_RENEWAL:
		def UpdateScrollbarLenght(self, scrollableSpace):
			minheight = self.GetHeight() - self.downButton.GetHeight()*2
			scrollable = max(minheight, scrollableSpace)
			scale = float(scrollable) / max(1, minheight)
			scale = 1.0 / max(1.0, scale)
			scale = max(0.08, scale)
			self.SetScrollStep(scale/3.0)
			self.SetMiddleBarSize(scale)


	def GetPos(self):
		return self.curPos

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos) + self.SCROLLBAR_BUTTON_HEIGHT + self.MIDDLE_BAR_UPPER_PLACE)
		self.OnMove()

	def SetScrollStep(self, step):
		self.scrollStep = step

	def GetScrollStep(self):
		return self.scrollStep

	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def OnMove(self):

		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal - self.SCROLLBAR_BUTTON_HEIGHT - self.MIDDLE_BAR_UPPER_PLACE) / float(self.pageSize)

		self.eventScroll()

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		pickedPos = yMouseLocalPosition - self.SCROLLBAR_BUTTON_HEIGHT - self.SCROLLBAR_MIDDLE_HEIGHT/2
		newPos = float(pickedPos) / float(self.pageSize)
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = True

	def UnlockScroll(self):
		self.lockFlag = False

class ThinScrollBar(ScrollBar):

	def CreateScrollBar(self):
		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.SetUpVisual("d:/ymir work/ui/public/scrollbar_thin_middle_button_01.sub")
		middleBar.SetOverVisual("d:/ymir work/ui/public/scrollbar_thin_middle_button_02.sub")
		middleBar.SetDownVisual("d:/ymir work/ui/public/scrollbar_thin_middle_button_03.sub")

		upButton = Button()
		upButton.SetParent(self)
		upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_thin_up_button_01.sub")
		upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_thin_up_button_02.sub")
		upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_thin_up_button_03.sub")
		upButton.SetEvent(__mem_func__(self.OnUp))
		upButton.Show()

		downButton = Button()
		downButton.SetParent(self)
		downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_thin_down_button_01.sub")
		downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_thin_down_button_02.sub")
		downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_thin_down_button_03.sub")
		downButton.SetEvent(__mem_func__(self.OnDown))
		downButton.Show()

		self.middleBar = middleBar
		self.upButton = upButton
		self.downButton = downButton

		self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
		self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()
		self.MIDDLE_BAR_POS = 0
		self.MIDDLE_BAR_UPPER_PLACE = 0
		self.MIDDLE_BAR_DOWNER_PLACE = 0
		self.TEMP_SPACE = 0

	def UpdateBarSlot(self):
		pass

class SmallThinScrollBar(ScrollBar):

	def CreateScrollBar(self):
		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.SetUpVisual("d:/ymir work/ui/public/scrollbar_small_thin_middle_button_01.sub")
		middleBar.SetOverVisual("d:/ymir work/ui/public/scrollbar_small_thin_middle_button_01.sub")
		middleBar.SetDownVisual("d:/ymir work/ui/public/scrollbar_small_thin_middle_button_01.sub")

		upButton = Button()
		upButton.SetParent(self)
		upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_small_thin_up_button_01.sub")
		upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_small_thin_up_button_02.sub")
		upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_small_thin_up_button_03.sub")
		upButton.SetEvent(__mem_func__(self.OnUp))
		upButton.Show()

		downButton = Button()
		downButton.SetParent(self)
		downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_small_thin_down_button_01.sub")
		downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_small_thin_down_button_02.sub")
		downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_small_thin_down_button_03.sub")
		downButton.SetEvent(__mem_func__(self.OnDown))
		downButton.Show()

		self.middleBar = middleBar
		self.upButton = upButton
		self.downButton = downButton

		self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
		self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()
		self.MIDDLE_BAR_POS = 0
		self.MIDDLE_BAR_UPPER_PLACE = 0
		self.MIDDLE_BAR_DOWNER_PLACE = 0
		self.TEMP_SPACE = 0

	def UpdateBarSlot(self):
		pass

class SliderBar(Window):

	def __init__(self):
		Window.__init__(self)

		self.curPos = 1.0
		self.pageSize = 1.0
		self.eventChange = None

		self.__CreatebackGroundImage()
		self.__CreateCursor()

	def __del__(self):
		Window.__del__(self)

	def __CreatebackGroundImage(self):
		if app.ENABLE_NEW_GAMEOPTION:
			img = ExpandedImageBox()
		else:
			img = ImageBox()
		img.SetParent(self)
		img.LoadImage("d:/ymir work/ui/game/windows/sliderbar.sub")
		img.Show()
		self.backGroundImage = img

		##
		self.SetSize(self.backGroundImage.GetWidth(), self.backGroundImage.GetHeight())

	def __CreateCursor(self):
		cursor = DragButton()
		cursor.AddFlag("movable")
		cursor.AddFlag("animate")
		cursor.AddFlag("restrict_y")
		cursor.SetParent(self)
		cursor.SetMoveEvent(__mem_func__(self.__OnMove))
		cursor.SetUpVisual("d:/ymir work/ui/game/windows/sliderbar_cursor.sub")
		cursor.SetOverVisual("d:/ymir work/ui/game/windows/sliderbar_cursor.sub")
		cursor.SetDownVisual("d:/ymir work/ui/game/windows/sliderbar_cursor.sub")
		cursor.Show()
		self.cursor = cursor

		##
		self.cursor.SetRestrictMovementArea(0, 0, self.backGroundImage.GetWidth(), 0)
		self.pageSize = self.backGroundImage.GetWidth() - self.cursor.GetWidth()

	def __OnMove(self):
		(xLocal, yLocal) = self.cursor.GetLocalPosition()
		self.curPos = float(xLocal) / float(self.pageSize)

		if self.eventChange:
			self.eventChange()

	def SetSliderPos(self, pos):
		self.curPos = pos
		self.cursor.SetPosition(int(self.pageSize * pos), 0)

	def GetSliderPos(self):
		return self.curPos

	def SetEvent(self, event):
		self.eventChange = event

	def Enable(self):
		self.cursor.Show()

	def Disable(self):
		self.cursor.Hide()

class ListBox(Window):

	TEMPORARY_PLACE = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.overLine = -1
		self.selectedLine = -1
		self.width = 0
		self.height = 0
		self.stepSize = 17
		self.basePos = 0
		self.showLineCount = 0
		self.itemCenterAlign = True
		self.itemList = []
		self.keyDict = {}
		self.textDict = {}
		self.event = lambda *arg: None
	def __del__(self):
		Window.__del__(self)

	def SetWidth(self, width):
		self.SetSize(width, self.height)

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height

	def SetTextCenterAlign(self, flag):
		self.itemCenterAlign = flag

	def SetBasePos(self, pos):
		self.basePos = pos
		self._LocateItem()

	def ClearItem(self):
		self.keyDict = {}
		self.textDict = {}
		self.itemList = []
		self.overLine = -1
		self.selectedLine = -1

	def InsertItem(self, number, text):
		self.keyDict[len(self.itemList)] = number
		self.textDict[len(self.itemList)] = text

		textLine = TextLine()
		textLine.SetParent(self)
		textLine.SetText(text)
		textLine.Show()

		if self.itemCenterAlign:
			textLine.SetWindowHorizontalAlignCenter()
			textLine.SetHorizontalAlignCenter()

		self.itemList.append(textLine)

		self._LocateItem()

	def ChangeItem(self, number, text):
		for key, value in self.keyDict.items():
			if value == number:
				self.textDict[key] = text

				if number < len(self.itemList):
					self.itemList[key].SetText(text)

				return

	def LocateItem(self):
		self._LocateItem()

	def _LocateItem(self):

		skipCount = self.basePos
		yPos = 0
		self.showLineCount = 0

		for textLine in self.itemList:
			textLine.Hide()

			if skipCount > 0:
				skipCount -= 1
				continue

			if localeInfo.IsARABIC():
				w, h = textLine.GetTextSize()
				textLine.SetPosition(w+10, yPos + 3)
			else:
				textLine.SetPosition(0, yPos + 3)

			yPos += self.stepSize

			if yPos <= self.GetHeight():
				self.showLineCount += 1
				textLine.Show()

	def ArrangeItem(self):
		self.SetSize(self.width, len(self.itemList) * self.stepSize)
		self._LocateItem()

	def GetViewItemCount(self):
		return int(self.GetHeight() / self.stepSize)

	def GetItemCount(self):
		return len(self.itemList)

	def SetEvent(self, event):
		self.event = event

	def SelectItem(self, line):

		if not self.keyDict.has_key(line):
			return

		if line == self.selectedLine:
			return

		self.selectedLine = line
		self.event(self.keyDict.get(line, 0), self.textDict.get(line, "None"))

	def GetSelectedItem(self):
		return self.keyDict.get(self.selectedLine, 0)

	def OnMouseLeftButtonDown(self):
		if self.overLine < 0:
			return

	def OnMouseLeftButtonUp(self):
		if self.overLine >= 0:
			self.SelectItem(self.overLine+self.basePos)

	def OnUpdate(self):

		self.overLine = -1

		if self.IsIn():
			x, y = self.GetGlobalPosition()
			height = self.GetHeight()
			xMouse, yMouse = wndMgr.GetMousePosition()

			if yMouse - y < height - 1:
				self.overLine = (yMouse - y) / self.stepSize

				if self.overLine < 0:
					self.overLine = -1
				if self.overLine >= len(self.itemList):
					self.overLine = -1

	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()
		yRender -= self.TEMPORARY_PLACE
		widthRender = self.width
		heightRender = self.height + self.TEMPORARY_PLACE*2

		if localeInfo.IsCIBN10:
			if -1 != self.overLine and self.keyDict[self.overLine] != -1:
				grp.SetColor(HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)

			if -1 != self.selectedLine and self.keyDict[self.selectedLine] != -1:
				if self.selectedLine >= self.basePos:
					if self.selectedLine - self.basePos < self.showLineCount:
						grp.SetColor(SELECT_COLOR)
						grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)

		else:
			if -1 != self.overLine:
				grp.SetColor(HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)

			if -1 != self.selectedLine:
				if self.selectedLine >= self.basePos:
					if self.selectedLine - self.basePos < self.showLineCount:
						grp.SetColor(SELECT_COLOR)
						grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)



class ListBox2(ListBox):
	def __init__(self, *args, **kwargs):
		ListBox.__init__(self, *args, **kwargs)
		self.rowCount = 10
		self.barWidth = 0
		self.colCount = 0

	def SetRowCount(self, rowCount):
		self.rowCount = rowCount

	def SetSize(self, width, height):
		ListBox.SetSize(self, width, height)
		self._RefreshForm()

	def ClearItem(self):
		ListBox.ClearItem(self)
		self._RefreshForm()

	def InsertItem(self, *args, **kwargs):
		ListBox.InsertItem(self, *args, **kwargs)
		self._RefreshForm()

	def OnUpdate(self):
		mpos = wndMgr.GetMousePosition()
		self.overLine = self._CalcPointIndex(mpos)

	def OnRender(self):
		x, y = self.GetGlobalPosition()
		pos = (x + 2, y)

		if -1 != self.overLine:
			grp.SetColor(HALF_WHITE_COLOR)
			self._RenderBar(pos, self.overLine)

		if -1 != self.selectedLine:
			if self.selectedLine >= self.basePos:
				if self.selectedLine - self.basePos < self.showLineCount:
					grp.SetColor(SELECT_COLOR)
					self._RenderBar(pos, self.selectedLine-self.basePos)



	def _CalcPointIndex(self, mpos):
		if self.IsIn():
			px, py = mpos
			gx, gy = self.GetGlobalPosition()
			lx, ly = px - gx, py - gy

			col = lx / self.barWidth
			row = ly / self.stepSize
			idx = col * self.rowCount + row
			if col >= 0 and col < self.colCount:
				if row >= 0 and row < self.rowCount:
					if idx >= 0 and idx < len(self.itemList):
						return idx

		return -1

	def _CalcRenderPos(self, pos, idx):
		x, y = pos
		row = idx % self.rowCount
		col = idx / self.rowCount
		return (x + col * self.barWidth, y + row * self.stepSize)

	def _RenderBar(self, basePos, idx):
		x, y = self._CalcRenderPos(basePos, idx)
		grp.RenderBar(x, y, self.barWidth - 3, self.stepSize)

	def _LocateItem(self):
		pos = (0, self.TEMPORARY_PLACE)

		self.showLineCount = 0
		for textLine in self.itemList:
			x, y = self._CalcRenderPos(pos, self.showLineCount)
			textLine.SetPosition(x, y)
			textLine.Show()

			self.showLineCount += 1

	def _RefreshForm(self):
		if len(self.itemList) % self.rowCount:
			self.colCount = len(self.itemList) / self.rowCount + 1
		else:
			self.colCount = len(self.itemList) / self.rowCount

		if self.colCount:
			self.barWidth = self.width / self.colCount
		else:
			self.barWidth = self.width
class TextLink(Window):
	NORMAL_COLOR =  0xfff9e486
	OVER_COLOR = 0xffe6d0a2
	DOWN_COLOR = 0xffefe4cd
	
	def __init__(self):
		Window.__init__(self)
	
		self.eventFunc = None
		self.eventArgs = None
	
		self.text = TextLine()
		self.text.SetParent(self)
		self.text.SetPackedFontColor(self.NORMAL_COLOR)
		self.text.Show()
	
		self.underline = TextLine()
		self.underline.SetParent(self)
		self.underline.SetPackedFontColor(self.NORMAL_COLOR)
		self.underline.Hide()
	
	def __del__(self):
		Window.__del__(self)
	
	def SetText(self, text):
		self.text.SetText(text)
		self.SetSize(self.text.GetTextSize()[0], self.text.GetTextSize()[1])
		self.underline.SetPosition(-20, self.text.GetTextSize()[1])
		self.underline.SetWindowHorizontalAlignCenter()
		self.underline.SetSize(self.text.GetTextSize()[0], 0)
	
	def OnMouseOverIn(self):
		self.text.SetPackedFontColor(self.OVER_COLOR)
		self.underline.Show()
	
	def OnMouseOverOut(self):
		self.text.SetPackedFontColor(self.NORMAL_COLOR)
		self.underline.Hide()
	
	def OnMouseLeftButtonDown(self):
		self.text.SetPackedFontColor(self.DOWN_COLOR)
		self.underline.SetPackedFontColor(self.DOWN_COLOR)
		self.underline.Show()
	
	def OnMouseLeftButtonUp(self):
		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)
		self.OnMouseOverOut()
	
	def SetEvent(self, event, *args):
		self.eventFunc = event
		self.eventArgs = args


class ComboBox(Window):

	class ListBoxWithBoard(ListBox):

		def __init__(self, layer):
			ListBox.__init__(self, layer)

		def OnRender(self):
			xRender, yRender = self.GetGlobalPosition()
			yRender -= self.TEMPORARY_PLACE
			widthRender = self.width
			heightRender = self.height + self.TEMPORARY_PLACE*2
			grp.SetColor(BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
			grp.SetColor(DARK_COLOR)
			grp.RenderLine(xRender, yRender, widthRender, 0)
			grp.RenderLine(xRender, yRender, 0, heightRender)
			grp.SetColor(BRIGHT_COLOR)
			grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
			grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

			ListBox.OnRender(self)

	def __init__(self):
		Window.__init__(self)
		self.x = 0
		self.y = 0
		self.width = 0
		self.height = 0
		self.isSelected = False
		self.isOver = False
		self.isListOpened = False
		self.event = lambda *arg: None
		self.enable = True

		self.textLine = MakeTextLine(self)
		self.textLine.SetText(localeInfo.UI_ITEM)

		self.listBox = self.ListBoxWithBoard("TOP_MOST")
		self.listBox.SetPickAlways()
		self.listBox.SetParent(self)
		self.listBox.SetEvent(__mem_func__(self.OnSelectItem))
		self.listBox.Hide()

	def __del__(self):
		Window.__del__(self)

	def Destroy(self):
		self.textLine = None
		self.listBox = None

	def SetPosition(self, x, y):
		Window.SetPosition(self, x, y)
		self.x = x
		self.y = y
		self.__ArrangeListBox()

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		self.textLine.UpdateRect()
		self.__ArrangeListBox()

	def __ArrangeListBox(self):
		self.listBox.SetPosition(0, self.height + 5)
		self.listBox.SetWidth(self.width)

	def Enable(self):
		self.enable = True

	def Disable(self):
		self.enable = False
		self.textLine.SetText("")
		self.CloseListBox()

	def SetEvent(self, event):
		self.event = event

	def ClearItem(self):
		self.CloseListBox()
		self.listBox.ClearItem()

	def InsertItem(self, index, name):
		self.listBox.InsertItem(index, name)
		self.listBox.ArrangeItem()

	def SetCurrentItem(self, text):
		self.textLine.SetText(text)

	def SelectItem(self, key):
		self.listBox.SelectItem(key)

	def OnSelectItem(self, index, name):

		self.CloseListBox()
		self.event(index)

	def CloseListBox(self):
		self.isListOpened = False
		self.listBox.Hide()

	def OnMouseLeftButtonDown(self):

		if not self.enable:
			return

		self.isSelected = True

	def OnMouseLeftButtonUp(self):

		if not self.enable:
			return

		self.isSelected = False

		if self.isListOpened:
			self.CloseListBox()
		else:
			if self.listBox.GetItemCount() > 0:
				self.isListOpened = True
				self.listBox.Show()
				self.__ArrangeListBox()

	def OnUpdate(self):

		if not self.enable:
			return

		if self.IsIn():
			self.isOver = True
		else:
			self.isOver = False

	def OnRender(self):
		self.x, self.y = self.GetGlobalPosition()
		xRender = self.x
		yRender = self.y
		widthRender = self.width
		heightRender = self.height
		grp.SetColor(BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.SetColor(BRIGHT_COLOR)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

			if self.isSelected:
				grp.SetColor(WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

if app.USE_OFFLINESHOP_SEARCH_V2:
	class AutoGrowingVerticalContainer(Window):
		def __init__(self, layer = "UI"):
			Window.__init__(self, layer)

			self.width = 0
			self.height = 0

			self.elementOffsetY = 0
			self.elementStartOffsetY = 0

			self.containerItems = []

		def __del__(self):
			self.width = 0
			self.height = 0

			self.elementOffsetY = 0
			self.elementStartOffsetY = 0

			for i in self.containerItems:
				del i

			self.containerItems = []

			Window.__del__(self)

		def SetWidth(self, width):
			self.width = width

		def SetElementOffset(self, offsetY):
			self.elementOffsetY = offsetY

		def AppendItem(self, item):
			self.height += item.GetHeight() + self.elementOffsetY
			self.containerItems.append(item)
			self.Update()

		def Update(self):
			self.SetSize(self.width, self.height)
			self.UpdateRect()

		def RecalculateHeight(self):
			self.height = 0
			for item in self.containerItems:
				self.height = self.height + item.GetHeight() + self.elementOffsetY

			self.Update()

		def GetElementByIndex(self, index):
			if index >= len(self.containerItems):
				return None

			return self.containerItems[index]

		def ClearItems(self):
			self.containerItems[:] = []
			self.RecalculateHeight()

	class AutoGrowingVerticalContainerEx(AutoGrowingVerticalContainer):
		def __init__(self):
			AutoGrowingVerticalContainer.__init__(self)

			self.elementStartOffsetY = 0
			self.elementCurrentOffsetY = 0
			self.elementOffsetX = 0

			self.oldItems = []

		def SetStartOffset(self, offset):
			self.elementStartOffsetY = offset
			self.height = offset

		def AppendItem(self, item):
			addHeight = item.GetHeight() + self.elementOffsetY
			self.height += addHeight
			self.containerItems.append(item)
			self.containerItems[-1].SetPosition(self.elementOffsetX, self.height - self.containerItems[-1].GetHeight())
			self.Update()

		def ShowItems(self):
			for item in self.containerItems:
				item.Show()

		def HideItems(self):
			for item in self.containerItems:
				item.Hide()

		def GetItems(self):
			return self.containerItems

		def RecalculateHeight(self):
			self.oldItems = self.containerItems
			self.height = self.elementStartOffsetY
			self.containerItems = []

			for item in self.oldItems:
				self.AppendItem(item)

	class ToggleAbleTitledWindow(Window):
		def __init__(self, imageFileName, imageFileNameOn, iconFileName = None):
			Window.__init__(self)

			self.imageFileName = imageFileName
			self.imageFileNameOn = imageFileNameOn

			self.baseSize = [169, 25]
			self.eventsOpenCloseActive = True

			tmp = ImageBox()
			tmp.SetEvent(__mem_func__(self.OnMouseLeftButtonDoubleClick), "mouse_click")
			tmp.SetParent(self)
			tmp.LoadImage(imageFileName)
			tmp.Show()
			self.backGroundImage = tmp

			tmp = ImageBox()
			tmp.SetParent(self.backGroundImage)
			tmp.SetEvent(__mem_func__(self.OnToggle), "mouse_click")
			tmp.SetWindowHorizontalAlignRight()
			tmp.SetPosition(18, 13)
			tmp.Show()
			self.toggleIndicator = tmp

			tmp = TextLine()
			tmp.SetParent(self.backGroundImage)
			tmp.SetPosition(48, 14)
			tmp.SetHorizontalAlignLeft()
			tmp.Show()
			self.titleText = tmp

			if iconFileName:
				tmp = ImageBox()
				tmp.SetEvent(__mem_func__(self.OnMouseLeftButtonDoubleClick), "mouse_click")
				tmp.SetParent(self.backGroundImage)
				tmp.LoadImage(iconFileName)
				tmp.SetPosition(8, 10)
				tmp.Show()
				self.iconImage = tmp
			else:
				self.iconImage = None

			tmp = AutoGrowingVerticalContainerEx()
			tmp.SetParent(self)
			tmp.SetStartOffset(0)
			tmp.SetPosition(0, self.backGroundImage.GetHeight())
			tmp.Show()
			self.toggleAbleContent = tmp

			self.eventFunc = {}
			self.eventArgs = {}

			self.toggleContentIsShown = False
			self.toggleShowFilename = ""
			self.toggleHideFilename = ""
			self.baseSize[1] = self.backGroundImage.GetHeight()
			self.width = 0

			self.haveDelay = True

		def __del__(self):
			self.eventFunc = {}
			self.eventArgs = {}

			self.imageFileName = ""
			self.imageFileNameOn = ""

			Window.__del__(self)

		def SetDelay(self, flag):
			self.haveDelay = flag

		def OnMouseLeftButtonDoubleClick(self):
			if self.haveDelay:
				dif = constInfo.changeShopSearchTime - app.GetTime()
				if dif > 0:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.V2_TRY_AGAIN_IN_MILLESECONDS % float(dif))
					return

				constInfo.changeShopSearchTime = app.GetTime() + 1

			self.OnToggle()
			return True

		def SetOpenCloseEventStatus(self, status):
			self.eventsOpenCloseActive = status

		def SetWidth(self, width):
			self.toggleAbleContent.SetWidth(width)
			self.width = width
			self.SetSize(width, self.GetHeight())

		def SetTitle(self, title):
			self.titleText.SetText(title)

		def SetbackGroundImage(self, filename):
			self.backGroundImage = ImageBox()
			self.backGroundImage.SetParent(self)
			self.backGroundImage.LoadImage(filename)
			self.backGroundImage.Show()

		def SetToggleShowFilename(self, filename):
			self.toggleShowFilename = filename
			if not self.toggleContentIsShown:
				self.toggleIndicator.LoadImage(self.toggleShowFilename)

		def SetToggleHideFilename(self, filename):
			self.toggleHideFilename = filename
			if self.toggleContentIsShown:
				self.toggleIndicator.LoadImage(self.toggleHideFilename)

		def SetOnToggleEvent(self, event, *args):
			self.eventFunc["toggle"] = event
			self.eventArgs["toggle"] = args

		def SetOnGrowEvent(self, event, *args):
			self.eventFunc["grow"] = event
			self.eventArgs["grow"] = args

		def SetOpenEvent(self, event, *args):
			self.eventFunc["open"] = event
			self.eventArgs["open"] = args

		def SetCloseEvent(self, event, *args):
			self.eventFunc["close"] = event
			self.eventArgs["close"] = args

		def AppendToToggleContent(self, item):
			self.toggleAbleContent.AppendItem(item)
			item.SetParent(self.toggleAbleContent)

			if self.eventFunc.has_key("grow"):
				apply(self.eventFunc["grow"], self.eventArgs["grow"])

			self.Update()

		def Update(self):
			if self.toggleContentIsShown:
				self.SetSize(self.baseSize[0], self.baseSize[1] + self.toggleAbleContent.GetHeight())
			else:
				self.SetSize(*self.baseSize)

			self.UpdateRect()

		def Close(self):
			self.backGroundImage.UnloadImage()
			self.backGroundImage.LoadImage(self.imageFileName)
			self.backGroundImage.SetDiffuseColor(1.0, 1.0, 1.0, 1.0)

			if len(self.toggleShowFilename) != 0:
				self.toggleIndicator.UnloadImage()
				self.toggleIndicator.LoadImage(self.toggleShowFilename)

			self.toggleAbleContent.Hide()
			self.toggleAbleContent.HideItems()

			self.toggleContentIsShown = False

			self.Update()

			if self.eventsOpenCloseActive and self.eventFunc.has_key("close"):
				apply(self.eventFunc["close"], self.eventArgs["close"])

		def Open(self):
			self.backGroundImage.UnloadImage()
			self.backGroundImage.LoadImage(self.imageFileNameOn)

			if len(self.toggleHideFilename) != 0:
				self.toggleIndicator.UnloadImage()
				self.toggleIndicator.LoadImage(self.toggleHideFilename)

			self.toggleAbleContent.Show()
			self.toggleAbleContent.ShowItems()
			self.toggleContentIsShown = True

			self.Update()

			if self.eventsOpenCloseActive and self.eventFunc.has_key("open"):
				apply(self.eventFunc["open"], self.eventArgs["open"])

		def OnToggle(self):
			self.eventsOpenCloseActive = False

			if self.toggleContentIsShown:
				self.Close()
			else:
				self.Open()

			self.eventsOpenCloseActive = True

			if self.eventFunc.has_key("toggle"):
				apply(self.eventFunc["toggle"], self.eventArgs["toggle"])

	class ScrollBarToggle(Window):
		BASE_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
		CORNERS_AND_LINES_COLOR = grp.GenerateColor(0.3411, 0.3411, 0.3411, 1.0)
		BAR_NUMB = 9

		class MiddleBar(DragButton):
			MIDDLE_BAR_COLOR = grp.GenerateColor(0.6470, 0.6470, 0.6470, 1.0)

			def __init__(self):
				DragButton.__init__(self)
				self.AddFlag("movable")
	
				self.middle = Bar()
				self.middle.SetParent(self)
				self.middle.AddFlag("attach")
				self.middle.AddFlag("not_pick")
				self.middle.SetColor(self.MIDDLE_BAR_COLOR)
				self.middle.SetSize(1, 1)
				self.middle.Show()

			def __del__(self):
				DragButton.__del__(self)

			def SetStaticScale(self, size):
				(base_width, base_height) = (self.middle.GetWidth(), self.middle.GetHeight())
				DragButton.SetSize(self, base_width, size)
				self.middle.SetSize(base_width, size)

			def SetSize(self, selfSize, fullSize):
				(base_width, base_height) = (self.middle.GetWidth(), self.middle.GetHeight())
				DragButton.SetSize(self, base_width, truediv(int(selfSize), int(fullSize)) * selfSize)
				self.middle.SetSize(base_width, truediv(int(selfSize), int(fullSize)) * selfSize)

			def SetStaticSize(self, size):
				size = max(2, size)
				DragButton.SetSize(self, size, self.middle.GetHeight())
				self.middle.SetSize(size, self.middle.GetHeight())

		def __init__(self):
			Window.__init__(self)

			self.scrollEvent = None
			self.scrollSpeed = 1
			self.sizeScale = 1.0

			self.bars = []
			for i in xrange(self.BAR_NUMB):
				br = Bar()
				br.SetParent(self)
				br.AddFlag("attach")
				br.AddFlag("not_pick")
				br.SetColor([self.CORNERS_AND_LINES_COLOR, self.BASE_COLOR][i == (self.BAR_NUMB - 1)])

				if not (i % 2 == 0):
					br.SetSize(1, 1)

				br.Show()

				self.bars.append(br)

			self.middleBar = self.MiddleBar()
			self.middleBar.SetParent(self)
			self.middleBar.SetMoveEvent(__mem_func__(self.OnScrollMove))
			self.middleBar.Show()

		def __del__(self):
			Window.__del__(self)

		def Destroy(self):
			for e in self.bars:
				del e

			self.bars = []

			if self.middleBar:
				del self.middleBar
				self.middleBar = None

		def OnScrollMove(self):
			if not self.scrollEvent:
				return

			arg = float(self.middleBar.GetLocalPosition()[1] - 1) / float(self.GetHeight() - 2 - self.middleBar.GetHeight())
			self.scrollEvent(arg)

		def ResetScrollEvent(self):
			self.scrollEvent = None

		def SetScrollEvent(self, func):
			self.scrollEvent = __mem_func__(func)

		def SetScrollSpeed(self, speed):
			self.scrollSpeed = speed

		def OnRunMouseWheel(self, length):
			if self.IsInPosition():
				val = min(max(1, self.middleBar.GetLocalPosition()[1] - (length * 0.01) * self.scrollSpeed * self.sizeScale), self.GetHeight() - self.middleBar.GetHeight() - 1)
				self.middleBar.SetPosition(1, val)

				self.OnScrollMove()
				return True

			return False

		def OnMouseLeftButtonDown(self):
			(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
			if xMouseLocalPosition == 0 or xMouseLocalPosition == self.GetWidth():
				return

			y_pos = (yMouseLocalPosition - self.middleBar.GetHeight() / 2)
			self.middleBar.SetPosition(1, y_pos)

			self.OnScrollMove()

		def SetSize(self, w, h):
			(width, height) = (max(3, w), max(3, h))

			Window.SetSize(self, width, height)

			self.bars[0].SetSize(1, (height - 2))
			self.bars[0].SetPosition(0, 1)
			self.bars[2].SetSize((width - 2), 1)
			self.bars[2].SetPosition(1, 0)
			self.bars[4].SetSize(1, (height - 2))
			self.bars[4].SetPosition((width - 1), 1)
			self.bars[6].SetSize((width - 2), 1)
			self.bars[6].SetPosition(1, (height - 1))
			self.bars[8].SetSize((width - 2), (height - 2))
			self.bars[8].SetPosition(1, 1)

			self.bars[1].SetPosition(0, 0)
			self.bars[3].SetPosition((width - 1), 0)
			self.bars[5].SetPosition((width - 1), (height - 1))
			self.bars[7].SetPosition(0, (height - 1))

			self.middleBar.SetStaticSize(width - 2)
			self.middleBar.SetSize(12, self.GetHeight())

			self.middleBar.SetRestrictMovementArea(1, 1, width - 2, height - 2)

		def SetScale(self, selfSize, fullSize):
			self.sizeScale = float(selfSize/fullSize)
			self.middleBar.SetSize(selfSize, fullSize)

		def SetStaticScale(self, r_size):
			self.middleBar.SetStaticScale(r_size)

		def SetPosScale(self, fScale):
			pos = (math.ceil((self.GetHeight() - 2 - self.middleBar.GetHeight()) * fScale) + 1)
			self.SetPos(pos)

		def SetPos(self, pos):
			wPos = (1, pos)
			self.middleBar.SetPosition(*wPos)

###################################################################################################
## Python Script Loader
###################################################################################################

class ScriptWindow(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.Children = []
		self.ElementDictionary = {}
	def __del__(self):
		Window.__del__(self)

	def ClearDictionary(self):
		self.Children = []
		self.ElementDictionary = {}
	def InsertChild(self, name, child):
		self.ElementDictionary[name] = child

	def IsChild(self, name):
		return self.ElementDictionary.has_key(name)
	def GetChild(self, name):
		return self.ElementDictionary[name]

	def GetChild2(self, name):
		return self.ElementDictionary.get(name, None)

class EditLineCentered(EditLine):
	def __init__(self):
		EditLine.__init__(self)
		self.basePos = (0, 0)

	def __del__(self):
		EditLine.__del__(self)
		del self.basePos

	def SetFocus(self):
		EditLine.SetFocus(self)
		self.AdjustTextPosition()

	def SetPosition(self, x, y):
		EditLine.SetPosition(self, x, y)
		self.basePos = (x, y)
		self.AdjustTextPosition()

	def OnIMEUpdate(self):
		EditLine.OnIMEUpdate(self)
		self.AdjustTextPosition()

	def SetText(self, text):
		EditLine.SetText(self, text)
		self.AdjustTextPosition()

	def OnMouseLeftButtonDown(self):
		if False == self.canEdit:
			return False

		if self.IsIn():
			EditLine.SetFocus(self)
			ime.SetCursorPosition(wndMgr.GetCursorPosition(self.hWnd))

	def AdjustTextPosition(self):
		(textX, textY) = EditLine.GetTextSize(self)
		(locX, locY) = self.basePos
		TextLine.SetPosition(self, locX + (self.GetWidth() / 2) - textX / 2, locY)

class PythonScriptLoader(object):

	BODY_KEY_LIST = ( "x", "y", "width", "height" )

	#####

	DEFAULT_KEY_LIST = ( "type", "x", "y", )
	WINDOW_KEY_LIST = ( "width", "height", )
	IMAGE_KEY_LIST = ( "image", )
	EXPANDED_IMAGE_KEY_LIST = ( "image", )
	ANI_IMAGE_KEY_LIST = ( "images", )
	SLOT_KEY_LIST = ( "width", "height", "slot", )
	CANDIDATE_LIST_KEY_LIST = ( "item_step", "item_xsize", "item_ysize", )
	GRID_TABLE_KEY_LIST = ( "start_index", "x_count", "y_count", "x_step", "y_step", )
	EDIT_LINE_KEY_LIST = ( "width", "height", "input_limit", )
	COMBO_BOX_KEY_LIST = ( "width", "height", "item", )
	TITLE_BAR_KEY_LIST = ( "width", )
	HORIZONTAL_BAR_KEY_LIST = ( "width", )
	BOARD_KEY_LIST = ( "width", "height", )
	BOARD_WITH_TITLEBAR_KEY_LIST = ( "width", "height", "title", )
	BOX_KEY_LIST = ( "width", "height", )
	BAR_KEY_LIST = ( "width", "height", )
	LINE_KEY_LIST = ( "width", "height", )
	SLOTBAR_KEY_LIST = ( "width", "height", )
	GAUGE_KEY_LIST = ( "width", "color", )
	SCROLLBAR_KEY_LIST = ( "size", )
	LIST_BOX_KEY_LIST = ( "width", "height", )
	MODERN_SCROLLBAR_KEY_LIST = ("size",)
	RENDER_TARGET_KEY_LIST = ( "index", )

	def __init__(self):
		self.Clear()

	def Clear(self):
		self.ScriptDictionary = { "SCREEN_WIDTH" : wndMgr.GetScreenWidth(), "SCREEN_HEIGHT" : wndMgr.GetScreenHeight() }
		self.InsertFunction = 0

	def LoadScriptFile(self, window, FileName):
		import exception
		import exceptions
		import os
		import errno
		self.Clear()

		print "===== Load Script File : %s" % (FileName)

		#ui �ڵ�� sandbox ������ ����Ǿ���Ѵ�.(���� ������ ������ �ֱ� ������)
		import sys
		from utils import Sandbox
		sandbox = Sandbox(True, ["uiScriptLocale", "localeInfo", "sys", "item", "app", "player", "grp"])

		# chr, player ���� sandbox ������ import�� ������ �ʱ� ������,(���� �ǿ��� ������ �ſ� ŭ.)
		#  �̸� script dictionary�� �ʿ��� ����� �־���´�.
		import chr
		import player
		import app
		self.ScriptDictionary["PLAYER_NAME_MAX_LEN"] = chr.PLAYER_NAME_MAX_LEN
		self.ScriptDictionary["DRAGON_SOUL_EQUIPMENT_SLOT_START"] = player.DRAGON_SOUL_EQUIPMENT_SLOT_START
		self.ScriptDictionary["LOCALE_PATH"] = app.GetLocalePath()
		self.ScriptDictionary["__AUTO_QUQUE_ATTACK__"] = app.__AUTO_QUQUE_ATTACK__

		if app.USE_CRYSTAL_SYSTEM:
			self.ScriptDictionary["SHARD_CRAFT_SLOT_MAIN"] = player.SHARD_CRAFT_SLOT_MAIN
			self.ScriptDictionary["SHARD_CRAFT_SLOT_MATERIAL"] = player.SHARD_CRAFT_SLOT_MATERIAL
			self.ScriptDictionary["SHARD_CRAFT_SLOT_RESULT"] = player.SHARD_CRAFT_SLOT_RESULT

		self.ScriptDictionary["USE_PREMIUM_AFFECT"] = app.USE_PREMIUM_AFFECT

		try:
			sandbox.execfile(FileName, self.ScriptDictionary)
		except IOError, err:
			import sys
			import dbg
			dbg.TraceError("Failed to load script file : %s" % (FileName))
			dbg.TraceError("error  : %s" % (err))
			exception.Abort("LoadScriptFile1")
		except RuntimeError,err:
			import sys
			import dbg
			dbg.TraceError("Failed to load script file : %s" % (FileName))
			dbg.TraceError("error  : %s" % (err))
			exception.Abort("LoadScriptFile2")
		except:
			import sys
			import dbg
			dbg.TraceError("Failed to load script file : %s" % (FileName))
			exception.Abort("LoadScriptFile!!!!!!!!!!!!!!")

		#####

		Body = self.ScriptDictionary["window"]
		self.CheckKeyList("window", Body, self.BODY_KEY_LIST)

		window.ClearDictionary()
		self.InsertFunction = window.InsertChild

		window.SetPosition(int(Body["x"]), int(Body["y"]))

		if localeInfo.IsARABIC():
			w = wndMgr.GetScreenWidth()
			h = wndMgr.GetScreenHeight()
			if Body.has_key("width"):
				w = int(Body["width"])
			if Body.has_key("height"):
				h = int(Body["height"])

			window.SetSize(w, h)
		else:
			window.SetSize(int(Body["width"]), int(Body["height"]))
			if True == Body.has_key("style"):
				for StyleList in Body["style"]:
					window.AddFlag(StyleList)


		self.LoadChildren(window, Body)

	def LoadChildren(self, parent, dicChildren):

		if localeInfo.IsARABIC():
			parent.AddFlag( "rtl" )

		if True == dicChildren.has_key("style"):
			for style in dicChildren["style"]:
				parent.AddFlag(style)

		if False == dicChildren.has_key("children"):
			return False

		Index = 0

		ChildrenList = dicChildren["children"]
		parent.Children = range(len(ChildrenList))
		for ElementValue in ChildrenList:
			try:
				Name = ElementValue["name"]
			except KeyError:
				Name = ElementValue["name"] = "NONAME"

			try:
				Type = ElementValue["type"]
			except KeyError:
				Type = ElementValue["type"] = "window"

			if False == self.CheckKeyList(Name, ElementValue, self.DEFAULT_KEY_LIST):
				del parent.Children[Index]
				continue

			if Type == "window":
				parent.Children[Index] = ScriptWindow()
				parent.Children[Index].SetParent(parent)
				self.LoadElementWindow(parent.Children[Index], ElementValue, parent)

			elif Type == "button":
				parent.Children[Index] = Button()
				parent.Children[Index].SetParent(parent)
				self.LoadElementButton(parent.Children[Index], ElementValue, parent)

			elif Type == "render_target":	
				parent.Children[Index] = RenderTarget()
				parent.Children[Index].SetParent(parent)
				self.LoadElementRenderTarget(parent.Children[Index], ElementValue, parent)

			elif Type == "radio_button":
				parent.Children[Index] = RadioButton()
				parent.Children[Index].SetParent(parent)
				self.LoadElementButton(parent.Children[Index], ElementValue, parent)

			elif Type == "toggle_button":
				parent.Children[Index] = ToggleButton()
				parent.Children[Index].SetParent(parent)
				self.LoadElementButton(parent.Children[Index], ElementValue, parent)

			elif Type == "mark":
				parent.Children[Index] = MarkBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementMark(parent.Children[Index], ElementValue, parent)

			elif Type == "image":
				parent.Children[Index] = ImageBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementImage(parent.Children[Index], ElementValue, parent)

			elif Type == "special_image":
				parent.Children[Index] = ImageBoxSungMahi()
				parent.Children[Index].SetParent(parent)
				self.LoadElementImage(parent.Children[Index], ElementValue, parent)

			elif Type == "expanded_image":
				parent.Children[Index] = ExpandedImageBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementExpandedImage(parent.Children[Index], ElementValue, parent)

			elif Type == "ani_image":
				parent.Children[Index] = AniImageBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementAniImage(parent.Children[Index], ElementValue, parent)

			elif Type == "slot":
				parent.Children[Index] = SlotWindow()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSlot(parent.Children[Index], ElementValue, parent)

			elif Type == "candidate_list":
				parent.Children[Index] = CandidateListBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementCandidateList(parent.Children[Index], ElementValue, parent)

			elif Type == "grid_table":
				parent.Children[Index] = GridSlotWindow()
				parent.Children[Index].SetParent(parent)
				self.LoadElementGridTable(parent.Children[Index], ElementValue, parent)
		
			elif Type == "textlink":
				parent.Children[Index] = TextLink()
				parent.Children[Index].SetParent(parent)
				self.LoadElementTextlink(parent.Children[Index], ElementValue, parent)

			elif Type == "text":
				parent.Children[Index] = TextLine()
				parent.Children[Index].SetParent(parent)
				self.LoadElementText(parent.Children[Index], ElementValue, parent)

			elif Type == "editline":
				parent.Children[Index] = EditLine()
				parent.Children[Index].SetParent(parent)
				self.LoadElementEditLine(parent.Children[Index], ElementValue, parent)

			elif Type == "multi_text":
				parent.Children[Index] = MultiTextLineNew()
				parent.Children[Index].SetParent(parent)
				self.LoadElementText(parent.Children[Index], ElementValue, parent)
			elif Type == "listbox_new":
				parent.Children[Index] = ListBoxNew()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBox(parent.Children[Index], ElementValue, parent)

			elif Type == "titlebar":
				parent.Children[Index] = TitleBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementTitleBar(parent.Children[Index], ElementValue, parent)

			elif Type == "horizontalbar":
				parent.Children[Index] = HorizontalBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementHorizontalBar(parent.Children[Index], ElementValue, parent)

			elif Type == "board":
				parent.Children[Index] = Board()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "board_with_titlebar":
				parent.Children[Index] = BoardWithTitleBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoardWithTitleBar(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard":
				parent.Children[Index] = ThinBoard()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			elif app.USE_OFFLINESHOP_SEARCH_V2 and Type == "v2_thinboard":
				parent.Children[Index] = ThinBoard("UI", 2)
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			elif app.USE_OFFLINESHOP_SEARCH_V2 and Type == "v3_thinboard":
				parent.Children[Index] = ThinBoard("UI", 3)
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "scrollbar_new" and app.ENABLE_SWITCHBOT_WORLDARD:
				parent.Children[Index] = ScrollBarNew1()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)


			elif Type == "scrollbar_new": # offshop scrollbar.
				parent.Children[Index] = ScrollBarNew()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)
	
			elif Type == "thinboard_gold":
				parent.Children[Index] = ThinBoardGold()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoardGold(parent.Children[Index], ElementValue, parent)
				
			elif Type == "box":
				parent.Children[Index] = Box()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBox(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard_circle":
				parent.Children[Index] = ThinBoardCircle()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
				
			elif Type == "bar":
				parent.Children[Index] = Bar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBar(parent.Children[Index], ElementValue, parent)

			elif Type == "line":
				parent.Children[Index] = Line()
				parent.Children[Index].SetParent(parent)
				self.LoadElementLine(parent.Children[Index], ElementValue, parent)

			elif Type == "slotbar":
				parent.Children[Index] = SlotBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSlotBar(parent.Children[Index], ElementValue, parent)

			elif Type == "gauge":
				parent.Children[Index] = Gauge()
				parent.Children[Index].SetParent(parent)
				self.LoadElementGauge(parent.Children[Index], ElementValue, parent)

			elif Type == "scrollbar":
				parent.Children[Index] = ScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)


			elif Type == "scrollbar_new":
				parent.Children[Index] = ScrollBarNew()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBarNew(parent.Children[Index], ElementValue, parent)

			elif Type == "scrollbar_ex" and app.ENABLE_DUNGEON_INFO:
				parent.Children[Index] = ScrollBarEx()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBarNew(parent.Children[Index], ElementValue, parent)

			elif Type == "scrollbar_itemshop":
				parent.Children[Index] = ScrollBarItemShop()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)

			
			elif Type == "thin_scrollbar":
				parent.Children[Index] = ThinScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)

			elif Type == "small_thin_scrollbar":
				parent.Children[Index] = SmallThinScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)

			elif Type == "sliderbar":
				parent.Children[Index] = SliderBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSliderBar(parent.Children[Index], ElementValue, parent)

			elif Type == "listbox":
				parent.Children[Index] = ListBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBox(parent.Children[Index], ElementValue, parent)

			elif Type == "resizable_text_value":
				parent.Children[Index] = ResizableTextValue()
				parent.Children[Index].SetParent(parent)
				self.LoadElementResizableTextValue(parent.Children[Index], ElementValue, parent)

			elif Type == "modern_scrollbar":
				parent.Children[Index] = ModernScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementModernScrollBar(parent.Children[Index], ElementValue, parent)\

			elif Type == "resizable_button_with_image":
				parent.Children[Index] = ResizableButtonWithImage()
				parent.Children[Index].SetParent(parent)
				self.LoadElementResizableButtonWithImage(parent.Children[Index], ElementValue, parent)
				
			elif Type == "listbox2":
				parent.Children[Index] = ListBox2()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBox2(parent.Children[Index], ElementValue, parent)
			elif Type == "listboxex":
				parent.Children[Index] = ListBoxEx()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBoxEx(parent.Children[Index], ElementValue, parent)

			elif Type == "start_dungeon":
				parent.Children[Index] = StartDungeon()
				parent.Children[Index].SetParent(parent)
				self.LoadElementStartDungeon(parent.Children[Index], ElementValue, parent)

			elif Type == "editline_centered":
				parent.Children[Index] = EditLineCentered()
				parent.Children[Index].SetParent(parent)
				self.LoadElementEditLine(parent.Children[Index], ElementValue, parent)
			else:
				Index += 1
				continue

			parent.Children[Index].SetWindowName(Name)
			if 0 != self.InsertFunction:
				self.InsertFunction(Name, parent.Children[Index])

			self.LoadChildren(parent.Children[Index], ElementValue)
			Index += 1

	def CheckKeyList(self, name, value, key_list):

		for DataKey in key_list:
			if False == value.has_key(DataKey):
				print "Failed to find data key", "[" + name + "/" + DataKey + "]"
				return False

		return True

	def LoadDefaultData(self, window, value, parentWindow):
		loc_x = int(value["x"])
		loc_y = int(value["y"])
		if value.has_key("vertical_align"):
			if "center" == value["vertical_align"]:
				window.SetWindowVerticalAlignCenter()
			elif "bottom" == value["vertical_align"]:
				window.SetWindowVerticalAlignBottom()

		if parentWindow.IsRTL():
			loc_x = int(value["x"]) + window.GetWidth()
			if value.has_key("horizontal_align"):
				if "center" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignCenter()
					loc_x = - int(value["x"])
				elif "right" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignLeft()
					loc_x = int(value["x"]) - window.GetWidth()
					## loc_x = parentWindow.GetWidth() - int(value["x"]) + window.GetWidth()
			else:
				window.SetWindowHorizontalAlignRight()

			if value.has_key("all_align"):
				window.SetWindowVerticalAlignCenter()
				window.SetWindowHorizontalAlignCenter()
				loc_x = - int(value["x"])
		else:
			if value.has_key("horizontal_align"):
				if "center" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignCenter()
				elif "right" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignRight()

		window.SetPosition(loc_x, loc_y)
		window.Show()

	## Window
	def LoadElementWindow(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.WINDOW_KEY_LIST):
			return False

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	def LoadElementModernScrollBar(self, window, value, parentWindow):
		if not self.CheckKeyList(value["name"], value, self.MODERN_SCROLLBAR_KEY_LIST):
			return False

		window.SetScrollBarSize(value["size"])

		if value.has_key("content_height"):
			window.SetContentHeight(value["content_height"])

		if value.has_key("width"):
			window.SetWidth(value["width"])

		self.LoadDefaultData(window, value, parentWindow)
		return True

	## Button
	def LoadElementButton(self, window, value, parentWindow):

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(int(value["width"]), int(value["height"]))

		if True == value.has_key("default_image"):
			window.SetUpVisual(value["default_image"])
		if True == value.has_key("over_image"):
			window.SetOverVisual(value["over_image"])
		if True == value.has_key("down_image"):
			window.SetDownVisual(value["down_image"])
		if True == value.has_key("disable_image"):
			window.SetDisableVisual(value["disable_image"])

		if True == value.has_key("text"):
			if True == value.has_key("text_height"):
				window.SetText(value["text"], value["text_height"])
			elif TRUE == value.has_key("text_x") and app.ENABLE_NEW_GAMEOPTION:
				window.SetListText(value["text"], value["text_x"])
			else:
				window.SetText(value["text"])

			if value.has_key("text_color"):
				window.SetTextColor(value["text_color"])

		if True == value.has_key("tooltip_text"):
			if True == value.has_key("tooltip_x") and True == value.has_key("tooltip_y"):
				window.SetToolTipText(value["tooltip_text"], int(value["tooltip_x"]), int(value["tooltip_y"]))
			else:
				window.SetToolTipText(value["tooltip_text"])

		if app.USE_CHEQUE_CURRENCY:
			if value.has_key("x_scale") == True and value.has_key("y_scale") == True:
				window.SetButtonScale(float(value["x_scale"]), float(value["y_scale"]))
			elif value.has_key("x_scale") == True:
				window.SetButtonScale(float(value["x_scale"]), 1.0)
			elif value.has_key("y_scale") == True:
				window.SetButtonScale(1.0, float(value["y_scale"]))

		self.LoadDefaultData(window, value, parentWindow)

		return True
	def LoadElementResizableTextValue(self, window, value, parentWindow):

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(int(value["width"]), int(value["height"]))

		if TRUE == value.has_key("text"):
			window.SetText(value["text"])
			
		if value.has_key("line_color"):
			window.SetLineColor(value["line_color"])
			
		if value.has_key("color"):
			window.SetBackgroundColor(value["color"])
			
		if value.has_key("line_top"):
			window.SetLine('top')
		if value.has_key("line_bottom"):
			window.SetLine('bottom')
		if value.has_key("line_left"):
			window.SetLine('left')
		if value.has_key("line_right"):
			window.SetLine('right')
			
		if value.has_key('all_lines'):
			window.SetLine('top')
			window.SetLine('bottom')
			window.SetLine('left')
			window.SetLine('right')
			
		if value.has_key('without_background'):
			window.SetNoBackground()
			
		if value.has_key("text"):
			window.SetText(value["text"])

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE
		
	def LoadElementResizableButtonWithImage(self, window, value, parentWindow):

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(int(value["width"]), int(value["height"]))

		if TRUE == value.has_key("text"):
			window.SetText(value["text"])
			
		if TRUE == value.has_key("text_x") and value.has_key("text_y"):
			if value.has_key("text_align"):
				window.SetTextPosition(int(value["text_x"]), int(value["text_y"]), TRUE)
			else:
				window.SetTextPosition(int(value["text_x"]), int(value["text_y"]))

		if value.has_key("text_color"):
			window.SetTextColor(value["text_color"])

		if TRUE == value.has_key("tooltip_text"):
			window.SetToolTipText(value["tooltip_text"])
			
		if value.has_key("image"):
			window.SetImage(value["image"])

		self.LoadDefaultData(window, value, parentWindow)

	## Mark
	def LoadElementMark(self, window, value, parentWindow):

		#if False == self.CheckKeyList(value["name"], value, self.MARK_KEY_LIST):
		#	return False

		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Image
	def LoadElementImage(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.IMAGE_KEY_LIST):
			return False

		window.LoadImage(value["image"])
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## AniImage
	def LoadElementAniImage(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.ANI_IMAGE_KEY_LIST):
			return False

		if True == value.has_key("delay"):
			window.SetDelay(value["delay"])

		if True == value.has_key("x_scale") and True == value.has_key("y_scale"):
			for image in value["images"]:
				window.AppendImageScale(image, float(value["x_scale"]), float(value["y_scale"]))
		else:
			for image in value["images"]:
				window.AppendImage(image)

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(value["width"], value["height"])

		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Expanded Image
	def LoadElementExpandedImage(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.EXPANDED_IMAGE_KEY_LIST):
			return False

		window.LoadImage(value["image"])

		if True == value.has_key("x_origin") and True == value.has_key("y_origin"):
			window.SetOrigin(float(value["x_origin"]), float(value["y_origin"]))

		if True == value.has_key("x_scale") and True == value.has_key("y_scale"):
			window.SetScale(float(value["x_scale"]), float(value["y_scale"]))

		if True == value.has_key("rect"):
			RenderingRect = value["rect"]
			window.SetRenderingRect(RenderingRect[0], RenderingRect[1], RenderingRect[2], RenderingRect[3])

		if True == value.has_key("mode"):
			mode = value["mode"]
			if "MODULATE" == mode:
				window.SetRenderingMode(wndMgr.RENDERING_MODE_MODULATE)

		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Slot
	def LoadElementSlot(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.SLOT_KEY_LIST):
			return False

		global_x = int(value["x"])
		global_y = int(value["y"])
		global_width = int(value["width"])
		global_height = int(value["height"])

		window.SetPosition(global_x, global_y)
		window.SetSize(global_width, global_height)
		window.Show()

		r = 1.0
		g = 1.0
		b = 1.0
		a = 1.0

		if True == value.has_key("image_r") and \
			True == value.has_key("image_g") and \
			True == value.has_key("image_b") and \
			True == value.has_key("image_a"):
			r = float(value["image_r"])
			g = float(value["image_g"])
			b = float(value["image_b"])
			a = float(value["image_a"])

		SLOT_ONE_KEY_LIST = ("index", "x", "y", "width", "height")

		for slot in value["slot"]:
			if True == self.CheckKeyList(value["name"] + " - one", slot, SLOT_ONE_KEY_LIST):
				wndMgr.AppendSlot(window.hWnd,
									int(slot["index"]),
									int(slot["x"]),
									int(slot["y"]),
									int(slot["width"]),
									int(slot["height"]))

		if True == value.has_key("image"):
			if True == value.has_key("x_scale") and TRUE == value.has_key("y_scale"):
				wndMgr.SetSlotBaseImageScale(window.hWnd,
										value["image"],
										r, g, b, a, float(value["x_scale"]), float(value["y_scale"]))
			else:
				wndMgr.SetSlotBaseImage(window.hWnd,
										value["image"],
										r, g, b, a)

		return True

	def LoadElementCandidateList(self, window, value, parentWindow):
		if False == self.CheckKeyList(value["name"], value, self.CANDIDATE_LIST_KEY_LIST):
			return False

		window.SetPosition(int(value["x"]), int(value["y"]))
		window.SetItemSize(int(value["item_xsize"]), int(value["item_ysize"]))
		window.SetItemStep(int(value["item_step"]))
		window.Show()

		return True

	## Table
	def LoadElementGridTable(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.GRID_TABLE_KEY_LIST):
			return False

		xBlank = 0
		yBlank = 0
		if True == value.has_key("x_blank"):
			xBlank = int(value["x_blank"])
		if True == value.has_key("y_blank"):
			yBlank = int(value["y_blank"])

		if localeInfo.IsARABIC():
			pass
		else:
			window.SetPosition(int(value["x"]), int(value["y"]))

		window.ArrangeSlot(	int(value["start_index"]),
							int(value["x_count"]),
							int(value["y_count"]),
							int(value["x_step"]),
							int(value["y_step"]),
							xBlank,
							yBlank)
		if True == value.has_key("image"):
			r = 1.0
			g = 1.0
			b = 1.0
			a = 1.0
			if True == value.has_key("image_r") and \
				True == value.has_key("image_g") and \
				True == value.has_key("image_b") and \
				True == value.has_key("image_a"):
				r = float(value["image_r"])
				g = float(value["image_g"])
				b = float(value["image_b"])
				a = float(value["image_a"])
			wndMgr.SetSlotBaseImage(window.hWnd, value["image"], r, g, b, a)

		if True == value.has_key("style"):
			if "select" == value["style"]:
				wndMgr.SetSlotStyle(window.hWnd, wndMgr.SLOT_STYLE_SELECT)
		if localeInfo.IsARABIC():
			self.LoadDefaultData(window, value, parentWindow)
		else:
			window.Show()

		return True
		
	def LoadElementTextlink(self, window, value, parentWindow):

		if value.has_key("all_align"):
			window.SetWindowHorizontalAlignCenter()
			window.SetWindowVerticalAlignCenter()

		if True == value.has_key("text"):
			window.SetText(value["text"])
			
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Text
	def LoadElementText(self, window, value, parentWindow):

		if value.has_key("fontsize"):
			fontSize = value["fontsize"]

			if "LARGE" == fontSize:
				window.SetFontName(localeInfo.UI_DEF_FONT_LARGE)

		elif value.has_key("fontname"):
			fontName = value["fontname"]
			window.SetFontName(fontName)

		if value.has_key("text_horizontal_align"):
			if "left" == value["text_horizontal_align"]:
				window.SetHorizontalAlignLeft()
			elif "center" == value["text_horizontal_align"]:
				window.SetHorizontalAlignCenter()
			elif "right" == value["text_horizontal_align"]:
				window.SetHorizontalAlignRight()

		if value.has_key("text_vertical_align"):
			if "top" == value["text_vertical_align"]:
				window.SetVerticalAlignTop()
			elif "center" == value["text_vertical_align"]:
				window.SetVerticalAlignCenter()
			elif "bottom" == value["text_vertical_align"]:
				window.SetVerticalAlignBottom()

		if value.has_key("all_align"):
			window.SetHorizontalAlignCenter()
			window.SetVerticalAlignCenter()
			window.SetWindowHorizontalAlignCenter()
			window.SetWindowVerticalAlignCenter()

		if value.has_key("r") and value.has_key("g") and value.has_key("b"):
			window.SetFontColor(float(value["r"]), float(value["g"]), float(value["b"]))
		elif value.has_key("color"):
			window.SetPackedFontColor(value["color"])
		else:
			window.SetFontColor(0.8549, 0.8549, 0.8549)

		if TRUE == value.has_key("text_range"):
			window.SetTextRange(value["text_range"])
		if TRUE == value.has_key("text_type"):
			window.SetTextType(value["text_type"])
		if value.has_key("outline"):
			if value["outline"]:
				window.SetOutline()
		if True == value.has_key("text"):
			window.SetText(value["text"])

		self.LoadDefaultData(window, value, parentWindow)

		return True

	## EditLine
	def LoadElementEditLine(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.EDIT_LINE_KEY_LIST):
			return False


		if value.has_key("secret_flag"):
			window.SetSecret(value["secret_flag"])
		if value.has_key("with_codepage"):
			if value["with_codepage"]:
				window.bCodePage = True
		if value.has_key("only_number"):
			if value["only_number"]:
				window.SetNumberMode()
		if value.has_key("enable_codepage"):
			window.SetIMEFlag(value["enable_codepage"])
		if value.has_key("enable_ime"):
			window.SetIMEFlag(value["enable_ime"])
		if value.has_key("limit_width"):
			window.SetLimitWidth(value["limit_width"])
		if value.has_key("multi_line"):
			if value["multi_line"]:
				window.SetMultiLine()
		if value.has_key("info_msg"):
			window.SetInfoMessage(value["info_msg"])

		window.SetMax(int(value["input_limit"]))
		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadElementText(window, value, parentWindow)

		return True

	def LoadElementRenderTarget(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.RENDER_TARGET_KEY_LIST):
			return False

		window.SetSize(value["width"], value["height"])
		
		if True == value.has_key("style"):
			for style in value["style"]:
				window.AddFlag(style)
				
		self.LoadDefaultData(window, value, parentWindow)
		
		if value.has_key("index"):
			window.SetRenderTarget(int(value["index"]))

		return True

	## TitleBar
	def LoadElementTitleBar(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.TITLE_BAR_KEY_LIST):
			return False

		window.MakeTitleBar(int(value["width"]), value.get("color", "red"))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## HorizontalBar
	def LoadElementHorizontalBar(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.HORIZONTAL_BAR_KEY_LIST):
			return False

		window.Create(int(value["width"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Board
	def LoadElementBoard(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return False

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Board With TitleBar
	def LoadElementBoardWithTitleBar(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.BOARD_WITH_TITLEBAR_KEY_LIST):
			return False

		window.SetSize(int(value["width"]), int(value["height"]))
		window.SetTitleName(value["title"])
		self.LoadDefaultData(window, value, parentWindow)

		return True
	def LoadElementThinBoardGold(self, window, value, parentWindow):
		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE
		
		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)
		return TRUE

	def LoadElementThinBoardCircle(self, window, value, parentWindow):
		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE
		
		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)
		return TRUE
	## ThinBoard
	def LoadElementThinBoard(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return False

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Box
	def LoadElementBox(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.BOX_KEY_LIST):
			return False

		if True == value.has_key("color"):
			window.SetColor(value["color"])

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Bar
	def LoadElementBar(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.BAR_KEY_LIST):
			return False

		if True == value.has_key("color"):
			window.SetColor(value["color"])

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Line
	def LoadElementLine(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.LINE_KEY_LIST):
			return False

		if True == value.has_key("color"):
			window.SetColor(value["color"])

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Slot
	def LoadElementSlotBar(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.SLOTBAR_KEY_LIST):
			return False

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## Gauge
	def LoadElementGauge(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.GAUGE_KEY_LIST):
			return False

		window.MakeGauge(value["width"], value["color"])
		self.LoadDefaultData(window, value, parentWindow)

		return True

	def LoadElementScrollBarNew(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.SCROLLBAR_KEY_LIST):
			return False

		window.SetScrollBarSize(value["size"])
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## ScrollBar
	def LoadElementScrollBar(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.SCROLLBAR_KEY_LIST):
			return False

		window.SetScrollBarSize(value["size"])
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## SliderBar
	def LoadElementSliderBar(self, window, value, parentWindow):

		self.LoadDefaultData(window, value, parentWindow)

		return True

	## ListBox
	def LoadElementListBox(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.LIST_BOX_KEY_LIST):
			return False

		if value.has_key("item_align"):
			window.SetTextCenterAlign(value["item_align"])

		window.SetSize(value["width"], value["height"])
		self.LoadDefaultData(window, value, parentWindow)

		return True

	## ListBox2
	def LoadElementListBox2(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.LIST_BOX_KEY_LIST):
			return False

		window.SetRowCount(value.get("row_count", 10))
		window.SetSize(value["width"], value["height"])
		self.LoadDefaultData(window, value, parentWindow)

		if value.has_key("item_align"):
			window.SetTextCenterAlign(value["item_align"])

		return True
	def LoadElementListBoxEx(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.LIST_BOX_KEY_LIST):
			return False

		window.SetSize(value["width"], value["height"])
		self.LoadDefaultData(window, value, parentWindow)

		if value.has_key("itemsize_x") and value.has_key("itemsize_y"):
			window.SetItemSize(int(value["itemsize_x"]), int(value["itemsize_y"]))

		if value.has_key("itemstep"):
			window.SetItemStep(int(value["itemstep"]))

		if value.has_key("viewcount"):
			window.SetViewItemCount(int(value["viewcount"]))

		return True

	def LoadElementStartDungeon(self, window, value, parentWindow):
		self.LoadDefaultData(window, value, parentWindow)

		return True

class ReadingWnd(Bar):

	def __init__(self):
		Bar.__init__(self,"TOP_MOST")

		self.__BuildText()
		self.SetSize(80, 19)
		self.Show()

	def __del__(self):
		Bar.__del__(self)

	def __BuildText(self):
		self.text = TextLine()
		self.text.SetParent(self)
		self.text.SetPosition(4, 3)
		self.text.Show()

	def SetText(self, text):
		self.text.SetText(text)

	def SetReadingPosition(self, x, y):
		xPos = x + 2
		yPos = y  - self.GetHeight() - 2
		self.SetPosition(xPos, yPos)

	def SetTextColor(self, color):
		self.text.SetPackedFontColor(color)


def MakeImageBoxNew(parent, name, x, y):
	image = ImageBox()
	image.SetParent(parent)
	image.LoadImage(name)
	image.SetPosition(x, y)
	image.Show()
	return image


def MakeSlotBar(parent, x, y, width, height):
	slotBar = SlotBar()
	slotBar.SetParent(parent)
	slotBar.SetSize(width, height)
	slotBar.SetPosition(x, y)
	slotBar.Show()
	return slotBar

def MakeImageBox(parent, name, x, y):
	image = ImageBox()
	image.SetParent(parent)
	image.LoadImage(name)
	image.SetPosition(x, y)
	image.Show()
	return image

if app.ENABLE_MINI_GAME_CATCH_KING:
	def MakeImageBox2(parent, name, x, y):
		image = ImageBox2()
		image.SetParent(parent)
		image.LoadImage(name)
		image.SetPosition(x, y)
		image.Show()
		return image

def MakeTextLine(parent,x=0,y=0):
	textLine = TextLine()
	textLine.SetParent(parent)
	textLine.SetWindowHorizontalAlignCenter()
	textLine.SetWindowVerticalAlignCenter()
	textLine.SetHorizontalAlignCenter()
	textLine.SetVerticalAlignCenter()
	if x > 0 and y > 0:
		textLine.SetPosition(x,y)

	textLine.Show()
	return textLine


if app.ENABLE_QUEST_PAGE_SCROLLING:	
	def MakeTextLineNew(parent, x, y, text, fontType = None):
		textLine = TextLine()
		textLine.SetParent(parent)
		textLine.SetPosition(x, y)
		textLine.SetText(text)
		if fontType != None:
			textLine.SetFontName(fontType)
		textLine.Show()
		return textLine


def MakeButton(parent, x, y, tooltipText, path, up, over, down):
	button = Button()
	button.SetParent(parent)
	button.SetPosition(x, y)
	button.SetUpVisual(path + up)
	button.SetOverVisual(path + over)
	button.SetDownVisual(path + down)
	button.SetToolTipText(tooltipText)
	button.Show()
	return button

if app.USE_OFFLINESHOP_SEARCH_V2:
	def MakeRadioButton(parent, x, y, text, tooltipText, path, up, over, down):
		button = RadioButton()
		button.SetParent(parent)
		button.SetPosition(x, y)
		button.SetUpVisual(path + up)
		button.SetOverVisual(path + over)
		button.SetDownVisual(path + down)
		button.SetText(text)
		button.SetToolTipText(tooltipText)
		button.Show()
		return button

	def MakeToggleButton(parent, x, y, text, tooltipText, path, up, over, down):
		button = ToggleButton()
		button.SetParent(parent)
		button.SetPosition(x, y)
		button.SetUpVisual(path + up)
		button.SetOverVisual(path + over)
		button.SetDownVisual(path + down)
		button.SetText(text)
		button.SetToolTipText(tooltipText)
		button.Show()
		return button

def RenderRoundBox(x, y, width, height, color):
	grp.SetColor(color)
	grp.RenderLine(x+2, y, width-3, 0)
	grp.RenderLine(x+2, y+height, width-3, 0)
	grp.RenderLine(x, y+2, 0, height-4)
	grp.RenderLine(x+width, y+1, 0, height-3)
	grp.RenderLine(x, y+2, 2, -2)
	grp.RenderLine(x, y+height-2, 2, 2)
	grp.RenderLine(x+width-2, y, 2, 2)
	grp.RenderLine(x+width-2, y+height, 2, -2)

def GenerateColor(r, g, b):
	r = float(r) / 255.0
	g = float(g) / 255.0
	b = float(b) / 255.0
	return grp.GenerateColor(r, g, b, 1.0)

def EnablePaste(flag):
	ime.EnablePaste(flag)

def GetHyperlink():
	return wndMgr.GetHyperlink()

class ComboBoxImage(Window):
	class ListBoxWithBoard(ListBox):

		def __init__(self, layer):
			ListBox.__init__(self, layer)

		def OnRender(self):
			xRender, yRender = self.GetGlobalPosition()
			yRender -= self.TEMPORARY_PLACE
			widthRender = self.width
			heightRender = self.height + self.TEMPORARY_PLACE*2
			grp.SetColor(BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
			grp.SetColor(DARK_COLOR)
			grp.RenderLine(xRender, yRender, widthRender, 0)
			grp.RenderLine(xRender, yRender, 0, heightRender)
			ListBox.OnRender(self)

	def __init__(self, parent, name, x ,y):
		Window.__init__(self)
		self.isSelected = False
		self.isOver = False
		self.isListOpened = False
		self.event = lambda *arg: None
		self.enable = True
		self.imagebox = None
		
		## imagebox
		image = ImageBox()
		image.SetParent(parent)
		image.LoadImage(name)
		image.SetPosition(x, y)
		image.Show()
		self.imagebox = image
		
		## BaseSetting
		self.x = x + 1
		self.y = y + 1
		self.width = self.imagebox.GetWidth() - 3
		self.height = self.imagebox.GetHeight() - 3
		self.SetParent(parent)

		## TextLine
		self.textLine = MakeTextLine(self)
		self.textLine.SetText(localeInfo.UI_ITEM)
		
		## ListBox
		self.listBox = self.ListBoxWithBoard("TOP_MOST")
		self.listBox.SetPickAlways()
		self.listBox.SetParent(self)
		self.listBox.SetEvent(__mem_func__(self.OnSelectItem))
		self.listBox.Hide()

		Window.SetPosition(self, self.x, self.y)
		Window.SetSize(self, self.width, self.height)
		self.textLine.UpdateRect()
		self.__ArrangeListBox()
		
	def __del__(self):
		Window.__del__(self)

	def Destroy(self):
		self.textLine = None
		self.listBox = None
		self.imagebox = None

	def SetPosition(self, x, y):
		Window.SetPosition(self, x, y)
		self.imagebox.SetPosition(x, y)
		self.x = x
		self.y = y
		self.__ArrangeListBox()

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		self.textLine.UpdateRect()
		self.__ArrangeListBox()

	def __ArrangeListBox(self):
		self.listBox.SetPosition(0, self.height + 5)
		self.listBox.SetWidth(self.width)

	def Enable(self):
		self.enable = True

	def Disable(self):
		self.enable = False
		self.textLine.SetText("")
		self.CloseListBox()

	def SetEvent(self, event):
		self.event = event

	def ClearItem(self):
		self.CloseListBox()
		self.listBox.ClearItem()

	def InsertItem(self, index, name):
		self.listBox.InsertItem(index, name)
		self.listBox.ArrangeItem()

	def SetCurrentItem(self, text):
		self.textLine.SetText(text)

	def SelectItem(self, key):
		self.listBox.SelectItem(key)

	def OnSelectItem(self, index, name):
		self.CloseListBox()
		self.event(index)

	def CloseListBox(self):
		self.isListOpened = False
		self.listBox.Hide()

	def OnMouseLeftButtonDown(self):
	
		if not self.enable:
			return

		self.isSelected = True

	def OnMouseLeftButtonUp(self):
		if not self.enable:
			return
		
		self.isSelected = False
		
		if self.isListOpened:
			self.CloseListBox()
		else:
			if self.listBox.GetItemCount() > 0:
				self.isListOpened = True
				self.listBox.Show()
				self.__ArrangeListBox()

	def OnUpdate(self):

		if not self.enable:
			return

		if self.IsIn():
			self.isOver = True
		else:
			self.isOver = False

	def OnRender(self):
		self.x, self.y = self.GetGlobalPosition()
		xRender = self.x
		yRender = self.y
		widthRender = self.width
		heightRender = self.height
		if self.isOver:
			grp.SetColor(HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)
			if self.isSelected:
				grp.SetColor(WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

def calculateRect(curValue, maxValue):
	try:
		return -1.0 + float(curValue) / float(maxValue)
	except:
		return 0.0

class ListBoxNew(Window):
	def __del__(self):
		Window.__del__(self)
	def Destroy(self):
		for item in self.itemList:
			item.Destroy()
		self.itemList=[]
		self.scrollBar=None
		self.basePos=0
		self.scrollLen=0
		self.scrollLenExtra=0
		self.isHorizontal= 0
	def __init__(self, isHorizontal = False):
		Window.__init__(self)
		self.itemList=[]
		self.Destroy()
		self.isHorizontal= isHorizontal
	def RemoveAllItems(self):
		for item in self.itemList:
			item.Destroy()
		self.itemList=[]
		if self.scrollBar:
			self.scrollBar.SetPos(0)
		self.RefreshAll()
	def SetExtraScrollLen(self, extraLen):
		self.scrollLenExtra=extraLen
	def GetItems(self):
		return self.itemList
	def AppendItem(self, newItem):
		self.itemList.append(newItem)
		self.RefreshAll()
	def SetScrollBar(self, scrollBar):
		scrollBar.SetScrollEvent(__mem_func__(self.__OnScroll))
		self.scrollBar=scrollBar
	def OnMouseWheel(self, nLen):
		if self.scrollBar:
			if self.scrollBar.IsShow():
				if nLen > 0:
					self.scrollBar.OnUp()
				else:
					self.scrollBar.OnDown()
				return True
		return False
	def __OnScroll(self):
		self.SetBasePos(int(self.scrollBar.GetPos()*self.scrollLen))
	def RefreshAll(self):
		windowHeight = self.GetHeight()
		scrollBar = self.scrollBar
		screenSize = 0
		for child in self.itemList:
			if child.exPos[1]+child.GetHeight() > screenSize:
				screenSize = child.exPos[1]+child.GetHeight()
		if screenSize > windowHeight:
			scrollLen = screenSize-windowHeight
			if scrollLen != 0:
				scrollLen += self.scrollLenExtra
			self.scrollLen = scrollLen
			scrollBar.SetMiddleBarSize(float(windowHeight-5)/float(screenSize))
		else:
			scrollBar.SetMiddleBarSize(1.0)
	def Render(self,basePos):
		for item in self.itemList:
			(ex,ey) = item.exPos
			if self.isHorizontal:
				item.SetPosition(ex-(basePos), ey)
			else:
				item.SetPosition(ex, ey-(basePos))
			item.OnRender()
	def SetBasePos(self, basePos):
		if self.basePos == basePos:
			return
		self.Render(basePos)
		self.basePos=basePos

class MultiTextLineNew(Window):
	def __del__(self):
		Window.__del__(self)
	def Destroy(self):
		self.textRules = {}
	def __init__(self):
		Window.__init__(self)
		self.Destroy()
		self.AddFlag("not_pick")
		self.textRules["textRange"] = 15
		self.textRules["text"] = ""
		self.textRules["textType"] = ""
		self.textRules["fontName"] = ""
		self.textRules["hexColor"] = 0
		self.textRules["fontColor"] = 0
		self.textRules["outline"] = 0
	def SetTextType(self, textType):
		self.textRules["textType"] = textType
		self.Refresh()
	def SetTextRange(self, textRange):
		self.textRules["textRange"] = textRange
		self.Refresh()
	def SetOutline(self, outline):
		self.textRules["outline"] = outline
		self.Refresh()
	def SetPackedFontColor(self, hexColor):
		self.textRules["hexColor"] = hexColor
		self.Refresh()
	def SetFontColor(self, r, g, b):
		self.textRules["fontColor"] =[r, g, b]
		self.Refresh()
	def SetFontName(self, fontName):
		self.textRules["fontName"] = fontName
		self.Refresh()
	def SetText(self, newText):
		self.textRules["text"] = newText
		self.Refresh()
	def Refresh(self):
		textRules = self.textRules
		self.children=[]

		outline = textRules["outline"]
		fontColor = textRules["fontColor"]
		hexColor = textRules["hexColor"]
		yRange = textRules["textRange"]
		fontName = textRules["fontName"]
		textType = textRules["textType"].split("#")
		totalTextList = textRules["text"].split("#")

		(xPosition, yPosition) = (0, 0)

		for text in totalTextList:
			childText = TextLine()
			childText.SetParent(self)
			childText.AddFlag("not_pick")
			childText.SetPosition(xPosition, yPosition)
			if fontName != "":
				childText.SetFontName(fontName)
			if hexColor != 0:
				childText.SetPackedFontColor(hexColor)
			if fontColor != 0:
				childText.SetFontColor(*fontColor)
			if outline:
				childText.SetOutline()
			self.AddTextType(childText, textType)
			childText.SetText(str(text))
			childText.Show()
			self.children.append(childText)
			yPosition+=yRange
		self.CheckTexType(self.children, textType, yPosition)
	def AddTextType(self, text,  typeArg):
		if len(typeArg) != 2:
			return
		_typeDict = {
			"vertical":{
				"top":text.SetVerticalAlignTop,
				"bottom":text.SetVerticalAlignBottom,
				"center":text.SetVerticalAlignCenter,
			},
			"horizontal":{
				"top":text.SetHorizontalAlignLeft,
				"bottom":text.SetHorizontalAlignRight,
				"center":text.SetHorizontalAlignCenter,
			},
			"all_align":{
				"1" : [text.SetHorizontalAlignCenter,text.SetVerticalAlignCenter,text.SetWindowHorizontalAlignCenter,text.SetWindowVerticalAlignCenter],
			},
		}
		(firstToken, secondToken) = tuple(typeArg)
		if _typeDict.has_key(firstToken):
			textType = _typeDict[firstToken][secondToken] if _typeDict[firstToken].has_key(secondToken) else None
			if textType != None:
				if isinstance(textType, list):
					for rule in textType:
						rule()
				else:
					textType()
	def CheckTexType(self, textList,  typeArg, yMax):
		if len(typeArg) != 2:
			return
		elif typeArg[1] != "center":
			return
		width = 0
		for text in textList:
			if text.GetTextSize()[0] > width:
				width = text.GetTextSize()[0]
		centerWidth = width / 2
		#for text in textList:
		#	text.SetPosition(centerWidth, text.GetLocalPosition()[1])

		self.SetSize(width, yMax)


class DropdownTree(Window):
	class Item(Window):
		def __init__(self):
			Window.__init__(self)
			self.id = -1
			self.parentId = -1
			self.offset = 0
			self.visible = False
			self.expanded = False
			self.event = None
			self.onCollapseEvent = None
			self.onExpandEvent = None

		def __del__(self):
			Window.__del__(self)

		def Destroy(self):
			self.id = 0
			self.parentId = 0
			self.offset = 0
			self.visible = 0
			self.expanded = 0
			self.event = 0
			self.onCollapseEvent = 0
			self.onExpandEvent = 0
			self.parent = 0

		def SetParent(self, parent):
			Window.SetParent(self, parent)
			self.parent=proxy(parent)

		def SetSize(self, width, height):
			Window.SetSize(self, width, height)

		def GetId(self):
			return self.id

		def SetId(self, id):
			self.id = id

		def GetParentId(self):
			return self.parentId

		def SetParentId(self, parentId):
			self.parentId = parentId
			
		def IsParent(self):
			return self.parentId == -1

		def SetVisible(self, visible):
			self.visible = visible
			
		def IsVisible(self):
			return self.visible
			
		def IsExpanded(self):
			return self.expanded
			
		def Expand(self):
			self.expanded = True
			if self.onExpandEvent:
				self.onExpandEvent()
			
		def Collapse(self):
			self.expanded = False
			if self.onCollapseEvent:
				self.onCollapseEvent()

		def SetOnExpandEvent(self, event):
			self.onExpandEvent = __mem_func__(event)

		def SetOnCollapseEvent(self, event):
			self.onCollapseEvent = __mem_func__(event)

		def SetOffset(self, offset):
			self.offset = offset

		def GetOffset(self):
			return self.offset

		def SetEvent(self, event):
			self.event = event

		def OnSelect(self):
			self.parent.SelectItem(self)

		def OnMouseLeftButtonDown(self):
			self.OnSelect()

	def __init__(self):
		Window.__init__(self)

		self.__curItemId=0
		self.viewItemCount=10
		self.basePos=0
		self.itemHeight=29
		self.isShopSearch=0
		self.itemStep=29
		self.selItem=0
		self.itemList=[]
		self.onSelectItemEvent = lambda *arg: None
		self.itemWidth=185

		self.scrollBar=None
		self.__UpdateSize()
	
	def __del__(self):
		Window.__del__(self)

	def __UpdateSize(self):
		height=self.itemStep*self.__GetViewItemCount()

		#self.SetSize(self.itemWidth, height)
		#self.SetSize(self.itemWidth, 375)

	def IsEmpty(self):
		if len(self.itemList)==0:
			return 1
		return 0

	#def OnMouseWheel(self, nLen):
	#	if self.scrollBar:
	#		self.scrollBar.OnMouseWheel(nLen)
	
	def OnMouseWheel(self, nLen):
		if self.scrollBar:
			if nLen > 0:
				self.scrollBar.OnUp()
			else:
				self.scrollBar.OnDown()
			return True
		return False

	def SetItemStep(self, itemStep):
		self.itemStep=itemStep
		self.__UpdateSize()

	def SetItemSize(self, itemWidth, itemHeight):
		self.itemWidth=itemWidth
		self.itemHeight=itemHeight
		self.__UpdateSize()
	
	def SetViewItemCount(self, viewItemCount):
		self.viewItemCount=viewItemCount
	
	def SetSelectEvent(self, event):
		self.onSelectItemEvent = event

	def SetBasePos(self, basePos):
		for oldItem in self.itemList:
			oldItem.Hide()

		self.basePos=basePos

		skipCount = basePos
		pos = basePos
		for lItem in self.itemList:
			if not lItem.IsVisible():
				continue
			
			if skipCount > 0:
				skipCount -= 1
				continue

			if pos >= (self.basePos+self.viewItemCount):
				break

			(x, y) = self.GetItemViewCoord(pos, lItem.GetWidth())
			lItem.SetPosition(x+lItem.GetOffset(), y)
			lItem.Show()
			pos+=1
		self.UpdateScrollbar()

	def GetItemIndex(self, argItem):
		return self.itemList.index(argItem)

	def GetSelectedItem(self):
		return self.selItem

	def SelectIndex(self, index):
		if index >= len(self.itemList) or index < 0:
			self.selItem = None
			return
		try:
			self.selItem=self.itemList[index]
		except:
			pass

	def ClearItem(self):
		self.selItem=None
		for lItem in self.itemList:
			lItem.Hide()
			lItem.Destroy()
			lItem = 0
		self.itemList=[]
		self.__curItemId = 0

		if self.scrollBar:
			self.scrollBar.SetPos(0)
		self.SetBasePos(0)

	def SelectItem(self, selItem):
		if self.isShopSearch:
			for item in self.itemList:
				if selItem != item:
					self.CloseTree(item, self.itemList)

		self.selItem = selItem
		if selItem.IsExpanded():
			self.CloseTree(selItem, self.itemList)
		else:
			if selItem.event:
				selItem.event()
			self.OpenTree(selItem, self.itemList)
		self.SetBasePos(self.basePos)
		

	def __AppendItem(self, newItem, parentId):
		curItemId = self.__curItemId
		self.__curItemId += 1
		
		newItem.SetParent(self)
		newItem.SetParentId(parentId)
		newItem.SetSize(self.itemWidth, self.itemHeight)
		newItem.SetId(curItemId)

		pos = self.__GetItemCount()
		self.itemList.append(newItem)

		if newItem.IsVisible() and self.__IsInViewRange(pos):
			(x, y)=self.GetItemViewCoord(pos, newItem.GetWidth())
			newItem.SetPosition(x, y)
			newItem.Show()
		else:
			newItem.Hide()

		self.UpdateScrollbar()

		return curItemId

	def AppendItemList(self, dict):
		self.__AppendItemList(-1, dict)
	
	def __AppendItemList(self, parentId, dict):
		for lItem in dict:
			if 'item' in lItem:
				id = self.__AppendItem(lItem['item'], parentId)
				if 'children' in lItem:
					self.__AppendItemList(id, lItem['children'])
				
	def SetScrollBar(self, scrollBar):
		scrollBar.SetScrollEvent(__mem_func__(self.__OnScroll))
		self.scrollBar=scrollBar

	def __OnScroll(self):
		self.SetBasePos(int(self.scrollBar.GetPos()*self.__GetScrollLen()))

	def __GetScrollLen(self):
		scrollLen=self.__GetItemCount()-self.__GetViewItemCount()
		if scrollLen<0:
			return 0

		return scrollLen

	def __GetViewItemCount(self):
		return self.viewItemCount

	def __GetItemCount(self):
		return sum(1 for lItem in self.itemList if lItem.IsVisible())

	def GetItemViewCoord(self, pos, itemWidth):
		return (0, (pos-self.basePos)*self.itemStep)

	def __IsInViewRange(self, pos):
		if pos<self.basePos:
			return 0
		if pos>=self.basePos+self.viewItemCount:
			return 0
		return 1
	
	def UpdateScrollbar(self):
		if self.__GetViewItemCount() < self.__GetItemCount():
			self.scrollBar.SetMiddleBarSize(float(self.__GetViewItemCount())/self.__GetItemCount())
			self.scrollBar.Show()
		else:
			self.scrollBar.Hide()

	def CloseTree(self, curItem, list):
		curItem.Collapse()
		for listboxItem in list:
			if listboxItem.GetParentId() == curItem.GetId():
				listboxItem.SetVisible(False)
				self.CloseTree(listboxItem, list)
		
	def OpenTree(self, curItem, list):
		curItem.Expand()
		for listboxItem in list:
			if listboxItem.GetParentId() == curItem.GetId():
				listboxItem.SetVisible(True)

class LastListItem(DropdownTree.Item):
	def __init__(self, text):
		DropdownTree.Item.__init__(self)
		self.overLine = False

		textLine=TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(localeInfo.UI_DEF_FONT)
		textLine.SetWindowHorizontalAlignLeft()
		textLine.SetPosition(5,5)
		textLine.SetText(text)
		textLine.Show()

		self.textLine = textLine
		self.text = text

	def __del__(self):
		DropdownTree.Item.__del__(self)

	def Destroy(self):
		DropdownTree.Item.Destroy(self)
		self.overLine = 0
		self.textLine = 0
		self.text = 0

	def GetText(self):
		return self.text

	def SetSize(self, width, height):
		DropdownTree.Item.SetSize(self, width-self.GetOffset(), height)

	def OnMouseOverIn(self):
		self.overLine = True

	def OnMouseOverOut(self):
		self.overLine = False

	def OnRender(self):
		if self.overLine and self.parent.GetSelectedItem()!=self:
			x, y = self.GetGlobalPosition()
			grp.SetColor(grp.GenerateColor(1.0, 1.0, 1.0, 0.2))
			grp.RenderBar(x, y, self.GetWidth(), self.GetHeight())
		elif self.parent.GetSelectedItem()==self:
			x, y = self.GetGlobalPosition()
			grp.SetColor(grp.GenerateColor(0.0, 0.0, 1.0, 1.0))
			grp.RenderBar(x, y, self.GetWidth(), self.GetHeight())
		else:
			x, y = self.GetGlobalPosition()
			grp.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 1.0))
			grp.RenderBar(x, y, self.GetWidth(), self.GetHeight())

RegisterToolTipWindow("TEXT", TextLine)

def MakeText(parent, textlineText, x, y, color):
	textline = TextLine()
	if parent != None:
		textline.SetParent(parent)
	textline.SetPosition(x, y)
	if color != None:
		textline.SetFontColor(color[0], color[1], color[2])
	textline.SetText(textlineText)
	textline.Show()
	return textline
def MakeThinBoard(parent,  x, y, width, heigh, moveable=FALSE,center=FALSE):
	thin = ThinBoard()
	if parent != None:
		thin.SetParent(parent)
	if moveable == TRUE:
		thin.AddFlag('movable')
		thin.AddFlag('float')
	thin.SetSize(width, heigh)
	thin.SetPosition(x, y)
	if center == TRUE:
		thin.SetCenterPosition()
	thin.Show()
	return thin

class gridCalculator:

	def __init__(self, pageWidth, pageHeight, pageCount, startIndex = 0):
		self.pageWidth = pageWidth
		self.pageHeight = pageHeight
		self.pageCount = pageCount
		self.startIndex = startIndex
		
		pageSlotCount = pageWidth * pageHeight
		self.pageSlotCount = pageSlotCount
		self.totalSlotCount = pageSlotCount * pageCount

	def GetPageWidth(self):
		return self.pageWidth

	def GetPageHeight(self):
		return self.pageHeight

	def GetPageCount(self):
		return self.pageCount

	def GetStartIndex(self):
		return self.startIndex
	
	def GetPageSlotCount(self):
		return self.pageSlotCount

	def GetTotalSlotCount(self):
		return self.totalSlotCount

	def GetPageStartIndex(self, pageIndex):
		return self.startIndex + self.pageSlotCount * pageIndex
		
	def SetStartIndex(self, startIndex):
		self.startIndex = startIndex

	def HasInventorySlot(self, slotIndex):
		return slotIndex >= self.startIndex and slotIndex <= self.startIndex + self.totalSlotCount

	def HasSlot(self, slotIndex):
		return self.HasInventorySlot(slotIndex)

	def HasPageSlot(self, pageIndex, slotIndex):
		return slotIndex >= self.GetPageStartIndex(pageIndex) and slotIndex < self.GetPageStartIndex(pageIndex + 1)

	def GetPageIndexBySlot(self, slotIndex):
		return int((slotIndex - self.startIndex) / self.pageSlotCount)

	def CanInventoryContainItemSize(self, itemSize):
		if self.pageCount == 0:
			return False
		
		if self.pageWidth == 0:
			return False
	
		if self.pageHeight < itemSize:
			return False
	
		return True

	def CanSlotContainItemSize(self, slotIndex, itemSize):
		if itemSize == 0:
			return True
		
		if not self.HasInventorySlot(slotIndex):
			return False
	
		if itemSize == 1:
			return True
		
		pageIndex = self.GetPageIndexBySlot(slotIndex)
		return self.HasPageSlot(pageIndex, slotIndex + (itemSize - 1) * self.pageWidth)

	def GetItemSizeBySlot(self, slotIndex):
		if not player.isItem(slotIndex):
			return 0
		
		return item.GetItemSize(item.SelectItem(player.GetItemIndex(slotIndex)))[1]

	def GetEmptySlot(self, itemSize):
		blockedSlots = []

		if not self.CanInventoryContainItemSize(itemSize):
			return -1
			
	
		for slotIndex in xrange(self.startIndex, self.startIndex + self.totalSlotCount):
			if slotIndex in blockedSlots:
				continue
	
			if not self.CanSlotContainItemSize(slotIndex, itemSize):
				continue
	
			for i in xrange(itemSize):
				size = self.GetItemSizeBySlot(slotIndex + i * self.pageWidth)
			
				if size > 0:
		
					for j in xrange(1, i + size):
						blockedSlots.append(slotIndex + j * self.pageWidth)
					break
		
			else:
				return slotIndex
		return -1

class RenderTarget(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		if app.ENABLE_WIKI:
			self.eventDict={}
			self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
			self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}

	def Destroy(self):
		if app.ENABLE_WIKI:
			self.eventDict={}
			self.eventFunc = 0
			self.eventArgs = 0
	def __del__(self):
		Window.__del__(self)
	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterRenderTarget(self, layer)
	def SetRenderTarget(self, number):
		wndMgr.SetRenderTarget(self.hWnd, number)
	if app.ENABLE_WIKI:
		def SetRenderingRect(self, left, top, right, bottom):
			wndMgr.SetRenderingRect(self.hWnd, left, top, right, bottom)
		def SetEvent(self, func, *args):
			result = self.eventFunc.has_key(args[0])
			if result:
				self.eventFunc[args[0]] = func
				self.eventArgs[args[0]] = args
			else:
				#print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]
				return
		def OnMouseLeftButtonUp(self) :
			if self.eventFunc["mouse_click"] :
				apply(self.eventFunc["mouse_click"], self.eventArgs["mouse_click"])
		def OnMouseOverIn(self):
			try:
				if self.eventFunc["mouse_over_in"]:
					if self.eventArgs:
						apply(self.eventFunc["mouse_over_in"], self.eventArgs["mouse_over_in"])
					else:
						self.eventFunc["mouse_over_in"]()
			except KeyError:
				pass
		def OnMouseOverOut(self):
			try:
				if self.eventFunc["mouse_over_out"]:
					if self.eventArgs:
						apply(self.eventFunc["mouse_over_out"], self.eventArgs["mouse_over_out"])
					else:
						self.eventFunc["mouse_over_out"]()
			except KeyError:
				pass

class Grid: # from KeN
	def __init__(self, width, height):
		self.width = width
		self.height = height
		self.reset()
	def find_blank(self, width, height):
		if width > self.width or height > self.height:
			return -1

		for row in range(self.height):
			for col in range(self.width):
				index = row * self.width + col
				if self.is_empty(index, width, height):
					return index
		return -1
	def put(self, pos, width, height):
		if not self.is_empty(pos, width, height):
			return False
		for row in range(height):
			start = pos + (row * self.width)
			self.grid[start] = True
			col = 1
			while col < width:
				self.grid[start + col] = True
				col += 1
		return True
	def clear(self, pos, width, height):
		if pos < 0 or pos >= (self.width * self.height):
			return
		for row in range(height):
			start = pos + (row * self.width)
			self.grid[start] = True
			col = 1
			while col < width:
				self.grid[start + col] = False
				col += 1
	def is_empty(self, pos, width, height):
		if pos < 0:
			return False
		row = pos // self.width
		if (row + height) > self.height:
			return False
		if (pos + width) > ((row * self.width) + self.width):
			return False
		for row in range(height):
			start = pos + (row * self.width)
			if self.grid[start]:
				return False
			col = 1
			while col < width:
				if self.grid[start + col]:
					return False
				col += 1
		return True
	def get_size(self):
		return self.width * self.height
	def reset(self):
		self.grid = [False] * (self.width * self.height)
		#self.put(self.width,1,1) # fix from dracaryS

if app.__CHAT_SETTINGS__:
	class MouseReflector(Window):
		def __init__(self, parent):
			Window.__init__(self)
			self.SetParent(parent)
			self.AddFlag("not_pick")
			self.width = self.height = 0
			self.isDown = False
		def __del__(self):
			Window.__del__(self)
		def Down(self):
			self.isDown = True
		def Up(self):
			self.isDown = False
		def OnRender(self):
			if self.isDown:
				grp.SetColor(WHITE_COLOR)
			else:
				grp.SetColor(HALF_WHITE_COLOR)
			x, y = self.GetGlobalPosition()
			grp.RenderBar(x + 2, y + 2, self.GetWidth() - 4, self.GetHeight() - 4)
	class CheckBoxNew(ImageBox):
		def __init__(self, parent, x, y, event, filename = "d:/ymir work/ui/chat/chattingoption_check_box_off.sub"):
			ImageBox.__init__(self)
			self.SetParent(parent)
			self.SetPosition(x, y)
			self.LoadImage(filename)
			self.mouseReflector = MouseReflector(self)
			self.mouseReflector.SetSize(self.GetWidth(), self.GetHeight())
			image = MakeImageBox(self, "d:/ymir work/ui/public/check_image.sub", 0, 0)
			image.AddFlag("not_pick")
			image.SetWindowHorizontalAlignCenter()
			image.SetWindowVerticalAlignCenter()
			image.Hide()
			self.check = False
			self.enable = True
			self.image = image
			self.event = event
			self.Show()
			self.mouseReflector.UpdateRect()
		def __del__(self):
			ImageBox.__del__(self)
		def GetCheck(self):
			return self.check
		def SetCheck(self, flag):
			if flag:
				self.check = True
				self.image.Show()
			else:
				self.check = False
				self.image.Hide()
		def Disable(self):
			self.enable = False
		def OnMouseOverIn(self):
			if not self.enable:
				return
			self.mouseReflector.Show()
		def OnMouseOverOut(self):
			if not self.enable:
				return
			self.mouseReflector.Hide()
		def OnMouseLeftButtonDown(self):
			if not self.enable:
				return
			self.mouseReflector.Down()
		def OnMouseLeftButtonUp(self):
			if not self.enable:
				return
			self.mouseReflector.Up()
			self.event()

if app.ENABLE_DUNGEON_INFO:
	class ScrollBarEx(Window):
		SCROLLBAR_WIDTH = 13
		SCROLLBAR_MIDDLE_HEIGHT = 1
		SCROLLBAR_BUTTON_WIDTH = 17
		SCROLLBAR_BUTTON_HEIGHT = 17
		SCROLL_BTN_XDIST = 1
		SCROLL_BTN_YDIST = 1
		IMG_DIR = "d:/ymir work/ui/game/dungeon_info/"
		#class MiddleBar(DragButton):
		#	IMG_DIR = "d:/ymir work/ui/game/dungeon_info/"
		#	def __init__(self):
		#		DragButton.__init__(self)
		#		self.AddFlag("movable")
		#		self.SetWindowName("scrollbar_middlebar")
		#	def MakeImage(self):
		#		top = ExpandedImageBox()
		#		top.SetParent(self)
		#		top.LoadImage(self.IMG_DIR+"btc_slider_top.tga")
		#		#top.LoadImage("d:/ymir work/ui/itemshop/scrollbar_top.tga")
		#		top.AddFlag("not_pick")
		#		top.Show()
		#
		#		bottom = ExpandedImageBox()
		#		bottom.SetParent(self)
		#		bottom.LoadImage(self.IMG_DIR+"btc_slider_bot.tga")
		#		#bottom.LoadImage("d:/ymir work/ui/itemshop/scrollbar_bottom.tga")
		#		bottom.AddFlag("not_pick")
		#		bottom.Show()
		#
		#
		#		middle = ExpandedImageBox()
		#		middle.SetParent(self)
		#		middle.LoadImage(self.IMG_DIR+"btc_slider_mid.tga")
		#		#middle.LoadImage("d:/ymir work/ui/itemshop/scrollbar_middle.tga")
		#		middle.AddFlag("not_pick")
		#		middle.Show()
		#
		#		self.top = top
		#		self.bottom = bottom
		#		self.middle = middle
		#
		#	def SetSize(self, height):
		#		extraHeight = 30
		#		minHeight = self.top.GetHeight() + self.bottom.GetHeight() + self.middle.GetHeight()+extraHeight
		#		height = max(minHeight, height)
		#		DragButton.SetSize(self, 8, height)
		#
		#		scale = (height - minHeight) / 2 
		#		extraScale = 0
		#		if (height - minHeight) % 2 == 1:
		#			extraScale = 1
		#
		#		self.middle.SetPosition(0, self.top.GetHeight() + scale)
		#		self.bottom.SetPosition(0, height - self.bottom.GetHeight())
		class MiddleBar(DragButton):
			IMG_DIR = "d:/ymir work/ui/game/battle_pass/"
			def __init__(self):
				DragButton.__init__(self)
				self.AddFlag("movable")
				self.SetWindowName("scrollbar_middlebar")
			def MakeImage(self):
				top = ExpandedImageBox()
				top.SetParent(self)
				top.LoadImage(self.IMG_DIR+"scrollbar/scroll_top.tga")
				top.AddFlag("not_pick")
				top.Show()
				bottom = ExpandedImageBox()
				bottom.SetParent(self)
				bottom.LoadImage(self.IMG_DIR+"scrollbar/scroll_buttom.tga")
				bottom.AddFlag("not_pick")
				bottom.Show()
				middle = ExpandedImageBox()
				middle.SetParent(self)
				middle.LoadImage(self.IMG_DIR+"scrollbar/scroll_mid.tga")
				middle.AddFlag("not_pick")
				middle.Show()
				self.top = top
				self.bottom = bottom
				self.middle = middle
			def SetSize(self, height):
				minHeight = self.top.GetHeight() + self.bottom.GetHeight() + self.middle.GetHeight()
				height = max(minHeight, height)
				DragButton.SetSize(self, 10, height)
				scale = (height - minHeight) / 2 
				extraScale = 0
				if (height - minHeight) % 2 == 1:
					extraScale = 1
				self.middle.SetPosition(0, self.top.GetHeight() + scale)
				self.bottom.SetPosition(0, height - self.bottom.GetHeight())

		def __init__(self):
			Window.__init__(self)

			self.pageSize = 1
			self.curPos = 0.0
			self.eventScroll = None
			self.eventArgs = None
			self.lockFlag = False

			self.CreateScrollBar()
			self.SetScrollBarSize(0)

			self.scrollStep = 0.07#you can set speed.
			self.SetWindowName("NONAME_ScrollBar")

		def __del__(self):
			Window.__del__(self)

		def CreateScrollBar(self):
			topImage = ExpandedImageBox()
			topImage.SetParent(self)
			topImage.AddFlag("not_pick")
			topImage.LoadImage(self.IMG_DIR+"slider_top.tga")
			topImage.Show()

			bottomImage = ExpandedImageBox()
			bottomImage.SetParent(self)
			bottomImage.AddFlag("not_pick")
			bottomImage.LoadImage(self.IMG_DIR+"slider_bot.tga")
			bottomImage.Show()

			middleImage = ExpandedImageBox()
			middleImage.SetParent(self)
			middleImage.AddFlag("not_pick")
			middleImage.SetPosition(0, topImage.GetHeight())
			middleImage.LoadImage(self.IMG_DIR+"slider_mid.tga")
			middleImage.Show()

			self.topImage = topImage
			self.bottomImage = bottomImage
			self.middleImage = middleImage

			middleBar = self.MiddleBar()
			middleBar.SetParent(self)
			middleBar.SetMoveEvent(__mem_func__(self.OnMove))
			middleBar.Show()
			middleBar.MakeImage()
			middleBar.SetSize(0) # set min height
			self.middleBar = middleBar

		def Destroy(self):
			self.eventScroll = None
			self.eventArgs = None

		def SetScrollEvent(self, event, *args):
			self.eventScroll = event
			self.eventArgs = args

		def SetMiddleBarSize(self, pageScale):
			self.middleBar.SetSize(int(pageScale * float(self.GetHeight() - self.SCROLL_BTN_YDIST*2)))
			realHeight = self.GetHeight() - self.SCROLL_BTN_YDIST*2 - self.middleBar.GetHeight()
			self.pageSize = realHeight

		def SetScrollBarSize(self, height):
			self.SetSize(self.SCROLLBAR_WIDTH, height)

			self.pageSize = height - self.SCROLL_BTN_YDIST*2 - self.middleBar.GetHeight()

			middleImageScale = float((height - self.SCROLL_BTN_YDIST*2) - self.middleImage.GetHeight()) / float(self.middleImage.GetHeight())
			self.middleImage.SetRenderingRect(0, 0, 0, middleImageScale)
			self.bottomImage.SetPosition(0, height - self.bottomImage.GetHeight())

			#self.middleBar.SetRestrictMovementArea(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST, self.middleBar.GetWidth(), height)# - self.SCROLL_BTN_YDIST * 2)
			self.middleBar.SetRestrictMovementArea(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST, self.middleBar.GetWidth(), height - self.SCROLL_BTN_YDIST * 2)
			self.middleBar.SetPosition(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST)

		def SetScrollStep(self, step):
			self.scrollStep = step

		def GetScrollStep(self):
			return self.scrollStep

		def GetPos(self):
			return self.curPos

		def OnUp(self):
			self.SetPos(self.curPos-self.scrollStep)

		def OnDown(self):
			self.SetPos(self.curPos+self.scrollStep)

		def SetPos(self, pos, moveEvent = True):
			pos = max(0.0, pos)
			pos = min(1.0, pos)
			newPos = float(self.pageSize) * pos
			self.middleBar.SetPosition(self.SCROLL_BTN_XDIST, int(newPos) + self.SCROLL_BTN_YDIST)
			if moveEvent == True:
				self.OnMove()

		def OnMove(self):
			if self.lockFlag:
				return
			if 0 == self.pageSize:
				return
			(xLocal, yLocal) = self.middleBar.GetLocalPosition()
			self.curPos = float(yLocal - self.SCROLL_BTN_YDIST) / float(self.pageSize)
			if self.eventScroll:
				apply(self.eventScroll, self.eventArgs)

		def OnMouseLeftButtonDown(self):
			(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
			newPos = float(yMouseLocalPosition) / float(self.GetHeight())
			self.SetPos(newPos)

		def LockScroll(self):
			self.lockFlag = True

		def UnlockScroll(self):
			self.lockFlag = False

class MultiTextLine(Window):
	def __del__(self):
		Window.__del__(self)
	def Destroy(self):
		self.children = []
		self.rangeText = 0
	def __init__(self):
		Window.__init__(self)
		self.children = []
		self.rangeText = 15
		self.textType = ""
		self.fontName = ""
		self.outline = False
		self.rgb = (0,0,0)
	def SetFontColor(self, r, g, b):
		self.rgb = (r, g, b)
		for text in self.children:
			if self.rgb[0] != 0:
				text.SetFontColor(self.rgb[0],self.rgb[1],self.rgb[2])
	def SetOutline(self, flag):
		self.outline = flag
		for text in self.children:
			if self.outline:
				text.SetOutline()
	def SetTextType(self, textType):
		self.textType = textType
		for text in self.children:
			self.AddTextType(self.textType.split("#"),text)
	def SetFontName(self, fontName):
		self.fontName = fontName
		for text in self.children:
			text.SetFontName(fontName)
	def GetTextSize(self, index = 0):
		if len(self.children) == 0:
			return (0,0)
		elif index >= len(self.children):
			return (0,0)
		return self.children[index].GetTextSize()

	def SetTextRange(self, range):
		self.rangeText = range
		yPosition = 0
		for text in self.children:
			text.SetPosition(0,yPosition)
			yPosition+=self.rangeText
	def AddTextType(self, typeArg, text):
		if len(typeArg) > 1:
			if typeArg[0] == "vertical":
				if typeArg[1] == "top":
					text.SetVerticalAlignTop()
				elif typeArg[1] == "bottom":
					text.SetVerticalAlignBottom()
				elif typeArg[1] == "center":
					text.SetVerticalAlignCenter()
			elif typeArg[0] == "horizontal":
				if typeArg[1] == "left":
					text.SetHorizontalAlignLeft()
				elif typeArg[1] == "right":
					text.SetHorizontalAlignRight()
				elif typeArg[1] == "center":
					text.SetHorizontalAlignCenter()
	def SetText(self, cmd):
		if len(self.children) > 1:
			self.children=[]

		multi_arg = cmd.split("\m")
		yPosition = 0
		for text in multi_arg:
			childText = TextLine()
			childText.SetParent(self)
			childText.SetPosition(0,yPosition)
			if self.rgb[0] != 0:
				childText.SetFontColor(self.rgb[0],self.rgb[1],self.rgb[2])
			if self.textType != "":
				self.AddTextType(self.textType.split("#"),childText)
			if self.fontName != "":
				childText.SetFontName(self.fontName)
			if self.outline:
				childText.SetOutline()
			childText.SetText(str(text))
			childText.Show()
			self.children.append(childText)
			yPosition+=self.rangeText


class MessageBoard(Board):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BASE_PATH = "d:/ymir work/ui/pattern"

	IMAGES = {
		'CORNER' : {
			0 : "border_b_left_top",
			1 : "border_b_left_bottom",
			2 : "border_b_right_top",
			3 : "border_b_right_bottom"
		},
		'BAR' : {
			0 : "border_b_left",
			1 : "border_b_right",
			2 : "border_b_top",
			3 : "border_b_bottom"
		},
		'FILL' : "border_b_center"
	}
	def __init__(self):
		Board.__init__(self)
	def __del__(self):
		Board.__del__(self)

class MessageWindow(Window):
	def __del__(self):
		Window.__del__(self)
	def Destroy(self):
		self.children = []
	def __init__(self):
		Window.__init__(self)
		self.Destroy()
		self.SetPosition(0,wndMgr.GetScreenHeight()-400)
		self.AddFlag("not_pick")
		self.Show()

	def AddMessage(self, message, showSecond = 5):
		newMessage = MessageBoard()
		newMessage.SetParent(self)
		newMessageText = TextLine()
		newMessageText.SetParent(newMessage)
		newMessageText.SetHorizontalAlignLeft()
		newMessageText.SetPosition(8,7)
		newMessageText.SetText(message)
		newMessageText.Show()
		newMessage.newMessageText = newMessageText
		newMessage.SetSize(newMessageText.GetTextSize()[0]+16,16)
		newMessage.SetPosition(-newMessage.GetWidth(),(len(self.children)*35))
		newMessage.isActiveSlide = True
		newMessage.isActiveSlideOut = False
		newMessage.endTime = app.GetGlobalTimeStamp() + showSecond
		newMessage.SetTop()
		newMessage.Show()
		self.children.append(newMessage)

	def OnUpdate(self):
		for message in self.children:
			y = self.children.index(message)*35
			if message.isActiveSlide and message.isActiveSlide == True:
				x, _y = message.GetLocalPosition()
				if x < 0:
					message.SetPosition(x + 4, y)
				else:
					message.SetPosition(x, y)
			if message.endTime - app.GetGlobalTimeStamp() <= 0 and message.isActiveSlideOut == False and message.isActiveSlide == True:
				message.isActiveSlide = False
				message.isActiveSlideOut = True
			if message.isActiveSlideOut and message.isActiveSlideOut == True:
				x, _y = message.GetLocalPosition()
				if x > -message.GetWidth():
					message.SetPosition(x - 4, y)
				else:
					message.SetPosition(x, y)
				if x <= -message.GetWidth():
					message.isActiveSlideOut = False
					del self.children[self.children.index(message)]
					
def MakeExpandedImageBox(parent, name, x, y, flag = ""):
	image = ExpandedImageBox()
	image.SetParent(parent)
	image.LoadImage(name)
	image.SetPosition(x, y)

	if flag != "":
		image.AddFlag(flag)

	image.Show()

	return image
	
def MakeTextLineNew(parent, x, y, text):
	textLine = TextLine()
	textLine.SetParent(parent)
	textLine.SetPosition(x, y)
	textLine.SetText(text)
	textLine.SetOutline(True)
	textLine.Show()
	return textLine

def MakeButtonNew(parent, x, y, text, path, up, over, down):
	button = Button()
	button.SetParent(parent)
	button.SetPosition(x, y)
	button.SetUpVisual(path + up)
	button.SetOverVisual(path + over)
	button.SetDownVisual(path + down)
	button.SetText(text)
	button.UpdateRect()
	button.Show()
	return button


def MakeTextV3(parent, textlineText, x, y, color=False, font = False):
	if font:
		textline = TextLine(font)
	else:
		textline = TextLine()
	if parent != None:
		textline.SetParent(parent)
	textline.SetPosition(x, y)
	if color:
		textline.SetPackedFontColor(grp.GenerateColor(0.659,0.475,0.388,1.0))
	textline.SetText(textlineText)
	textline.Show()
	return textline


class PixelScrollListBox(Window):
	class Item(Window):
		def __init__(self, height=0):
			Window.__init__(self)

			self.SetSize(0, height)

			self.removeTopPixel = 0
			self.removeBottomPixel = 0

			self.elems = []

		def Destroy(self):
			for e in self.elems:
				e.Hide()
			Window.Destroy(self)
			self.elems = []

		def AddObj(self, elem, alignVerticalCenter = False):
			return self._AddElement(elem, alignVerticalCenter)

		def _AddElement(self, elem, alignVerticalCenter = False):
			elem.SetParent(self)
			elem.Show()
			if alignVerticalCenter:
				elem.SetWindowVerticalAlignCenter()
			self.elems.append(elem)
			return elem

		def SetRemovePixel(self, top, bottom):
			if self.removeTopPixel == top and self.removeBottomPixel == bottom:
				return

			self.removeTopPixel = top
			self.removeBottomPixel = bottom

			topPct = float(top) / self.GetHeight()
			bottomPct = float(bottom) / self.GetHeight()
			self._OnSetRenderingRect()

		def GetRemovedTopPixel(self):
			return self.removeTopPixel

		def GetRemovedBottomPixel(self):
			return self.removeBottomPixel

		def _OnSetRenderingRect(self):
			startY = self.GetRemovedTopPixel()
			endY = self.GetHeight() - self.GetRemovedBottomPixel()

			for elem in self.elems:
				elemPosX, elemPosY = elem.GetLocalPosition(self)

				elemHeight = elem.GetHeight()
				if elemHeight == 0:
					if isinstance(elem, TextLine):
						elemHeight = elem.GetTextHeight()
					else:
						continue

				topPixel = max(0, startY - elemPosY)
				bottomPixel = max(0, (elemPosY + elemHeight) - endY)
				topPct = float(topPixel) / elemHeight
				bottomPct = float(bottomPixel) / elemHeight

				if isinstance(elem, ExpandedImageBox):
					elem.SetRenderingRect(0, -topPct, 0, -bottomPct)

				elif isinstance(elem, Button) or isinstance(elem, TextLine) or isinstance(elem, ExtendedTextLine):
					if topPct > 0 or bottomPct > 0:
						elem.Hide()
					else:
						elem.Show()

	def __init__(self):
		Window.__init__(self)

		self.items = []
		self.__Initialize()

		self.verticalElementSpace = 0

	def __Initialize(self):
		for item in self.items:
			item.Destroy()
		self.items = []

		self.basePixelPos = 0
		self.maxHeight = 0

	def SetVerticalSpace(self, space):
		self.verticalElementSpace = space

	def ClearItem(self):
		self.__Initialize()

	def AppendItem(self, item):
		item.SetParent(self)
		item.SetSize(self.GetWidth(), item.GetHeight())

		self.items.append(item)

		self.maxHeight += item.GetHeight()

	def GetItemCount(self):
		return len(self.items)

	def LocateItem(self):
		curPxPos = -self.basePixelPos
		height = self.GetHeight()

		for item in self.items:
			item.SetPosition(0, curPxPos)

			if curPxPos >= height or curPxPos + item.GetHeight() <= 0:
				item.Hide()

			else:
				topRemovePixel = max(0, -curPxPos)
				bottomRemovePixel = max(0, (curPxPos + item.GetHeight()) - height)
				item.SetRemovePixel(topRemovePixel, bottomRemovePixel)
				item.Show()

			curPxPos += item.GetHeight() + self.verticalElementSpace

	def GetBasePos(self):
		return self.basePixelPos

	def SetBasePos(self, basePos):
		if self.basePixelPos == int(basePos):
			return

		self.basePixelPos = int(basePos)
		self.LocateItem()

	def GetMaxBasePos(self):
		return max(0, self.GetMaxHeight() - self.GetHeight())

	def GetMaxHeight(self):
		return self.maxHeight + self.verticalElementSpace * max(0, len(self.items) - 1)

class ModernScrollBar(Window):
	MIDDLE_BAR_POS = 2
	MIDDLE_BAR_UPPER_PLACE = 1
	MIDDLE_BAR_DOWNER_PLACE = 1
	TEMP_SPACE = MIDDLE_BAR_UPPER_PLACE + MIDDLE_BAR_DOWNER_PLACE

	class MiddleBar(DragButton):
		BUTTON_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
		BUTTON_ACTIVE_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)

		def __init__(self):
			DragButton.__init__(self)
			self.AddFlag("movable")

			self.bar = Bar()

		def MakeImage(self):
			self.bar = Bar()
			self.bar.SetParent(self)
			self.bar.SetPosition(0, 0)
			self.bar.SetSize(10, 0)
			self.bar.SetColor(self.BUTTON_COLOR)
			self.bar.AddFlag("not_pick")
			self.bar.Show()

		def OnMouseOverIn(self):
			self.bar.SetColor(self.BUTTON_ACTIVE_COLOR)

		def OnMouseOverOut(self):
			self.bar.SetColor(self.BUTTON_COLOR)

		def SetSize(self, width, height):
			height = max(12, height)
			DragButton.SetSize(self, width, height)
			self.bar.SetSize(width, height)

	def __init__(self):
		Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = lambda *arg: None
		self.lockFlag = False
		self.scrollStep = 0.20
		self.contentHeight = 0
		self.scroll_span = 0

		self.CreateScrollBar()

	def __del__(self):
		Window.__del__(self)

	def CreateScrollBar(self):
		self.SetSize(10, 0)

		self.barSlot = BoxedBoard()
		self.barSlot.SetParent(self)
		self.barSlot.AddFlag("not_pick")
		self.barSlot.Show()

		self.middleBar = self.MiddleBar()
		self.middleBar.SetParent(self)
		self.middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		self.middleBar.Show()
		self.middleBar.MakeImage()
		self.middleBar.SetSize(self.GetWidth() - 2, 12)

	def Destroy(self):
		self.barSlot = None
		self.middleBar = None
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def __SetMiddleBarSize(self, pageScale):
		realHeight = self.GetHeight() - 2
		self.middleBar.SetSize(self.GetWidth() - 2, int(pageScale * float(realHeight)))
		self.pageSize = self.GetHeight() - self.middleBar.GetHeight() - self.TEMP_SPACE

	# Deprecated, just refresh the middle bar size in case anything changed.
	def SetMiddleBarSize(self, tmp):
		self.SetContentHeight(self.contentHeight)

	def SetContentHeight(self, contentHeight):
		contentHeight = max(contentHeight, self.GetHeight() - self.TEMP_SPACE)
		self.contentHeight = contentHeight
		self.__SetMiddleBarSize(float(self.GetHeight() - 2) / float(contentHeight))

	def SetScrollBarSize(self, height):
		self.pageSize = height - self.middleBar.GetHeight() - self.TEMP_SPACE
		self.SetSize(self.GetWidth(), height)
		self.middleBar.SetRestrictMovementArea(1, self.MIDDLE_BAR_UPPER_PLACE, self.middleBar.GetWidth(), height - self.TEMP_SPACE)
		self.middleBar.SetPosition(1, self.MIDDLE_BAR_UPPER_PLACE)

		self.UpdateBarSlot()

	def UpdateBarSlot(self):
		self.barSlot.SetSize(self.GetWidth(), self.GetHeight())

	def SetWidth(self, width):
		self.SetSize(width, self.GetHeight())
		self.middleBar.SetSize(width - 2, self.middleBar.GetHeight())
		self.SetScrollBarSize(self.GetHeight())

	def GetPos(self):
		return self.curPos

	def SetSpan(self, span):
		self.scroll_span = span

	def IncreaseSpan(self, span):
		self.scroll_span += span

	def GetSpan(self):
		return self.scroll_span

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos) + self.MIDDLE_BAR_UPPER_PLACE)
		self.OnMove()

	def SetScrollStep(self, step):
		self.scrollStep = step

	def GetScrollStep(self):
		return self.scrollStep

	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def OnMove(self):
		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal - self.MIDDLE_BAR_UPPER_PLACE) / float(self.pageSize)

		self.eventScroll()

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		pickedPos = yMouseLocalPosition - (self.middleBar.GetHeight() / 2)
		newPos = float(pickedPos) / float(self.pageSize)
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = True

	def UnlockScroll(self):
		self.lockFlag = False

	def OnMouseWheel(self, len):
		curY = int(self.GetPos() * float(self.contentHeight)) - len
		self.SetPos(float(curY) / float(self.contentHeight))
		return True

class FineListBox(Window):
	class FineListBoxItem(Window):
		def __init__(self,color = 0xff0099ff, height = 40):
			Window.__init__(self)
			self.SetColor(color)

			self.width  = 100
			self.height = height
			self.minh   = 0
			self.maxh   = height

			self.components = []

		def __del__(self):
			Window.__del__(self)

		def Destroy(self):
			Window.Destroy(self)

			self.components = None

		# Set Background Color
		def SetColor(self, color = 0xff0099ff):
			self.color = color

		def SetParent(self, parent):
			Window.SetParent(self, parent)
			self.parent=proxy(parent)

		# Functions for Height and Width
		def SetHeight(self,h):
			self.SetSize(self.width,h)
		def SetWidth(self,w):
			self.SetSize(w,self.height)
		def SetSize(self,w,h):
			self.width  = w
			self.height = h
			self.maxh   = h
			Window.SetSize(self,w,h)

		# Set the minimum y Position for Rendering
		def SetRenderMin(self, minh):
			self.minh = minh
			self.maxh = self.height
			self.RecalculateRenderedComponents()

		def SetRenderMax(self, maxh):
			self.maxh = maxh
			self.minh = 0
			self.RecalculateRenderedComponents()

		def RegisterComponent(self,component):
			mtype       = type(component).__name__
			if mtype == "Bar":
				(x,y, w,h)            = component.GetRect()
				(x,y)                 = component.GetLocalPosition()
				component.__list_data = [x,y,w,h]
			self.components.append(component)

		def UnregisterComponent(self,component):
			self.components.remove(component)
			if component.__list_data:
				component.__list_data = None

		def RecalculateRenderedComponents(self):
			for component in self.components:
				# Get Size and Position
				(xl,yl)    = component.GetLocalPosition()
				(x,y, w,h) = component.GetRect()
				mtype       = type(component).__name__
				if mtype == "TextLine":
					(w,h) = component.GetTextSize()


				if yl + h < self.minh:
					# Komponente ist nicht sichtbar (oben)
					component.Hide()
				elif yl > self.maxh:
					# Komponente ist nicht sichtbar (unten)
					component.Hide()
				else:
					if mtype == "ExpandedImageBox":
						miny = 0
						if self.minh > 0 and yl < self.minh:
							miny = -float(self.minh-yl)/float(h)

						maxy = 0
						if h != 0:
							maxy = float(self.maxh-yl-h)/float(h)

						maxy = min(0,max(-1,maxy))

						component.SetRenderingRect(0.0,miny,0.0, maxy)
						component.Show()
					else:
						if yl < self.minh or yl + h > self.maxh:
							component.Hide()
						else:
							component.Show()

		# Lulz, I got Clicked!
		def OnMouseLeftButtonDown(self):
			self.parent.SelectItem(self)

		def OnRender(self):
			x, y = self.GetGlobalPosition()
			grp.SetColor(self.color)
			grp.RenderBar(x, y+self.minh, self.GetWidth(), self.maxh-self.minh)

			# if self.parent.GetSelectedItem()==self:
				# self.OnSelectedRender()

		# def OnSelectedRender(self):
			# x, y = self.GetGlobalPosition()
			# grp.SetColor(grp.GenerateColor(0.0, 0.0, 0.7, 0.7))
			# grp.RenderBar(x, y, self.GetWidth(), self.GetHeight())
	def __init__(self):
		Window.__init__(self)

		self.items     = []         # Itemlist
		self.selected  = None       # Current selected Item
		self.basePos   = 0          # Scroll Position
		self.itemWidth = 100
		self.itemStep  = 4         # Step between 2 items
		self.scrollbar = None

		self.isModernScrollBar = False
		self.selectEvent = None     # Fired when an item gets selected

	def Destroy(self):
		Window.Destroy(self)

		self.items = None

	def SetItemStep(self, itemStep):
		self.itemStep = itemStep

	def SetSize(self,w,h):
		Window.SetSize(self,w,h)
		self.SetItemWidth(w)

		self.UpdateList()

	def SetScrollBar(self, scrollbar, isModernScrollBar = False):
		self.scrollbar = scrollbar
		self.scrollbar.SetScrollEvent(__mem_func__(self.__OnScroll))
		self.scrollbar.SetScrollStep(0.10)

		self.isModernScrollBar = isModernScrollBar

		self.UpdateList()

	def CalcTotalItemHeight(self):
		total_height = 0
		for item in self.items:
			total_height += item.GetHeight()
			total_height += self.itemStep

		# if total_height > self.itemStep:
			# total_height -= 2* self.itemStep
		return total_height

	def ConfigureScrollBar(self):
		if self.scrollbar:
			itemheight = self.CalcTotalItemHeight()
			myheight   = self.GetHeight()- 2 * self.itemStep
			dif = 0.97
			if itemheight > myheight and itemheight != 0:
				dif = 1.0 * myheight / itemheight

			self.scrollbar.SetMiddleBarSize(dif)

	def __OnScroll(self):
		pos   = self.scrollbar.GetPos()
		toscr = self.CalcTotalItemHeight() - self.GetHeight() + 2* self.itemStep
		self.basePos = toscr * pos

		self.UpdateList()

	def SelectItem(self,item):
		self.selected = item

		if self.selectEvent:
			self.selectEvent(item)


	def AppendItem(self,item):
		item.SetParent(self)
		item.SetWidth(self.itemWidth);
		item.Show()
		self.items.append(item)

		self.UpdateList()

	def RemoveItem(self,item):
		self.items.remove(item)
		self.UpdateList()

	def UpdateList(self):
		self.ConfigureScrollBar()
		self.RecalcItemPositions()

	def IsEmpty(self):
		if len(self.itemList)==0:
			return 1
		return 0

	def SetItemWidth(self,w):
		self.itemWidth = w
		for item in self.items:
			item.SetWidth(w)

	# Set the Item-Positions
	def RecalcItemPositions(self):
		curbp = self.basePos

		itemheight = self.CalcTotalItemHeight()
		myheight   = self.GetHeight() - 2 * self.itemStep

		if itemheight < myheight:
			curbp = 0
			return




		fromPos = curbp
		curPos  = 0
		toPos   = curbp + self.GetHeight()
		for item in self.items:
			hw = item.GetHeight()
			if curPos+hw < fromPos:
				# Item ist nicht zu sehen (oben)
				# item.SetColor(0xffff0000)
				item.Hide()
			elif curPos < fromPos and curPos+hw> fromPos:
				# Item ist nur teilweise zu sehen (oben)
				# item.SetColor(0xffffcc00)
				item.SetRenderMin(fromPos-curPos)
				item.Show()
			elif curPos < toPos and curPos+hw > toPos:
				# Item ist nur teilweise zu sehen (unten)
				# item.SetColor(0xffffcc00)
				item.SetRenderMax(toPos-curPos)
				item.Show()
			elif curPos > toPos:
				# Item ist nicht zu sehen (unten)
				# item.SetColor(0xffff0000)
				item.Hide()
			else:
				# Item vollstandig sichtbar
				# item.SetColor(0xff00ff00)
				item.SetRenderMin(0)
				item.Show()

			item.SetPosition(0,curPos- fromPos)
			curPos+= hw+self.itemStep

	def OnMouseWheel(self, len):
		if not self.scrollbar:
			return False

		if not self.isModernScrollBar:
			return False

		return self.scrollbar.OnMouseWheel(len)


class GradingBar(Window):
	def __init__(self):
		Window.__init__(self)

		self.startColor = 0
		self.endColor = 0

	def __del__(self):
		Window.__del__(self)

	def SetColor(self, startColor, endColor):
		self.SetStartColor(startColor)
		self.SetEndColor(endColor)

	def SetStartColor(self, startColor):
		self.startColor = startColor

	def SetEndColor(self, endColor):
		self.endColor = endColor

	def OnRender(self):
		(x, y) = self.GetGlobalPosition()
		grp.RenderGradationBar(x, y, self.GetWidth(), self.GetHeight(), self.startColor, self.endColor)
		
if app.ENABLE_BATTLEPASS:
	class SlimScrollBar(Window):

		SCROLLBAR_WIDTH = 13
		SCROLLBAR_MIDDLE_HEIGHT = 1
		SCROLLBAR_BUTTON_WIDTH = 17
		SCROLLBAR_BUTTON_HEIGHT = 17
		SCROLL_BTN_XDIST = 1
		SCROLL_BTN_YDIST = 2

		class MiddleBar(DragButton):
			def __init__(self):
				DragButton.__init__(self)
				self.AddFlag("movable")

				self.SetWindowName("NONAME_ScrollBar_MiddleBar")

			def MakeImage(self):
				top = ImageBox()
				top.SetParent(self)
				top.LoadImage("battlepass/scrollbar/ScrollBar_Middle_Top_bottom.tga")
				top.AddFlag("not_pick")
				top.SetPosition(0, 0)
				top.Show()
				topScale = ExpandedImageBox()
				topScale.SetParent(self)
				topScale.SetPosition(0, top.GetHeight())
				topScale.LoadImage("battlepass/scrollbar/ScrollBar_Middle_TopScale.tga")
				topScale.AddFlag("not_pick")
				topScale.Show()

				bottom = ImageBox()
				bottom.SetParent(self)
				bottom.LoadImage("battlepass/scrollbar/ScrollBar_Middle_Top_bottom.tga")
				bottom.AddFlag("not_pick")
				bottom.Show()
				bottomScale = ExpandedImageBox()
				bottomScale.SetParent(self)
				bottomScale.LoadImage("battlepass/scrollbar/ScrollBar_Middle_TopScale.tga")
				bottomScale.AddFlag("not_pick")
				bottomScale.Show()

				middle = ExpandedImageBox()
				middle.SetParent(self)
				middle.LoadImage("battlepass/scrollbar/ScrollBar_Middle_Middle.tga")
				middle.AddFlag("not_pick")
				middle.Show()

				self.top = top
				self.topScale = topScale
				self.bottom = bottom
				self.bottomScale = bottomScale
				self.middle = middle

			def SetSize(self, height):
				minHeight = self.top.GetHeight() + self.bottom.GetHeight() + self.middle.GetHeight()
				height = max(minHeight, height)
				DragButton.SetSize(self, 10, height)

				scale = (height - minHeight) / 2
				extraScale = 0
				if (height - minHeight) % 2 == 1:
					extraScale = 1

				self.topScale.SetRenderingRect(0, 0, 0, scale - 1)
				self.middle.SetPosition(0, self.top.GetHeight() + scale)
				self.bottomScale.SetPosition(0, self.middle.GetBottom())
				self.bottomScale.SetRenderingRect(0, 0, 0, scale - 1 + extraScale)
				self.bottom.SetPosition(0, height - self.bottom.GetHeight())

		def __init__(self):
			Window.__init__(self)

			self.pageSize = 1
			self.curPos = 0.0
			self.eventScroll = None
			self.eventArgs = None
			self.lockFlag = False

			self.CreateScrollBar()
			self.SetScrollBarSize(0)

			self.scrollStep = 0.20
			self.SetWindowName("NONAME_ScrollBar")

		def __del__(self):
			Window.__del__(self)

		def CreateScrollBar(self):
			middleImage = ExpandedImageBox()
			middleImage.SetParent(self)
			middleImage.AddFlag("not_pick")
			middleImage.SetPosition(0, 1)
			middleImage.LoadImage("battlepass/scrollbar/SlimScrollBar_Middle.tga")
			middleImage.Show()
			self.middleImage = middleImage

			middleBar = self.MiddleBar()
			middleBar.SetParent(self)
			middleBar.SetMoveEvent(__mem_func__(self.OnMove))
			middleBar.Show()
			middleBar.MakeImage()
			middleBar.SetSize(0) # set min height
			self.middleBar = middleBar

		def Destroy(self):
			self.eventScroll = None
			self.eventArgs = None

		def SetScrollEvent(self, event, *args):
			self.eventScroll = event
			self.eventArgs = args

		def GetParent(self):
			return self.GetParentProxy()

		def SetMiddleBarSize(self, pageScale):
			self.middleBar.SetSize(int(pageScale * float(self.GetHeight() - self.SCROLL_BTN_YDIST*2)))
			realHeight = self.GetHeight() - self.SCROLL_BTN_YDIST*2 - self.middleBar.GetHeight()
			self.pageSize = realHeight

		def SetScrollBarSize(self, height):
			self.SetSize(self.SCROLLBAR_WIDTH, height)

			self.pageSize = height - self.SCROLL_BTN_YDIST*2 - self.middleBar.GetHeight()

			middleImageScale = float((height - self.SCROLL_BTN_YDIST*2) - self.middleImage.GetHeight()) / float(self.middleImage.GetHeight())
			self.middleImage.SetRenderingRect(0, 0, 0, middleImageScale)

			self.middleBar.SetRestrictMovementArea(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST, \
				self.middleBar.GetWidth(), height - self.SCROLL_BTN_YDIST * 2)
			self.middleBar.SetPosition(self.SCROLL_BTN_XDIST, self.SCROLL_BTN_YDIST)

		def SetScrollStep(self, step):
			self.scrollStep = step

		def GetScrollStep(self):
			return self.scrollStep

		def GetPos(self):
			return self.curPos

		def OnUp(self):
			self.SetPos(self.curPos-self.scrollStep)

		def OnDown(self):
			self.SetPos(self.curPos+self.scrollStep)

		def SetPos(self, pos, moveEvent = True):
			pos = max(0.0, pos)
			pos = min(1.0, pos)

			newPos = float(self.pageSize) * pos
			self.middleBar.SetPosition(self.SCROLL_BTN_XDIST, int(newPos) + self.SCROLL_BTN_YDIST)
			if moveEvent == True:
				self.OnMove()

		def OnMove(self):
			if self.lockFlag:
				return

			if 0 == self.pageSize:
				return

			(xLocal, yLocal) = self.middleBar.GetLocalPosition()
			self.curPos = float(yLocal - self.SCROLL_BTN_YDIST) / float(self.pageSize)

			if self.eventScroll:
				apply(self.eventScroll, self.eventArgs)

		def OnMouseLeftButtonDown(self):
			(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
			newPos = float(yMouseLocalPosition) / float(self.GetHeight())
			self.SetPos(newPos)

		def LockScroll(self):
			self.lockFlag = True

		def UnlockScroll(self):
			self.lockFlag = False
class BoxedBoard(Window):
	BORDER_TOP = 0
	BORDER_RIGHT = 1
	BORDER_BOTTOM = 2
	BORDER_LEFT = 3

	DEFAULT_BORDER_COLOR = grp.GenerateColor(0.3, 0.3, 0.3, 1.0)
	DEFAULT_BASE_COLOR = grp.GenerateColor(0, 0, 0, 0.5)

	def __init__(self):
		Window.__init__(self)

		self.borderSize = 1

		# Create Borders
		self.borders = [
			Bar(),
			Bar(),
			Bar(),
			Bar()
		]

		for border in self.borders:
			border.SetParent(self)
			border.AddFlag("not_pick")
			border.Show()

		# Create Base
		self.base = Bar()
		self.base.SetParent(self)
		self.base.AddFlag("not_pick")
		self.base.Show()

		# Set Default Colors
		self.SetBorderColor(self.DEFAULT_BORDER_COLOR)
		self.SetBaseColor(self.DEFAULT_BASE_COLOR)

	def __del__(self):
		self.Destroy()
		Window.__del__(self)

	def Destroy(self):
		del self.borders[:]
		self.base = None

		Window.Destroy(self)

	def SetBorderColor(self, color):
		for border in self.borders:
			border.SetColor(color)

	def SetBorderSize(self, borderSize):
		self.borderSize = borderSize
		self.SetSize(self.GetWidth(), self.GetHeight())

	def SetBaseColor(self, color):
		self.base.SetColor(color)

	def SetSize(self, width, height):
		width = max(width, (2 * self.borderSize) + 1)
		height = max(height, (2 * self.borderSize) + 1)

		Window.SetSize(self, width, height)
		self.UpdateBoard()

	def UpdateBoard(self):
		width = self.GetWidth()
		height = self.GetHeight()

		top, right, bottom, left = self.borders

		# Top Border
		top.SetSize(width - self.borderSize, self.borderSize)

		# Right Border
		right.SetSize(self.borderSize, height - self.borderSize)
		right.SetPosition(width - self.borderSize, 0)

		# Bottom Border
		bottom.SetSize(width - self.borderSize, self.borderSize)
		bottom.SetPosition(self.borderSize, height - self.borderSize)

		# Left Border
		left.SetSize(self.borderSize, height - self.borderSize)
		left.SetPosition(0, self.borderSize)

		# Base
		self.base.SetSize(width - (2 * self.borderSize), height - (2 * self.borderSize))
		self.base.SetPosition(self.borderSize, self.borderSize)
		
