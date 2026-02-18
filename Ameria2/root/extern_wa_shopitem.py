# coding: latin_1

import item
import ui
import shop
import localeInfo
import app
#---<UiTooltip Functiones>---#
#----------------------------#
#----------------------------#

def FuncCheckPrice(self,slotIndex,price):
    self.AppendPrice(price)

def AppendPriceItem(self,vnum_special,count_special):
    self.AppendSpace(5)
        
    item.SelectItem(vnum_special)

    itemImage = ui.ImageBox()
    itemImage.SetParent(self)
    itemImage.LoadImage(item.GetIconImageFileName())
    itemImage.SetPosition((self.toolTipWidth/2)-10, self.toolTipHeight)
    itemImage.Show()
    
    itemName = ui.TextLine()
    itemName.SetParent(itemImage)
    itemName.SetHorizontalAlignCenter()
    itemName.SetPosition(17,itemImage.GetHeight()+6)
    itemName.SetText(item.GetItemName())
    itemName.Show()
    
    itemCount = ui.TextLine()
    itemCount.SetParent(itemImage)
    itemCount.SetHorizontalAlignCenter()
    itemCount.SetPosition(18,itemImage.GetHeight()+17)
    itemCount.SetText(localeInfo.PRICE02 %count_special)
    itemCount.Show()

    self.toolTipHeight += itemImage.GetHeight()+28
    self.childrenList.append(itemImage)
    self.childrenList.append(itemName)
    self.childrenList.append(itemCount)
    self.ResizeToolTip()

#----------------------------#
#----------------------------#
#---<UiTooltip Functiones>---#


#---<LocaleInfo Functiones>--#
#----------------------------#
#----------------------------#

def FuncCheckMsg(slotPos,itemName,itemCount,itemPrice):
    return localeInfo.DO_YOU_BUY_ITEM(itemName, itemCount, localeInfo.NumberToMoneyString(itemPrice))
        
def DO_YOU_BUY_ITEM_SPECIAL(buyItemName, buyItemCount, buyItemVnumSpecial, buyItemSpecialCount):        
    item.SelectItem(buyItemVnumSpecial)
    itemName = item.GetItemName()

    if buyItemCount > 1 :
        return "Do you want to buy %s x%d for %s x%d"%(buyItemName,buyItemCount,itemName,buyItemSpecialCount)
    else:
        return "Do you want to buy %s for %s x%d"%(buyItemName,itemName,buyItemSpecialCount)

#---<LocaleInfo Functiones>--#
#----------------------------#
#----------------------------#

#---<Game Functiones>--#
#----------------------------#
#----------------------------#

def ShopErrorDict(type):
    return localeInfo.SHOP_ERROR_DICT[type]

#---<Game Functiones>--#
#----------------------------#
#----------------------------#