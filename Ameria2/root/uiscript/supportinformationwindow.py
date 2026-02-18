# coding: latin_1

import uiScriptLocale

SUPPORT_BG_WIDTH    = 269
SUPPORT_BG_HEIGHT    = 284
PATCH_SUPPORT = "support/"

window = {
    "name" : "SupportInformationWindow",
    "style" : ("movable", "float", "animate",),
    
    "x" : SCREEN_WIDTH - 176 -200 -146 -145,
    "y" : SCREEN_HEIGHT - 37 - 565,

    "width" : SUPPORT_BG_WIDTH,
    "height" : SUPPORT_BG_HEIGHT,

    "children" :
    (
        {
            "name" : "board",
            "type" : "window",

            "x" : 0,
            "y" : 0,

            "width" : SUPPORT_BG_WIDTH,
            "height" : SUPPORT_BG_HEIGHT,
            
            "children" :
            (
                ## Support UI BG
                { "name" : "PetUIBG", "type" : "expanded_image", "style" : ("attach",), "x" : 0, "y" : 0, "image" : PATCH_SUPPORT+"support_bg22.tga" },

                ##Slot Support
                {
                    "name" : "CombSlot",
                    "type" : "slot",
            
                    "x" : 0,
                    "y" : 0,
            
                    "width" : 300,
                    "height" : 300,
                    
                    "slot" :
                    (
                        {"index":1, "x":188, "y":120, "width":32, "height":64},
                        {"index":2, "x":188, "y":84, "width":32, "height":32},
                        {"index":3, "x":140, "y":88, "width":32, "height":96},
                        
                        {"index":4, "x":120, "y":228, "width":32, "height":32},
                        {"index":5, "x":120+43, "y":228, "width":32, "height":32},
                        {"index":6, "x":120+(44*2), "y":228, "width":32, "height":32},
                    ),
                },
                ## Support Information Title
                { 
                    "name" : "TitleWindow", "type" : "window", "x" : 10, "y" : 10, "width" : SUPPORT_BG_WIDTH-10-15, "height" : 15, "style" : ("attach",),
                    "children" :
                    (
                        {"name":"TitleName", "type":"text", "x":5, "y":-1, "text":"Auto Buff", "all_align" : "center"},
                    ),    
                },
                
                {"name":"WeaponTitle", "type":"text", "x":5, "y":55, "text":"Atuendos (visual)", "horizontal_align" : "center"},
                {"name":"SkillTitle", "type":"text", "x":22, "y":228-25, "text":"Habilidades", "horizontal_align" : "center"},
                ## Close Button
                { 
                    "name" : "CloseButton", 
                    "type" : "button", 
                    "x" : SUPPORT_BG_WIDTH -8-15-2, 
                    "y" : 10, 
                    "tooltip_text" : uiScriptLocale.CLOSE, 
                    "default_image" : "d:/ymir work/ui/public/close_button_01.sub",    
                    "over_image" : "d:/ymir work/ui/public/close_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/close_button_03.sub",
                },
            ),            
        },
    ),
}
