# coding: latin_1

import uiScriptLocale
# import wndMgr
ROOT_PATH = "d:/ymir work/ui/public/"
window = {
    "name" : "SwitchChannelWindow",

    "x" : (SCREEN_WIDTH - 180) / 2,
    "y" : (SCREEN_HEIGHT - 200) / 2,

    "style" : ("movable", "float", "animate",),

    "width" : 180,
    "height" : 200,

    "children" :
    (
        {
            "name" : "board",
            "type" : "board_with_titlebar",

            "x" : 0,
            "y" : 0,

            "width" : 180,
            "height" : 200,
            "title" : uiScriptLocale.SWITCH_CHANNEL,
        },
        {
            "name" : "channel",
            "type" : "button",

            "x" : 25,
            "y" : 40,

            "text" : "Channel X",

            "default_image" : ROOT_PATH + "large_button_01.sub",
            "over_image" : ROOT_PATH + "large_button_02.sub",
            "down_image" : ROOT_PATH + "large_button_03.sub",
        },
        # {
            # "name" : "cancel",
            # "type" : "button",

            # "x" : 50,
            # "y" : 165,

            # "width" : 41,
            # "height" : 21,

            # "text" : uiScriptLocale.CANCEL,

            # "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
            # "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
            # "down_image" : "d:/ymir work/ui/public/small_button_03.sub",
        # },
    )
}