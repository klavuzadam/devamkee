# coding: latin_1

import uiScriptLocale

DX = 3
DY = 1

window = {
    "name" : "CalendarWindow",

    "x" : (SCREEN_WIDTH / 2) - (657 / 2),
    "y" : (SCREEN_HEIGHT / 2) - (365/ 2),

    "style" : ("movable", "float", "animate",),

    "width" : 657,
    "height" : 365,

    "children" :
    (
        {
            "name" : "board",
            "type" : "board",
            "style" : ("attach",),

            "x" : 0,
            "y" : 0,

            "width" : 657,
            "height" : 365,

            "children" :
            (
                {
                    "name" : "TitleBar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 8,
                    "y" : 7,

                    "width" : 657 - 15,
                    "color" : "yellow",
                    
                    "children" :
                    (
                        {
                            "name" : "TitleName",
                            "type" : "text",
                            
                            "x" : 657 / 2,
                            "y" : 3,

                            "text_horizontal_align" : "center",

                            "text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,
                        },
                    ),
                },

                {
                    "name": "Background",
                    "type": "image",
                    
                    "x": 10,
                    "y": 30,
                    
                    "image": "d:/ymir work/ui/calendar/background.tga",

                    "children" : 
                    (
                        {
                            "name": "Separator_01",
                            "type": "image",

                            "x": 10 + 68 + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },

                        {
                            "name": "Separator_02",
                            "type": "image",

                            "x": 10 + 68 + 77 + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },

                        {
                            "name": "Separator_03",
                            "type": "image",

                            "x": 10 + 68 + (77 * 2) + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },

                        {
                            "name": "Separator_04",
                            "type": "image",

                            "x": 10 + 68 + (77 * 3) + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },

                        {
                            "name": "Separator_05",
                            "type": "image",

                            "x": 10 + 68 + (77 * 4) + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },

                        {
                            "name": "Separator_06",
                            "type": "image",

                            "x": 10 + 68 + (77 * 5) + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },

                        {
                            "name": "Separator_07",
                            "type": "image",

                            "x": 10 + 68 + (77 * 6) + DX,
                            "y": 10 + DY,

                            "image": "d:/ymir work/ui/calendar/separator.tga",
                        },
                    ),
                },
            ),
        },
    ),
}
