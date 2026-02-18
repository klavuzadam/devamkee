# coding: latin_1

import uiScriptLocale
import localeInfo

BOARDWIDTH = 133
BOARDHEIGHT = 140

MAINFOLDER = "locale/es/ui/"
IMG_ITEMSHOP = "Itemshop/"

window = {
    "name" : "ItemBox",

    "x" : SCREEN_WIDTH/2-BOARDWIDTH/2,
    "y" : SCREEN_HEIGHT /2-BOARDHEIGHT/2,

    "style" : ("float",),

    "width" : BOARDWIDTH,
    "height" : BOARDHEIGHT,

    "children" :
    (
        {
            "name" : "Background",
            "type" : "image",
            "style" : ("attach",),

            "x" : 0,
            "y" : 0,

            "image" : IMG_ITEMSHOP + "slot_item.tga",

            "children" :
            (
                {
                    "name" : "TimeBox",
                    "type" : "image",
                    "x" : 0,
                    "y" : -19,
                    "image" : MAINFOLDER+"itemshop/grey_medium_slotbar.sub",

                    "children" :
                    (
                        ## Countdown
                        {
                            "name" : "tx_countdown",
                            "type" : "text",
                            "text" : "Xh Xm Xs",
                            "x" : 20,
                            "y" : 3,
                        },
                    ),
                },
                ## Itemname
                {
                    "name" : "tx_itemName",
                    "type" : "text",
                    "text" : "XXX",
                    "x" : 65,
                    "y" : 5,
                    "text_horizontal_align" : "center",
                                    
                },
                ## Item icon

                {
                    "name" : "icon_item",
                    "type" : "expanded_image",
                    "x" : 17,
                    "y" : 32,
                    "image" : MAINFOLDER+"itemshop/item_icon_box.sub",
                },

                {
                    "name" : "icon_price",
                    "type" : "expanded_image",
                    "x" : 61,
                    "y" : 46,
                    "image" : IMG_ITEMSHOP+"icon_vcoin.tga",
                },


                {
                    "name" : "tx_itemPrice_title",
                    "type" : "text",
                    "text" : localeInfo.ITEMSHOP_PRICE,
                    "x" : 75,
                    "y" : 28,
                },

                {
                    "name" : "tx_itemPrice",
                    "type" : "text",
                    "text" : "XXX COINS",
                    "text_horizontal_align":"center",
                    "x" : 90,
                    "y" : 44,
                },
                ## buy button
                {
                    "name" : "btn_buy", 
                    "type" : "button",
                    "x" : 57,
                    "y" : 110,
                    "text" : localeInfo.ITEMSHOP_BUY,
                    "default_image" : IMG_ITEMSHOP+"comprar_1.tga",
                    "over_image" : IMG_ITEMSHOP+"comprar_2.tga",
                    "down_image" : IMG_ITEMSHOP+"comprar_3.tga",
                },
                ## amount
                {
                    "name" : "sb_amount_title",
                    "type" : "text",
                    "text" : localeInfo.ITEMSHOP_COUNT,
                    "x" : 68,
                    "y" : 62,
                },
                
                {
                    "name" : "sb_amount",
                    "type" : "image",
                    "x" : 59,
                    "y" : 77,
                    
                    "image" : IMG_ITEMSHOP+"precio.tga",

                    "children" :
                    (
                        {
                            "name" : "ed_amount",
                            "type" : "editline",
                                    
                            "width" : 57,
                            "height" : 18,
                            "input_limit" : 3,
                            "enable_codepage" : 0,
                                    
                            "x" : 5,
                            "y" : 1,
                        },
                    ),
                },
                {
                    "name" : "PercentBox",
                    "type" : "image",
                    "x" : 108,
                    "y" : 2,
                    "image" : IMG_ITEMSHOP+"descuento.tga",
                    
                    "children" : 
                    (
                        {
                            "name" : "tx_percent",
                            "type" : "text",
                            "text" : "10%",
                            "outline" : "true",
                            "text_horizontal_align": "center",
                            #"r": 1.000,
                            #"g": 0.549,
                            #"b": 0.000,
                            "x" : 12,
                            "y" : 5,
                        },
                    ),
                },
            ),
        },
    ),
}