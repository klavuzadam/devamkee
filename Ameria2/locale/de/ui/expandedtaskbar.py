# coding: latin_1

import uiScriptLocale
import localeInfo
import app

ROOT = "d:/ymir work/ui/game/"
NEW_DESIGN_FOLDER = "d:/ymir work/ui/game/new_inventory/"


Y_ADD_POSITION = 0
window = {
    "name" : "ExpandTaskBar",
    "style" : ("ltr", ),

    "x" : SCREEN_WIDTH/2 - 5,
    "y" : SCREEN_HEIGHT - 74,

    "width" : 4*40,
    "height" : 37,

    "children" :
    [
        {
            "name" : "ExpanedTaskBar_Board",
            "type" : "window",
            "style" : ("ltr", ),

            "x" : 0,
            "y" : 0,

            "width" : 4*40,
            "height" : 37,

            "children" :
            [
                {
                    "name" : "DragonSoulButton",
                    "type" : "button",
                    "style" : ("ltr", ),

                    "x" : 0,
                    "y" : 0,

                    "width" : 37,
                    "height" : 37,

                    "tooltip_text" : uiScriptLocale.TASKBAR_DRAGON_SOUL,
                            
                    "default_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_01.tga",
                    "over_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_02.tga",
                    "down_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_03.tga",
                },

                {
                    "name" : "SwitchbotButton",
                    "type" : "button",
                    "style" : ("ltr", ),
                    "x" : 40,
                    "y" : 0,
                    "width" : 37,
                    "height" : 37,
                    "tooltip_text" : uiScriptLocale.SWITCHBOT_TITLE,
                    "default_image" : NEW_DESIGN_FOLDER+"switch_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"switch_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"switch_2.tga",
                },
                
                {
                    "name" : "ShopSearchButton",
                    "type" : "button",
                    "style" : ("ltr", ),
                    "x" : 2 * 40,
                    "y" : 0,
                    "width" : 37,
                    "height" : 37,
                    "tooltip_text" : uiScriptLocale.SHOP_SEARCH_TITLE,
                    "default_image" : NEW_DESIGN_FOLDER+"search_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"search_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"search_2.tga",
                },
                
                {
                    "name" : "WikiButton",
                    "type" : "button",
                    "style" : ("ltr", ),
                    "x" : 3 * 40,
                    "y" : 0,
                    "width" : 37,
                    "height" : 37,
                    "tooltip_text" : localeInfo.WIKI_TITLE2,
                    "default_image" : NEW_DESIGN_FOLDER+"wiki_0.tga",
                    "over_image" : NEW_DESIGN_FOLDER+"wiki_1.tga",
                    "down_image" : NEW_DESIGN_FOLDER+"wiki_2.tga",
                },
            ],
        },
    ],
}
# if app.ENABLE_AUTO_SYSTEM:
    # window["width"] = 37*2
    # window["children"][0]["width"] = window["children"][0]["width"] + 37
    # window["children"][0]["children"] = window["children"][0]["children"] + [
                    # {
                        # "name" : "AutoButton",
                        # "type" : "button",
                        # "style" : ("ltr", ),

                        # "x" : 38,
                        # "y" : 0,

                        # "width" : 37,
                        # "height" : 37,

                        # "tooltip_text" : uiScriptLocale.KEYCHANGE_AUTO_WINDOW,
                                
                        # "default_image" : "icon/item/TaskBar_Auto_Button_01.tga",
                        # "over_image" : "icon/item/TaskBar_Auto_Button_02.tga",
                        # "down_image" : "icon/item/TaskBar_Auto_Button_03.tga",
                    # },]

if app.ENABLE_GROWTH_PET_SYSTEM:
    window["width"] = 37*3
    window["children"][0]["width"] = window["children"][0]["width"] + 37
    window["children"][0]["children"] = window["children"][0]["children"] + [
                    {
                        "name" : "PetInfoButton",
                        "type" : "button",
                        "style" : ("ltr", ),

                        "x" : 74,
                        "y" : 0,

                        "width" : 37,
                        "height" : 37,

                        "tooltip_text" : uiScriptLocale.TASKBAR_DISABLE,
                                
                        "default_image" : "d:/ymir work/ui/pet/TaskBar_Pet_Button_01.tga",
                        "over_image" : "d:/ymir work/ui/pet/TaskBar_Pet_Button_02.tga",
                        "down_image" : "d:/ymir work/ui/pet/TaskBar_Pet_Button_03.tga",
                    },]
if app.ENABLE_MONSTER_CARD:
    window["width"] = 37*4
    window["children"][0]["width"] = window["children"][0]["width"] + 37
    window["children"][0]["children"] = window["children"][0]["children"] + [
                    {
                        "name" : "MonsterCardWindow",
                        "type" : "button",
                        "style" : ("ltr", ),

                        "x" : 110,
                        "y" : 0,

                        "width" : 37,
                        "height" : 37,

                        "tooltip_text" : uiScriptLocale.KEYCHANGE_MONSTER_CARD_WINDOW,
                                
                        "default_image" : "icon/item/Mcard_Button_01.tga",
                        "over_image" : "icon/item/Mcard_Button_02.tga",
                        "down_image" : "icon/item/Mcard_Button_03.tga",
                    },]