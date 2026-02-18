# coding: latin_1

import uiScriptLocale

window = {
    "name" : "EventCalendarItem",
    "style" : ("movable", "float", "animate",),

    "x" : SCREEN_WIDTH - 250,
    "y" : 10,

    "width" : 70,
    "height" : 70,

    "children" :
    (
        {
            "name": "Background",
            "type": "button",

            "style" : ("attach",),
            
            "x": 0,
            "y": 0,

            #"image": "d:/ymir work/ui/calendar/events/exp.tga",

            "default_image" : "d:/ymir work/ui/calendar/events/exp.tga",
            "over_image" : "d:/ymir work/ui/calendar/events/exp.tga",
            "down_image" : "d:/ymir work/ui/calendar/events/exp.tga",

            "children" : 
            (
                {
                    "name": "TextLine",
                    "type": "text",

                    "x": 35,
                    "y": 70,

                    "text_horizontal_align": "center",
                    "outline" : 1,
                    
                    "text": "99",
                },

                {
                    "name": "LeftTime",
                    "type": "text",

                    "x": 35,
                    "y": 70 + 17,

                    "text_horizontal_align": "center",
                    "outline" : 1,
                    
                    "text": "99",
                },
            ),
        },
    ),
}
