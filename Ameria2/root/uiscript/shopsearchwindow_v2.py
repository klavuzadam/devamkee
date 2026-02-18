# coding: latin_1

import uiScriptLocale

BOARD_WIDTH = 790
BOARD_HEIGHT = 523

MENU_WIDTH = 182
MENU_WIDTH2 = 205
MENU_HEIGHT = BOARD_HEIGHT - 107

MENU_Y = 95 + 2

BG_IMG = "background.png"

if USE_PREMIUM_AFFECT:
	MENU_HEIGHT -= 29
	MENU_Y += 29
	BG_IMG = "background_v2.png"

CONTENT_WIDTH = 546
CONTENT_HEIGHT = 415
# CONTENT_HEIGHT = MENU_HEIGHT - 28

IMG_PATH = "d:/ymir work/ui/game/v2_shopsearch/"
ROOT_PATH = "d:/ymir work/ui/game/newsearch/"
window = {
    "name" : "ShopSearchWindow",
    "x" : 0,
    "y" : 0,
    "style" : ("movable", "float",),
    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGHT,
    "children" :
    (
        {
            "name" : "board_ui",
            "type" : "board_with_titlebar",
            "style" : ("attach",),
            "x" : 0,
            "y" : 0,
            "width" : BOARD_WIDTH,
            "height" : BOARD_HEIGHT,
            "title" : uiScriptLocale.V2_SHOP_SEARCH_TITLE,
            "children" :
            (
                {
                    "name" : "bgall",
                    "type" : "image",
                    "style" : ("attach",),
                    "x" : -2,
                    "y" : 11,
                    "horizontal_align" : "center",
                    "vertical_align" : "center",
                    "image" : ROOT_PATH + BG_IMG,
                },
                {
                    "name" : "menu_ui",
                    "type" : "v3_thinboard",
                    "style" : ("attach",),
                    "x" : 10,
                    "y" : MENU_Y,
                    "width" : MENU_WIDTH2,
                    "height" : MENU_HEIGHT,
                },
                {
                    "name" : "content_ui",
                    "type" : "v3_thinboard",
                    "style" : ("attach",),
                    "x" : 36 + MENU_WIDTH + 10 + 3,
                    "y" : 67 + 2,
                    "width" : CONTENT_WIDTH,
                    "height" : CONTENT_HEIGHT,
                    "children" :
                    (
                        {
                            "name" : "loading_content_ui",
                            "type" : "ani_image",
                            "x" : 0,
                            "y" : 0,
                            "horizontal_align" : "center",
                            "vertical_align" : "center",
                            "delay" : 6,
                            "images" :
                            (
                                IMG_PATH + "loading_ani/00.dds",
                                IMG_PATH + "loading_ani/01.dds",
                                IMG_PATH + "loading_ani/02.dds",
                                IMG_PATH + "loading_ani/03.dds",
                                IMG_PATH + "loading_ani/04.dds",
                                IMG_PATH + "loading_ani/05.dds",
                                IMG_PATH + "loading_ani/06.dds",
                                IMG_PATH + "loading_ani/07.dds",
                            ),
                        },
                        {
                            "name" : "empty_content_ui",
                            "type" : "text",
                            "x" : 0,
                            "y" : 3,
                            "text" : uiScriptLocale.V2_SHOP_SEARCH_EMPTY_RESULT,
                            "all_align" : 1,
                        },
                    ),
                },
                {
                    "name": "buy_all_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": uiScriptLocale.V2_SHOP_SEARCH_BUY_SELECTED,
                    "default_image": IMG_PATH + "filter/btn_filter_normal.png",
                    "over_image": IMG_PATH + "filter/btn_filter_hover.png",
                    "down_image": IMG_PATH + "filter/btn_filter_down.png",
                },
                {
                    "name": "first_prev_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "<<",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "prev_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "<",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "page1_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "1",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "page2_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "2",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "page3_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "3",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "page4_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "4",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "page5_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": "5",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "next_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": ">",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
                {
                    "name": "last_next_button_ui",
                    "type": "button",
                    "x": 0,
                    "y": 0,
                    "text": ">>",
                    "fontname" : "Verdana:14b",
                    "default_image": ROOT_PATH + "right_rectangle/pagination_button_norm.png",
                    "over_image": ROOT_PATH + "right_rectangle/pagination_button_over.png",
                    "down_image": ROOT_PATH + "right_rectangle/pagination_button_down.png",
                },
            ),
        },
    ),
}
