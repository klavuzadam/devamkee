# coding: latin_1

import localeInfo

BOARD_WIDTH = 320
BOARD_HEIGHT = 150

window = {
    "name" : "AchievementShopDicountCategoryBox",
    
    "x" : 0,
    "y" : 0,
    
    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGHT,
    
    "children" :
    (
        {
            "name" : "board",
            "type" : "thinboard",
            "style" : ("attach",),
            
            "x" : 0,
            "y" : 0,
            
            "width" : BOARD_WIDTH,
            "height" : BOARD_HEIGHT,
            
            "children" :
            (
                {
                    "name" : "CategoryTitle",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : 12,
                    "outline" : 1,
                    
                    "text" : localeInfo.ITEMSHOP_ADD_DISCOUNT_TITLE1,
                },
                {
                    "name" : "PercentSlot",
                    "type" : "resizable_text_value",
                    "horizontal_align" : "center",
                    
                    "x" : 18,
                    "y" : 38,
                    
                    "width" : 35,
                    "height" : 20,
                    "all_lines" : 1,
                    
                    "children" :
                    (
                        {
                            "name" : "Percent",
                            "type" : "editline",

                            "x" : 5,
                            "y" : 3,
                            
                            "width" : 35-3,
                            "height" : 20,
                            
                            "text" : "1",
                            "outline" : 1,
                            "input_limit" : 3,
                            "only_number" : 1,
                        },
                        {
                            "name" : "PercentTxt",
                            "type" : "text",
                            "vertical_align" : "center",
                            "text_vertical_align" : "center",
                            
                            "x" : -60,
                            "y" : 0,
                            "outline" : 1,
                            
                            "text" : localeInfo.ITEMSHOP_ADD_DISCOUNT_DISCOUNT_TITLE,
                        },
                        {
                            "name" : "PercentSymbol",
                            "type" : "text",
                            "vertical_align" : "center",
                            "text_vertical_align" : "center",
                            
                            "x" : 42,
                            "y" : 0,
                            "outline" : 1,
                            
                            "text" : "%",
                        },
                    ),
                },
                {
                    "name" : "HoursSlot",
                    "type" : "resizable_text_value",
                    "horizontal_align" : "center",
                    
                    "x" : -(16+36),
                    "y" : 70,
                    
                    "width" : 25,
                    "height" : 20,
                    "all_lines" : 1,
                    
                    "children" :
                    (
                        {
                            "name" : "Hours",
                            "type" : "editline",

                            "x" : 5,
                            "y" : 3,
                            
                            "width" : 35-3,
                            "height" : 20,
                            
                            "text" : "1",
                            "outline" : 1,
                            "input_limit" : 3,
                            "only_number" : 1,
                        },
                        {
                            "name" : "HoursSymbol",
                            "type" : "text",
                            "vertical_align" : "center",
                            "text_vertical_align" : "center",
                            
                            "x" : 30,
                            "y" : 0,
                            "outline" : 1,
                            
                            "text" : localeInfo.ITEMSHOP_ADD_DISCOUNT_HOUR,
                        },
                    ),
                },
                {
                    "name" : "MinutesSlot",
                    "type" : "resizable_text_value",
                    "horizontal_align" : "center",
                    
                    "x" : 30-36,
                    "y" : 70,
                    
                    "width" : 25,
                    "height" : 20,
                    "all_lines" : 1,
                    
                    "children" :
                    (
                        {
                            "name" : "Minutes",
                            "type" : "editline",

                            "x" : 5,
                            "y" : 3,
                            
                            "width" : 35-3,
                            "height" : 20,
                            
                            "text" : "1",
                            "outline" : 1,
                            "input_limit" : 3,
                            "only_number" : 1,
                        },
                        {
                            "name" : "MinutesSymbol",
                            "type" : "text",
                            "vertical_align" : "center",
                            "text_vertical_align" : "center",
                            
                            "x" : 30,
                            "y" : 0,
                            "outline" : 1,
                            
                            "text" : localeInfo.ITEMSHOP_ADD_DISCOUNT_MINUTE,
                        },
                    ),
                },
                {
                    "name" : "SecondsSlot",
                    "type" : "resizable_text_value",
                    "horizontal_align" : "center",
                    
                    "x" : (16+30*2)-36,
                    "y" : 70,
                    
                    "width" : 25,
                    "height" : 20,
                    "all_lines" : 1,
                    
                    "children" :
                    (
                        {
                            "name" : "Seconds",
                            "type" : "editline",

                            "x" : 5,
                            "y" : 3,
                            
                            "width" : 35-3,
                            "height" : 20,
                            
                            "text" : "1",
                            "outline" : 1,
                            "input_limit" : 3,
                            "only_number" : 1,
                        },
                        {
                            "name" : "SecondsSymbol",
                            "type" : "text",
                            "vertical_align" : "center",
                            "text_vertical_align" : "center",
                            
                            "x" : 30,
                            "y" : 0,
                            "outline" : 1,
                            
                            "text" : localeInfo.ITEMSHOP_ADD_DISCOUNT_SECOND,
                        },
                    ),
                },
                {
                    "name" : "ApplyButton",
                    "type" : "button",
                    "horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : BOARD_HEIGHT - 30,
                    "text" : localeInfo.ITEMSHOP_ADD_ITEM_ACCEPT_TEXT_BUTTON,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
            ),
        },
    ),
}