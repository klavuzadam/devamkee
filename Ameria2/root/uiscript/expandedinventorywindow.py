# coding: latin_1

import localeInfo
import uiScriptLocale
import item

WIDTH = 40
HEIGHT = 168+40+40

START_WIDTH = 0
START_HEIGHT = 13
STEP_HEIGHT = 33

NEW_DESIGN_FOLDER = "d:/ymir work/ui/game/new_inventory/"

window = {
    "name" : "ExpandedInventoryWindow",
    "x" : 0,
    "y" : 0,
    "width" : WIDTH,
    "height" : HEIGHT,
    "children" :
    (
        {
            "name" : "ExpandedInventoryBoard",
            "type" : "window",
            "style" : ("attach", "float", "animate",),
            "x" : 0,
            "y" : 0,
            "width" : WIDTH,
            "height" : HEIGHT,
            "children" :
            (
                {
                    "name" : "btn_0",
                    "type" : "button",
                    "x" : START_WIDTH,
                    "y" : START_HEIGHT,
                    "tooltip_text" : uiScriptLocale.COSTUME_TITLE,
                    "tooltip_x": -45,
                    "tooltip_y": 7,
                    "default_image" : NEW_DESIGN_FOLDER+"Costume_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"Costume_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"Costume_2.tga",
                },

                {
                    "name" : "btn_1",
                    "type" : "button",
                    "x" : START_WIDTH,
                    "y" : START_HEIGHT+(STEP_HEIGHT*1),
                    "tooltip_text" : uiScriptLocale.SPECIAL_STORAGE,
                    "tooltip_x": -45,
                    "tooltip_y": 7,
                    "default_image" : NEW_DESIGN_FOLDER+"storage_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"storage_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"storage_2.tga",
                },
                
                {
                    "name" : "btn_2",
                    "type" : "button",
                    "x" : START_WIDTH,
                    "y" : START_HEIGHT+(STEP_HEIGHT*2),
                    "tooltip_text" : uiScriptLocale.SWITCHBOT_TITLE,
                    "tooltip_x": -45,
                    "tooltip_y": 7,
                    "default_image" : NEW_DESIGN_FOLDER+"switch_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"switch_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"switch_2.tga",
                },
                
                {
                    "name" : "btn_3",
                    "type" : "button",
                    "x" : START_WIDTH,
                    "y" : START_HEIGHT+(STEP_HEIGHT*3),
                    "tooltip_text" : uiScriptLocale.OFFLINE_SHOP_TITLE,
                    "tooltip_x": -45,
                    "tooltip_y": 7,
                    "default_image" : NEW_DESIGN_FOLDER+"shop_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"shop_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"shop_2.tga",
                },
                
                {
                    "name" : "btn_4",
                    "type" : "button",
                    "x" : START_WIDTH,
                    "y" : START_HEIGHT+(STEP_HEIGHT*4),
                    "tooltip_text" : uiScriptLocale.SHOP_SEARCH_TITLE,
                    "tooltip_x": -45,
                    "tooltip_y": 7,
                    "default_image" : NEW_DESIGN_FOLDER+"search_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"search_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"search_2.tga",
                },
                {
                    "name" : "btn_5",
                    "type" : "button",
                    "x" : START_WIDTH,
                    "y" : START_HEIGHT+(STEP_HEIGHT*5),
                    "tooltip_text" : localeInfo.WIKI_TITLE2,
                    "tooltip_x": -45,
                    "tooltip_y": 7,
                    "default_image" : NEW_DESIGN_FOLDER+"wiki_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"wiki_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"wiki_2.tga",
                },
            )
        },

    ),
}
