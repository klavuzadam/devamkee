# coding: latin_1

import uiScriptLocale
import item
import app
import localeInfo

LOCALE_PATH = "d:/ymir work/ui/privatesearch/"
GOLD_COLOR    = 0xFFFEE3AE

BOARD_WIDTH = 500 
BOARD_HEIGTH = 185
window = {
    "name" : "shout",

    "x" : 0,
    "y" : 0,

    "style" : ("movable", "float", "animate",),

    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGTH,

    "children" :
    (
        {
            "name" : "board",
            "type" : "board_with_titlebar",

            "x" : 0,
            "y" : 0,
            "width" : 500,
            "height" : 185,
                    
            "title" : "Auto Shout System",
                    
            "children" : 
            (
                {
                    "name" : "oyuncu",
                    "type" : "thinboard_circle",

                    "x" : 15,
                    "y" : 35,
                    "width" : 470,
                    "height" : 100,
                            
                    "children" :
                            
                    (
                        {
                            "name" : "bilgi",
                            "type" : "text",
                            "x" : 0,
                            "y" : 5,
                            "text" : "Type the message to send",
                            'horizontal_align': 'center',
                            'text_horizontal_align': 'center',
                        },
                                
                        {
                            "name" : "yazibar",
                            "type" : "slotbar",
                            "x" : 10,
                            "y" : 25,
                            "width" : 450,
                            "height" : 65,

                            "children" :
                            (
                                {
                                    "name" : "CommentValue",
                                    "type" : "editline",
                                    "x" : 2,
                                    "y" : 3,
                                    "width" : 440,
                                    "height" : 65,
                                    "input_limit" : 512,
                                    "text" : "",
                                },
                            ),
                        },
                                
                    ),
                },
                        
                {
                    "name" : "alttextbutton",
                    "type" : "thinboard_circle",

                    "x" : 15,
                    "y" : 135,
                    "width" : 470,
                    "height" : 40,
                            
                    "children" :
                            
                    (
                        {
                            "name" : "baslat",
                            "type" : "button",
                            "x" : 5,
                            "y" : 8,
                            "default_image" : "d:/ymir work/ui/buton/1.tga",
                            "over_image" : "d:/ymir work/ui/buton/2.tga",
                            "down_image" : "d:/ymir work/ui/buton/3.tga",
                            "text" : "Start",
                        },    
                                
                        {
                            "name" : "durdur",
                            "type" : "button",
                            "x" : 5+155,
                            "y" : 8,
                            "default_image" : "d:/ymir work/ui/buton/1.tga",
                            "over_image" : "d:/ymir work/ui/buton/2.tga",
                            "down_image" : "d:/ymir work/ui/buton/3.tga",
                            "text" : "Stop",
                        },
                                
                        {
                            "name" : "temizle",
                            "type" : "button",
                            "x" : 5+310,
                            "y" : 8,
                            "default_image" : "d:/ymir work/ui/buton/1.tga",
                            "over_image" : "d:/ymir work/ui/buton/2.tga",
                            "down_image" : "d:/ymir work/ui/buton/3.tga",
                            "text" : "Clear",
                        },                    
                    ),
                },                    
            ),
        },
    ),
}
