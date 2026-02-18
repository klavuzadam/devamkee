# coding: latin_1

import uiScriptLocale

BOARD_WIDTH = 242
BOARD_HEIGHT = 427

IMG_PATH = "d:/ymir work/ui/game/v2_shopsearch/"

window = {
    "name" : "ShopSearchBuyWindow",
    "x" : 0,
    "y" : 0,
    "style" : ("movable", "float",),
    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGHT,
    "children" :
    (
        {
            "name" : "board_ui",
            "type" : "image",
            "style" : ("attach",),
            "x" : 0,
            "y" : 0,
            "image" : IMG_PATH + "buy/bg.png",
            "children" :
            (
                {
                    "name" : "msg1_buy_ui",
                    "type" : "text",
                    "x" : 0,
                    "y" : 30,
                    "text" : uiScriptLocale.V2_SHOP_SEARCH_ALERT,
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text_vertical_align" : "center",
                },
                {
                    "name" : "item_bg_ui",
                    "type" : "image",
                    "style" : ("attach",),
                    "x" : 0,
                    "y" : 46,
                    "horizontal_align" : "center",
                    "image" : IMG_PATH + "buy/item_bg.png",
                    "children" :
                    (
                        {
                            "name" : "item_slot1_ui",
                            "type" : "image",
                            "x" : 0,
                            "y" : 0,
                            "horizontal_align" : "center",
                            "vertical_align" : "center",
                            "image" : IMG_PATH + "slot/_1.png",
                        },
                        {
                            "name" : "item_slot2_ui",
                            "type" : "image",
                            "x" : 0,
                            "y" : 0,
                            "horizontal_align" : "center",
                            "vertical_align" : "center",
                            "image" : IMG_PATH + "slot/_2.png",
                        },
                        {
                            "name" : "item_slot3_ui",
                            "type" : "image",
                            "x" : 0,
                            "y" : 0,
                            "horizontal_align" : "center",
                            "vertical_align" : "center",
                            "image" : IMG_PATH + "slot/_3.png",
                        },
                        {
                            "name" : "item_ico_ui",
                            "type" : "image",
                            "x" : 0,
                            "y" : 0,
                            "horizontal_align" : "center",
                            "vertical_align" : "center",
                            "image" : IMG_PATH + "money.png",
                        },
                    ),
                },
                {
                    "name" : "msg2_buy_ui",
                    "type" : "text",
                    "x" : 0,
                    "y" : 168,
                    "text" : "",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text_vertical_align" : "center",
                },
                {
                    "name" : "msg3_buy_ui",
                    "type" : "text",
                    "x" : 0,
                    "y" : 202,
                    "text" : "",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text_vertical_align" : "center",
                },
                {
                    "name" : "msg4_buy_ui",
                    "type" : "text",
                    "x" : 0,
                    "y" : 218,
                    "text" : "",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text_vertical_align" : "center",
                },
                {
                    "name" : "msg5_buy_ui",
                    "type" : "text",
                    "x" : 0,
                    "y" : 250,
                    "text" : "",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text_vertical_align" : "center",
                },
                {
                    "name" : "buy_button_ui",
                    "type" : "button",
                    "x" : -48,
                    "y" : 250,
                    "text" : uiScriptLocale.V2_SHOP_SEARCH_BUY,
                    "horizontal_align" : "center",
                    "default_image" : IMG_PATH + "buy/btn_normal.png",
                    "over_image" : IMG_PATH + "buy/btn_hover.png",
                    "down_image" : IMG_PATH + "buy/btn_down.png",
                },
                {
                    "name" : "close_button_ui",
                    "type" : "button",
                    "x" : 48,
                    "y" : 250,
                    "text" : uiScriptLocale.V2_SHOP_SEARCH_CLOSE,
                    "horizontal_align" : "center",
                    "default_image" : IMG_PATH + "buy/btn_normal.png",
                    "over_image" : IMG_PATH + "buy/btn_hover.png",
                    "down_image" : IMG_PATH + "buy/btn_down.png",
                },
            ),
        },
    ),
}
