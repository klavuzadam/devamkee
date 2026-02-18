# coding: latin_1

import uiScriptLocale
import item

LOCALE_PATH = uiScriptLocale.WINDOWS_PATH
ROOT_PATH = "d:/ymir work/ui/public/"
RUTA_IMGS = "tabla_bonus/"
WIDTH = 286
HEIGHT = 365

window = {
    "name" : "TableBonusWindows",

    "x" : 400*3,
    "y" : 300*2,

    "style" : ("movable", "float", "animate",),

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
                        { "name":"TitleName", "type":"text", "x":WIDTH/2, "y":3, "text":"Tabla de Bonus", "text_horizontal_align":"center", },
                    ),
                },
                {
                    "name": "ImgBase",
                    "type": "image",
                    "x": 7,
                    "y": 31,
                    "image": RUTA_IMGS+"board.tga",
                    "children":
                    (

                        {
                            "name" : "ScrollBar",
                            "type" : "scrollbar_new",

                            "x" : 256,
                            "y" : 25,

                            "size" : 293,
                        },

                        {
                            "name" : "bar_info_1",
                            "type" : "image",
                            "x": 3,
                            "y": 3,
                            "image": RUTA_IMGS+"barra.tga",

                            "children":
                            (
                                {    
                                    "name" : "filtro_txt",
                                    "type" : "text",
                                    "x" : 10,
                                    "y" : 3,
                                    "text": "Filtro: ",
                                },

                                {
                                    "name" : "filtro_1",
                                    "type" : "radio_button",

                                    "x" : 72,
                                    "y" : 3,

                                    "default_image" : RUTA_IMGS + "off_1.tga",
                                    "over_image" : RUTA_IMGS + "off_2.tga",
                                    "down_image" : RUTA_IMGS + "on_1.tga",

                                    "children":
                                    (
                                        {    
                                            "name" : "filtro_1_txt",
                                            "type" : "text",
                                            "x" : 18,
                                            "y" : 0,
                                            "text": "Todo",
                                        },
                                    ),
                                },

                                {
                                    "name" : "filtro_2",
                                    "type" : "radio_button",

                                    "x" : 65+52,
                                    "y" : 3,

                                    "default_image" : RUTA_IMGS + "off_1.tga",
                                    "over_image" : RUTA_IMGS + "off_2.tga",
                                    "down_image" : RUTA_IMGS + "on_1.tga",

                                    "children":
                                    (
                                        {    
                                            "name" : "filtro_2_txt",
                                            "type" : "text",
                                            "x" : 18,
                                            "y" : 0,
                                            "text": "Bonus pvm",
                                        },
                                    ),
                                },

                                {
                                    "name" : "filtro_3",
                                    "type" : "radio_button",

                                    "x" : 65+48+76,
                                    "y" : 3,

                                    "default_image" : RUTA_IMGS + "off_1.tga",
                                    "over_image" : RUTA_IMGS + "off_2.tga",
                                    "down_image" : RUTA_IMGS + "on_1.tga",

                                    "children":
                                    (
                                        {    
                                            "name" : "filtro_3_txt",
                                            "type" : "text",
                                            "x" : 18,
                                            "y" : 0,
                                            "text": "Bonus pvp",
                                        },
                                    ),
                                },


                            ),
                        },
                    ),
                },
            ),
        },
    ),
}