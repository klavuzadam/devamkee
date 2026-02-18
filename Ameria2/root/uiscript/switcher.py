# coding: latin_1

import uiScriptLocale
import item

LOCALE_PATH = uiScriptLocale.WINDOWS_PATH
ROOT_PATH = "d:/ymir work/ui/public/"
DOPADOR_PATCH = "Dopador_manual/"
WIDTH = 261
HEIGHT = 376

window = {
    "name" : "DopadorManualWindows",

    "x" : 300,
    "y" : 100,

    "style" : ("movable", "float"),

    "width" : WIDTH,
    "height" : HEIGHT,

    "children" :
    (
                                
        {
            "name" : "board",
            "type" : "board",
            "style" : ("attach",),

            "x" : 0,
            "y" : 0,

            "width" : WIDTH,
            "height" : HEIGHT,
            
            "children" :
            (
                ## Title
                {
                    "name" : "TitleBar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 6,
                    "y" : 6,

                    "width" : WIDTH-12,
                    "color" : "yellow",

                    "children" :
                    (
                        { "name":"TitleName", "type":"text", "x":120, "y":3, "text":"Dopador Manual v1", "text_horizontal_align":"center", },
                    ),
                },

                ## Equipment Slot
                {
                    "name" : "Base",
                    "type" : "image",
                    "style" : ("not_pick",),

                    "x" : 13,
                    "y" : 40,
                    
                    "image" : DOPADOR_PATCH+"slot_item.tga",                    

                    "children" :
                    (

                        {
                            "name" : "switchslot",
                            "type" : "slot",

                            "x" : 3,
                            "y" : 3,

                            "width" : 234,
                            "height" : 125,

                            "slot" : (
                                        {"index":0, "x":97, "y":11, "width":32, "height":96},
                                    ),
                        },
                    ),
                },
                {
                    "name" : "slot_bonus_0",
                    "type" : "image",
                    "style" : ("not_pick",),
                    
                    "x" : 13,
                    "y" : 170,

                    "image" : DOPADOR_PATCH+"slot_bonus.tga",

                    "children" :
                    (
                        {
                            "name" : "bonus_0",
                            "type" : "text",

                            "x" : 0,
                            "y" : -1,
                            "r":0.6911,
                            "g":0.8754,
                            "b":0.7068,
                            "a":1.0,

                            "text" : "None",

                            "all_align" : "center",
                        },
                    ),
                },


                {
                    "name" : "slot_bonus_1",
                    "type" : "image",
                    "style" : ("not_pick",),
                    
                    "x" : 13,
                    "y" : 170+27,

                    "image" : DOPADOR_PATCH+"slot_bonus.tga",

                    "children" :
                    (
                        {
                            "name" : "bonus_1",
                            "type" : "text",

                            "x" : 0,
                            "y" : -1,
                            "r":0.6911,
                            "g":0.8754,
                            "b":0.7068,
                            "a":1.0,

                            "text" : "None",

                            "all_align" : "center",
                        },
                    ),
                },

                {
                    "name" : "slot_bonus_2",
                    "type" : "image",
                    "style" : ("not_pick",),
                    
                    "x" : 13,
                    "y" : 170+(27*2),

                    "image" : DOPADOR_PATCH+"slot_bonus.tga",

                    "children" :
                    (
                        {
                            "name" : "bonus_2",
                            "type" : "text",

                            "x" : 0,
                            "y" : -1,
                            "r":0.6911,
                            "g":0.8754,
                            "b":0.7068,
                            "a":1.0,

                            "text" : "None",

                            "all_align" : "center",
                        },
                    ),
                },

                {
                    "name" : "slot_bonus_3",
                    "type" : "image",
                    "style" : ("not_pick",),
                    
                    "x" : 13,
                    "y" : 170+(27*3),

                    "image" : DOPADOR_PATCH+"slot_bonus.tga",

                    "children" :
                    (
                        {
                            "name" : "bonus_3",
                            "type" : "text",

                            "x" : 0,
                            "y" : -1,
                            "r":0.6911,
                            "g":0.8754,
                            "b":0.7068,
                            "a":1.0,

                            "text" : "None",

                            "all_align" : "center",
                        },
                    ),
                },

                {
                    "name" : "slot_bonus_4",
                    "type" : "image",
                    "style" : ("not_pick",),
                    
                    "x" : 13,
                    "y" : 170+(27*4),

                    "image" : DOPADOR_PATCH+"slot_bonus.tga",

                    "children" :
                    (
                        {
                            "name" : "bonus_4",
                            "type" : "text",

                            "x" : 0,
                            "y" : -1,
                            "r":0.6911,
                            "g":0.8754,
                            "b":0.7068,
                            "a":1.0,

                            "text" : "None",

                            "all_align" : "center",
                        },
                    ),
                },

                {
                    "name" : "cambiar_bonus",
                    "type" : "button",

                    "x" : 55,
                    "y" : 330,

                    "text" : "Cambiar Bonus",

                    "default_image" : DOPADOR_PATCH + "boton_1.tga",
                    "over_image" : DOPADOR_PATCH + "boton_2.tga",
                    "down_image" : DOPADOR_PATCH + "boton_3.tga",
                },

            ),
        },
    ),
}
