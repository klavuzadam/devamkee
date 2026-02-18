# coding: latin_1

import uiScriptLocale

window = {
    "name" : "Bank",
    "x" : 0,
    "y" : 0,
    "style" : ("movable", "float", "animate",),
    "width" : 250,
    "height" : 118,
    "children" :
    (
        {
            "name" : "board",
            "type" : "board",
            "x" : 0,
            "y" : 0,
            "width" : 250,
            "height" : 118,
            "children" :
            (
                {
                    "name" : "titlebar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 8,
                    "y" : 8,
 
                    "width" : 234,
                    "color" : "gray",

                    "children" :
                    (
                        {
                            "name" : "TitleName",
                            "type" : "text",
                            "x" : 0,
                            "y" : 3,
                            "horizontal_align" : "center",
                            "text" : "",
                            "text_horizontal_align":"center"
                        },
                    ),
                },
                {
                    "name" : "currentLine1_slot",
                    "type" : "image",
                    "x" : 135,
                    "y" : 34,
                    "image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",
                    "children" :
                    (
                        {
                            "name" : "currentLine1_name",
                            "type" : "text",
                            "x" : -110,
                            "y" : 3,
                            "text" : "",
                        },
                        {
                            "name" : "currentLine1_value",
                            "type" : "editline",
                            "x" : 3,
                            "y" : 3,
                            "width" : 100,
                            "height" : 18,
                            "input_limit" : 12,
                        },
                    ),
                },
                {
                    "name" : "currentLine2_slot",
                    "type" : "image",
                    "x" : 135,
                    "y" : 59,
                    "image" : "d:/ymir work/ui/public/Parameter_Slot_03.sub",
                    "children" :
                    (
                        {
                            "name" : "currentLine2_name",
                            "type" : "text",
                            "x" : -110,
                            "y" : 3,
                            "text" : "",
                        },
                        {
                            "name" : "currentLine2_value",
                            "type" : "editline",
                            "x" : 25,
                            "y" : 3,
                            "width" : 100,
                            "height" : 18,
                            "input_limit" : 4,
                        },
                        {
                            "name" : "password_random",
                            "type" : "button",
                            "x" : 37,
                            "y" : 2,
                            "text" : "",
                            "tooltip_text" : "ßáãÉ ãÑæÑ ÚÔæÇÆíå",
                            "horizontal_align" : "center",
                            "default_image" : "d:/ymir work/ui/public/bank/btn_plus_01.tga",
                            "over_image" : "d:/ymir work/ui/public/bank/btn_plus_02.tga",
                            "down_image" : "d:/ymir work/ui/public/bank/btn_plus_03.tga",
                        },    
                    ),
                },    
                
                {
                    "name" : "accept_button",
                    "type" : "button",

                    "x" : -35,
                    "y" : 87,

                    "text" : "",
                    "horizontal_align" : "center",

                    "default_image" : "d:/ymir work/ui/game/pvp_advanced/acceptbutton00.tga",
                    "over_image" : "d:/ymir work/ui/game/pvp_advanced/acceptbutton01.tga",
                    "down_image" : "d:/ymir work/ui/game/pvp_advanced/acceptbutton02.tga",
                },
                {
                    "name" : "cancel_button",
                    "type" : "button",

                    "x" : 35,
                    "y" : 87,
                    "horizontal_align" : "center",

                    "text" : "",

                    "default_image" : "d:/ymir work/ui/game/pvp_advanced/canclebutton00.tga",
                    "over_image" : "d:/ymir work/ui/game/pvp_advanced/canclebutton01.tga",
                    "down_image" : "d:/ymir work/ui/game/pvp_advanced/canclebutton02.tga",
                },
            ),
        },
    ),
}