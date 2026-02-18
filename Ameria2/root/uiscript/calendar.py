# coding: latin_1

import uiScriptLocale

WINDOW_WIDTH    = 481
WINDOW_HEIGHT    = 344    #455

PATCH_IMG = "rework_calendario/"

window = {
    "name" : "Calendar",
    "style" : ("movable", "float", "animate",),
    
    #"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2,
    #"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
    
    "x" : SCREEN_WIDTH /2 - (WINDOW_WIDTH/2) ,
    "y" : SCREEN_HEIGHT /2 - (WINDOW_HEIGHT/2),
    
    "width" : WINDOW_WIDTH,
    "height" : WINDOW_HEIGHT,
    "children" :
    (
        {
            "name" : "board",
            "type" : "board",
            "style" : ("attach",),

            "x" : 0,
            "y" : 0,

            "width" : WINDOW_WIDTH,
            "height" : WINDOW_HEIGHT,
        
            "children" :
            (
                ## Title
                {
                    "name" : "TitleBar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 6,
                    "y" : 6,

                    "width" : WINDOW_WIDTH-13,
                    "color" : "yellow",

                    "children" :
                    (
                        { "name":"TitleName", "type":"text", "x":WINDOW_WIDTH/2-30, "y":3, "text":"Calendario Semanal de Eventos", "text_horizontal_align":"center" },

                    ),
                },

                {
                    "name": "base_transparente",
                    "type": "image",
                    "x" : 11,
                    "y" : 34,
                    "image" : PATCH_IMG+"base_transparente.tga",

                    "children":
                    (
                        {
                            "name": "titlebar_menu",
                            "type": "image",
                            "x" : 4,
                            "y" : 3,
                            "image" : PATCH_IMG+"barra_informacion.tga",

                            "children":
                            (
                                { "name":"Dia", "type":"text", "x":30, "y":3, "text":"DIA"},
                                { "name":"TEvento", "type":"text", "x":30+100, "y":3, "text":"TIPO DE EVENTO"},
                                { "name":"1Ronda", "type":"text", "x":30+255, "y":3, "text":"1 Ronda"},
                                { "name":"2Ronda", "type":"text", "x":30+334, "y":3, "text":"2 Ronda"},


                            ),
                        },
                    ),
                },
            ),
        },
    ),
}