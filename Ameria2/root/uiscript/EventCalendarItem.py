# coding: latin_1

import uiScriptLocale

window = {
    "name" : "EventCalendarItem",

    "x" : 13,
    "y" : 13,

    "width" : 70,
    "height" : 70,

    "children" :
    (
        {
            "name": "Background",
            "type": "image",
            
            "x": 0,
            "y": 0,
            
            "image": "d:/ymir work/ui/calendar/without_event.tga",

            "children" : 
            (
                {
                    "name": "IndexText",
                    "type": "text",

                    "x": 15,
                    "y": 8,

                    "text_horizontal_align": "left",

                    "text": "99",
                },

                {
                    "name": "EventImage",
                    "type": "image",

                    "x": 2,
                    "y": 2,

                    "image": "d:/ymir work/ui/calendar/events/exp.tga",
                },
            ),
        },
    ),
}
