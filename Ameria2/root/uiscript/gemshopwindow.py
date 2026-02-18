# coding: latin_1

import localeInfo

IMG_DIR = "d:/ymir work/ui/gemshop/"

WINDOW_X = 259
WINDOW_Y = 335

window = {
    "name" : "GemShopWindows",
    "x" : 0,
    "y" : 0,
    "style" : ("movable", "float"),
    "width" : WINDOW_X,
    "height" : WINDOW_Y,
    "children" :
    (
        {
            "name" : "board",
            "type" : "board_with_titlebar",
            "style" : ("attach",),
            "x" : 0,
            "y" : 0,
            "width" :WINDOW_X,
            "height" : WINDOW_Y,
            "title":"Gaya Shop",
            "children" :
            (
                {
                    "name" : "bg_slots",
                    "type" : "image",
                    "style" : ("not_pick",),
                    "x" : 16,
                    "y" : 33,
                    "image" : IMG_DIR+"gemshop_new.png",
                },
            ),
        },
    ),
}