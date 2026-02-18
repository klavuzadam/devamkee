# coding: latin_1

import grp
import app
import localeInfo

BOARD_WIDTH = 830
BOARD_HEIGHT = 620
ITEMSHOP_PATH = "locale/es/ui/itemshop/"

window = {
    "name" : "ItemShopWindow",
    "style" : ("movable", "float", "animate",),
    
    "x" : 0,
    "y" : 0,
    
    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGHT,
    
    "children" :
    (
        {
            "name" : "board",
            "type" : "board_with_titlebar",
            "style" : ("attach",),
            
            "x" : 0,
            "y" : 0,
            
            "width" : BOARD_WIDTH,
            "height" : BOARD_HEIGHT,
            
            "title" : localeInfo.ITEMSHOP_TITLE_NAME,
            "children" :
            (
                {
                    "name" : "thin_user",
                    "type" : "thinboard",
                    
                    "x" : 12,
                    "y" : 75,
                    
                    "width" : BOARD_WIDTH - 24,
                    "height" : 27*2,
                },
                {
                    "name" : "HomeTab",
                    "type" : "radio_button",
                    
                    "x" : 18,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_HOME_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "IShopTab",
                    "type" : "radio_button",
                    
                    "x" : 18+100+8,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_ISHOP_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "DrsShopTab",
                    "type" : "radio_button",
                    
                    "x" : 18+(100+8)*2,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_DRSSHOP_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "3rdShopTab",
                    "type" : "radio_button",
                    
                    "x" : 18+(100+8)*3,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_3RDSHOP_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "WheelTab",
                    "type" : "radio_button",
                    
                    "x" : 18+(100+8)*4,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_WHEEL_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "LogsTab",
                    "type" : "radio_button",
                    
                    "x" : 18+(100+8)*5,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_LOGS_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "GmTab",
                    "type" : "radio_button",
                    
                    "x" : 18+(100+8)*6,
                    "y" : 40,
                    
                    "text" : localeInfo.ITEMSHOP_ADMIN_PANEL_NAME,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "CoinsSlot",
                    "type" : "slotbar",
                    
                    "x" : 25+17+8,
                    "y" : 65+23,
                    
                    "width" : 128,
                    "height" : 27,
                    
                    "children" :
                    (
                        {
                            "name" : "CoinsValue",
                            "type" : "text",
                            "horizontal_align" : "right",
                            "text_horizontal_align" : "right",

                            "x" : 4,
                            "y" : 6,

                            "text" : "123.456.789 MD's",
                        },
                        {
                            "name" : "CoinsIcon",
                            "type" : "image",
                            "horizontal_align" : "left",
                            
                            "x" : -(17+8),
                            "y" : (27-17)/2,
                            
                            "image" : ITEMSHOP_PATH + "coin_icon.tga",
                        },
                    ),
                },
                {
                    "name" : "DrsSlot",
                    "type" : "slotbar",
                    
                    "x" : 25+17+2+128+68,
                    "y" : 65+23,
                    
                    "width" : 128,
                    "height" : 27,
                    
                    "children" :
                    (
                        {
                            "name" : "DrsValue",
                            "type" : "text",
                            "horizontal_align" : "right",
                            "text_horizontal_align" : "right",

                            "x" : 4,
                            "y" : 6,

                            "text" : "123.456.789 DR's",
                        },
                        {
                            "name" : "DrsIcon",
                            "type" : "image",
                            "horizontal_align" : "left",
                            
                            "x" : -(17+8),
                            "y" : (27-17)/2,
                            
                            "image" : ITEMSHOP_PATH + "dr_icon.tga",
                        },
                    ),
                },
                {
                    "name" : "3rd_Slot",
                    "type" : "slotbar",
                    
                    "x" : 25+17+2+(128+68)*2,
                    "y" : 65+23,
                    
                    "width" : 128,
                    "height" : 27,
                    
                    "children" :
                    (
                        {
                            "name" : "3rd_Value",
                            "type" : "text",
                            "horizontal_align" : "right",
                            "text_horizontal_align" : "right",

                            "x" : 4,
                            "y" : 6,

                            "text" : "123.456.789 MD's",
                        },
                        {
                            "name" : "3rd_Icon",
                            "type" : "image",
                            "horizontal_align" : "left",
                            
                            "x" : -(17+8),
                            "y" : (27-17)/2,
                            
                            "image" : ITEMSHOP_PATH + "coin_icon.tga",
                        },
                    ),
                },
                {
                    "name" : "UpdateCoinsButton",
                    "type" : "button",
                    
                    "x" : (25+17+((128+68)*3)) - 37,
                    "y" : 65+22+7,
                    
                    "tooltip_text" : localeInfo.ITEMSHOP_UPDATE_COINS_TOOLTIP_BUTTON,
                    
                    "default_image" : ITEMSHOP_PATH + "update_1.tga",
                    "over_image" : ITEMSHOP_PATH + "update_2.tga",
                    "down_image" : ITEMSHOP_PATH + "update_3.tga",
                },
                {
                    "name" : "SearchSlot",
                    "type" : "slotbar",
                    
                    "x" : 25+17+((128+68)*3),
                    "y" : 65+22,
                    
                    "width" : 141+20,
                    "height" : 27,
                    
                    "children" :
                    (
                        {
                            "name" : "SearchValue",
                            "type" : "editline",
                            
                            "x" : 5,
                            "y" : 7,
                            
                            "width" : 141,
                            "height" : 27,
                            "input_limit" : 20,
                        
                            "text" : localeInfo.ITEMSHOP_SEARCH_TEXT,
                            "enable_codepage" : 0,
                        },
                        {
                            "name" : "SearchButton",
                            "type" : "button",
                            "horizontal_align" : "right",
                            
                            "x" : 29,
                            "y" : 0,
                            
                            "tooltip_text" : localeInfo.ITEMSHOP_SEARCH_TOOLTIP_BUTTON,
                            
                            "default_image" : ITEMSHOP_PATH + "search_1.tga",
                            "over_image" : ITEMSHOP_PATH + "search_2.tga",
                            "down_image" : ITEMSHOP_PATH + "search_3.tga",
                        },
                    ),
                },
                { 
                    "name" : "separatorUPanel",
                    "type" : "h_separator",

                    "x" : 12,
                    "y" : 65+60,
                    "width" : BOARD_WIDTH - 24,
                },
                {
                    "name" : "HomeWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "bgBanner",
                            "type" : "resizable_text_value",
                            
                            "x" : 5,
                            "y" : 8,
                            
                            "width" : 570,
                            "height" : 289,
                            
                            "line_top" : 1,
                            "line_left" : 1,
                            "line_bottom" : 1,
                            "line_right" : 1,
                            
                            "children" :
                            (
                                {
                                    "name" : "Banner1",
                                    "type" : "image",
                                    
                                    "x" : 1,
                                    "y" : 1,

                                    "image" : ITEMSHOP_PATH + "banner_0.tga",
                                },
                                {
                                    "name" : "Banner2",
                                    "type" : "image",
                                    
                                    "x" : 1,
                                    "y" : 1,
                                    
                                    "image" : ITEMSHOP_PATH + "banner_0.tga",
                                },
                                {
                                    "name" : "ChangeBanner1",
                                    "type" : "radio_button",
                                
                                    "x" : 570-(19*2),
                                    "y" : 288-20,
                                    
                                    "default_image" : ITEMSHOP_PATH + "icon_exp_empty.tga",
                                    "over_image" : ITEMSHOP_PATH + "icon_exp_full.tga",
                                    "down_image" : ITEMSHOP_PATH + "icon_exp_full.tga",
                                },
                                {
                                    "name" : "ChangeBanner2",
                                    "type" : "radio_button",
                                
                                    "x" : 570-20,
                                    "y" : 288-20,
                                    
                                    "default_image" : ITEMSHOP_PATH + "icon_exp_empty.tga",
                                    "over_image" : ITEMSHOP_PATH + "icon_exp_full.tga",
                                    "down_image" : ITEMSHOP_PATH + "icon_exp_full.tga",
                                },
                            ),
                        },
                        {
                            "name" : "thin_hotoffers",
                            "type" : "thinboard",
                            
                            "x" : 584,
                            "y" : 2,
                            
                            "width" : BOARD_WIDTH - (584+26),
                            "height" : 318,
                        },
                        {
                            "name" : "mostBoughtTitle",
                            "type" : "resizable_text_value",
                            
                            "x" : 17,
                            "y" : 342-22-15,
                            
                            "width" : 120,
                            "height" : 18,
                            
                            "text" : localeInfo.ITEMSHOP_MOST_BOUGHT_ITEMS,
                            
                            "line_top" : 1,
                            "line_left" : 1,
                            "line_right" : 1,
                        },
                        {
                            "name" : "thin_mostbought",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : (342-22-15)+18,
                            
                            "width" : BOARD_WIDTH - 24,
                            "height" : 153,
                        },
                        {
                            "name" : "hotOffersTitle",
                            "type" : "resizable_text_value",
                            
                            "x" : 77+14+10+(202+40)*2,
                            "y" : 3,
                            
                            "width" : 88,
                            "height" : 18,
                            
                            "text" : localeInfo.ITEMSHOP_LAST_DISCOUNTS,
                            
                            "line_bottom" : 1,
                            "line_right" : 1,
                        },
                        {
                            "name" : "HomeArrowLeft",
                            "type" : "button",
                            
                            "x" : 23,
                            "y" : 389+4,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_left_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_left_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_left_3.tga",
                        },
                        {
                            "name" : "HomeArrowRight",
                            "type" : "button",
                            "horizontal_align" : "right",
                            
                            "x" : 14+23,
                            "y" : 389+4,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_right_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_right_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_right_3.tga",
                        },
                        {
                            "name" : "HomeArrowUp",
                            "type" : "button",
                            
                            "x" : 115+87+14+10+(202+40)*2,
                            "y" : 10,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_up_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_up_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_up_3.tga",
                        },
                        {
                            "name" : "HomeArrowDown",
                            "type" : "button",
                            
                            "x" : 89+87+14+10+(202+40)*2,
                            "y" : 8+28+(117+14)*2,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_down_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_down_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_down_3.tga",
                        },
                    ),
                },
                {
                    "name" : "IShopWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_cats",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : 143,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "thin_items",
                            "type" : "thinboard",
                            
                            "x" : 143,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - (143+26),
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "ItemShopArrowUp",
                            "type" : "button",
                            
                            "x" : (87/2)+20,
                            "y" : 9,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_up_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_up_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_up_3.tga",
                        },
                        {
                            "name" : "ItemShopArrowDown",
                            "type" : "button",
                            
                            "x" : (87/2)+20,
                            "y" : 478-14-10,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_down_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_down_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_down_3.tga",
                        },
                        {
                            "name" : "ItemShopArrowLeft",
                            "type" : "button",
                            
                            "x" : (154+202+16+((202-65)/2))-(65-16)/2,
                            "y" : (BOARD_HEIGHT - 142) - 29,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_left_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_left_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_left_3.tga",
                        },
                        {
                            "name" : "ItemShopPageNumberSlot",
                            "type" : "slotbar",
                            
                            "x" : 154+202+16+((202-65)/2),
                            "y" : (BOARD_HEIGHT - 142) - 36,
                            
                            "width" : 65,
                            "height" : 27,
                            
                            "children" :
                            (
                                {
                                    "name" : "ItemShopPageNumber",
                                    "type" : "text",
                                    "text_horizontal_align" : "center",
                                    "horizontal_align" : "center",
                                    "text_vertical_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 0,
                                    
                                    "text" : "1/1",
                                },
                            ),
                        },
                        {
                            "name" : "ItemShopArrowRight",
                            "type" : "button",
                            
                            "x" : (154+202+16+((202-65)/2))+65+10,
                            "y" : (BOARD_HEIGHT - 142) - 29,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_right_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_right_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_right_3.tga",
                        },
                    ),
                },
                {
                    "name" : "DrsShopWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_cats_drs",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : 143,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "thin_items_drs",
                            "type" : "thinboard",
                            
                            "x" : 143,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - (143+26),
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "DrsShopArrowUp",
                            "type" : "button",
                            
                            "x" : (87/2)+20,
                            "y" : 9,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_up_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_up_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_up_3.tga",
                        },
                        {
                            "name" : "DrsShopArrowDown",
                            "type" : "button",
                            
                            "x" : (87/2)+20,
                            "y" : 478-14-10,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_down_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_down_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_down_3.tga",
                        },
                        {
                            "name" : "DrsShopArrowLeft",
                            "type" : "button",
                            
                            "x" : (154+202+16+((202-65)/2))-(65-16)/2,
                            "y" : (BOARD_HEIGHT - 142) - 29,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_left_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_left_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_left_3.tga",
                        },
                        {
                            "name" : "DrsShopPageNumberSlot",
                            "type" : "slotbar",
                            
                            "x" : 154+202+16+((202-65)/2),
                            "y" : (BOARD_HEIGHT - 142) - 36,
                            
                            "width" : 65,
                            "height" : 27,
                            
                            "children" :
                            (
                                {
                                    "name" : "DrsShopPageNumber",
                                    "type" : "text",
                                    "text_horizontal_align" : "center",
                                    "horizontal_align" : "center",
                                    "text_vertical_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 0,
                                    
                                    "text" : "1/1",
                                },
                            ),
                        },
                        {
                            "name" : "DrsShopArrowRight",
                            "type" : "button",
                            
                            "x" : (154+202+16+((202-65)/2))+65+10,
                            "y" : (BOARD_HEIGHT - 142) - 29,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_right_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_right_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_right_3.tga",
                        },
                    ),
                },
                {
                    "name" : "3rdShopWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_cats_3rd",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : 143,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "thin_items_3rd",
                            "type" : "thinboard",
                            
                            "x" : 143,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - (143+26),
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "3rdShopArrowUp",
                            "type" : "button",
                            
                            "x" : (87/2)+20,
                            "y" : 9,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_up_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_up_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_up_3.tga",
                        },
                        {
                            "name" : "3rdShopArrowDown",
                            "type" : "button",
                            
                            "x" : (87/2)+20,
                            "y" : 478-14-10,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_down_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_down_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_down_3.tga",
                        },
                        {
                            "name" : "3rdShopArrowLeft",
                            "type" : "button",
                            
                            "x" : (154+202+16+((202-65)/2))-(65-16)/2,
                            "y" : (BOARD_HEIGHT - 142) - 29,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_left_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_left_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_left_3.tga",
                        },
                        {
                            "name" : "3rdShopPageNumberSlot",
                            "type" : "slotbar",
                            
                            "x" : 154+202+16+((202-65)/2),
                            "y" : (BOARD_HEIGHT - 142) - 36,
                            
                            "width" : 65,
                            "height" : 27,
                            
                            "children" :
                            (
                                {
                                    "name" : "3rdShopPageNumber",
                                    "type" : "text",
                                    "text_horizontal_align" : "center",
                                    "horizontal_align" : "center",
                                    "text_vertical_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 0,
                                    
                                    "text" : "1/1",
                                },
                            ),
                        },
                        {
                            "name" : "3rdShopArrowRight",
                            "type" : "button",
                            
                            "x" : (154+202+16+((202-65)/2))+65+10,
                            "y" : (BOARD_HEIGHT - 142) - 29,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_right_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_right_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_right_3.tga",
                        },
                    ),
                },
                {
                    "name" : "WheelWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_wheel",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - 24,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "bgBestItems",
                            "type" : "resizable_text_value",
                            "horizontal_align" : "center",
                            
                            "x" : 240,
                            "y" : 12+25,
                            
                            "width" : 180,
                            "height" : 66*6,
                            
                            "line_bottom" : 1,
                            "line_left" : 1,
                            "line_right" : 1,
                        },
                        {
                            "name" : "BestWheelItemsTitle",
                            "type" : "horizontalbar",
                            "horizontal_align" : "right",
                            
                            "x" : 208+58,
                            "y" : 20,
                            "width" : 208,
                            "without_background" : 1,
                            
                            "children" :
                            (
                                {
                                    "name" : "BestWheelItemsTitleValue",
                                    "type" : "text",
                                    "text_horizontal_align" : "center",
                                    "horizontal_align" : "center",
                                    "text_vertical_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.ITEMSHOP_BEST_WHEEL_PRIZES,
                                },
                            ),
                        },
                        {
                            "name" : "BgWheel",
                            "type" : "image",
                            
                            "x" : 48,
                            "y" : 25,
                            
                            "image" : ITEMSHOP_PATH + "wheel.tga",
                        },
                        {
                            "name" : "RotationWheel",
                            "type" : "expanded_image",
                            
                            "x" : 48,
                            "y" : 25,
                            
                            "image" : ITEMSHOP_PATH + "spin_arrow.tga",
                            
                            "children" :
                            (
                                {
                                    "name" : "SpinButton",
                                    "type" : "button",
                                    "horizontal_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : -1,
                                    "y" : -4,
                                    
                                    "default_image" : ITEMSHOP_PATH + "spin_btn_1.tga",
                                    "over_image" : ITEMSHOP_PATH + "spin_btn_2.tga",
                                    "down_image" : ITEMSHOP_PATH + "spin_btn_3.tga",
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "WheelCostTitle1",
                                            "type" : "text",
                                            "all_align" : 1,
                                            
                                            "x" : 0,
                                            "y" : -8,
                                            
                                            "text" : localeInfo.ITEMSHOP_WHEEL_PRICE1,
                                            "outline" : 1,
                                            
                                            "fontname" : "Tahoma:14",
                                        },
                                        {
                                            "name" : "WheelCostTitle2",
                                            "type" : "text",
                                            "all_align" : 1,
                                            
                                            "x" : 0,
                                            "y" : 8,
                                            
                                            "text" : localeInfo.ITEMSHOP_WHEEL_PRICE2,
                                            "outline" : 1,
                                            
                                            "fontname" : "Tahoma:14",
                                        },
                                    ),
                                },
                            ),
                        },
                    ),
                },
                {
                    "name" : "LogsWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_logs",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - 24,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "LogsPageNumberSlot",
                            "type" : "slotbar",
                            "horizontal_align" : "center",
                            
                            "x" : 0,
                            "y" : (BOARD_HEIGHT - 142) - 42,
                            
                            "width" : 65,
                            "height" : 27,
                            
                            "children" :
                            (
                                {
                                    "name" : "LogsPageNumber",
                                    "type" : "text",
                                    "text_horizontal_align" : "center",
                                    "horizontal_align" : "center",
                                    "text_vertical_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 0,
                                    
                                    "text" : "1/1",
                                },
                            ),
                        },
                        {
                            "name" : "LogsArrowLeft",
                            "type" : "button",
                            "horizontal_align" : "center",
                            
                            "x" : -(53-7),
                            "y" : (BOARD_HEIGHT - 142) - 36,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_left_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_left_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_left_3.tga",
                        },
                        {
                            "name" : "LogsArrowRight",
                            "type" : "button",
                            "horizontal_align" : "center",
                            
                            "x" : 53-6,
                            "y" : (BOARD_HEIGHT - 142) - 36,
                            
                            "default_image" : ITEMSHOP_PATH + "arrow_right_1.tga",
                            "over_image" : ITEMSHOP_PATH + "arrow_right_2.tga",
                            "down_image" : ITEMSHOP_PATH + "arrow_right_3.tga",
                        },
                    ),
                },
                {
                    "name" : "GmWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "AdminTitle",
                            "type" : "horizontalbar",
                            "horizontal_align" : "center",
                            
                            "x" : 0,
                            "y" : 16,
                            "width" : 570,
                            
                            "children" :
                            (
                                {
                                    "name" : "AdminTitleValue",
                                    "type" : "text",
                                    "text_horizontal_align" : "center",
                                    "horizontal_align" : "center",
                                    "text_vertical_align" : "center",
                                    "vertical_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.ITEMSHOP_ADMIN_PANEL_TITLE,
                                },
                            ),
                        },
                        {
                            "name" : "AdminAddItemTime",
                            "type" : "resizable_button_with_image",
                            "horizontal_align" : "center",
                            
                            "x" : -88,
                            "y" : 16+50,
                            
                            "text_x" : 12+3,
                            "text_y" : 0,
                            
                            "width" : 150,
                            "height" : 55,
                            
                            "tooltip_text" : localeInfo.ITEMSHOP_ADMIN_PANEL_ADD_DISCOUNTS_TOOLTIP_BUTTON,
                            "text" : localeInfo.ITEMSHOP_ADMIN_PANEL_ADD_DISCOUNTS_TEXT_BUTTON,
                            "image" : ITEMSHOP_PATH + "admin_discount.tga",
                        },
                        {
                            "name" : "AdminAddNewItem",
                            "type" : "resizable_button_with_image",
                            "horizontal_align" : "center",
                            
                            "x" : 88,
                            "y" : 16+50,
                            
                            "text_x" : 12+4,
                            "text_y" : 0,
                            
                            "width" : 150,
                            "height" : 55,
                            
                            "tooltip_text" : localeInfo.ITEMSHOP_ADMIN_PANEL_ADD_ITEMS_TOOLTIP_BUTTON,
                            "text" : localeInfo.ITEMSHOP_ADMIN_PANEL_ADD_ITEMS_TEXT_BUTTON,
                            "image" : ITEMSHOP_PATH + "admin_plus.tga",
                        },
                    ),
                },
                {
                    "name" : "AddItemTimeWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_admin_add_time",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - 24,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "CategorysListBoxBg",
                            "type" : "bar",
                            
                            "x" : 15,
                            "y" : 10+45,
                            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                            
                            "width" : 145+20,
                            "height" : 180,
                            
                            "children" :
                            (
                                {
                                    "name" : "CategorysListBox",
                                    "type" : "listboxex",
                            
                                    "x" : 4,
                                    "y" : 2,
                                    
                                    "width" : 145,
                                    "height" : 178,
                                    
                                    "viewcount" : 9,
                                },
                                {
                                    "name" : "CategorysListBoxScroll",
                                    "type" : "scrollbar",
                                    "horizontal_align" : "right",
                                    
                                    "x" : 21,
                                    "y" : 4,
                                    
                                    "size" : 178-4,
                                },
                            ),
                        },
                        {
                            "name" : "ItemsListBoxBg",
                            "type" : "bar",
                                    
                            "x" : 15,
                            "y" : 10+220+13,
                            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                        
                            "width" : 165,
                            "height" : 200,
                        
                            "children" :
                            (
                                {
                                    "name" : "ItemsListBox",
                                    "type" : "listboxex",
                            
                                    "x" : 4,
                                    "y" : 2,
                                    
                                    "width" : 145,
                                    "height" : 198,
                                    
                                    "viewcount" : 10,
                                },
                                {
                                    "name" : "ItemsListBoxScroll",
                                    "type" : "scrollbar",
                                    "horizontal_align" : "right",
                                    
                                    "x" : 21,
                                    "y" : 4,
                                            
                                    "size" : 198-4,
                                },
                            ),
                        },
                    ),
                },
                {
                    "name" : "AddItemWindow",
                    "type" : "window",
                    
                    "x" : 14,
                    "y" : 132,
                    
                    "width" : BOARD_WIDTH - 14*2,
                    "height" : BOARD_HEIGHT - 142,
                    
                    "children" :
                    (
                        {
                            "name" : "thin_admin_add_item",
                            "type" : "thinboard",
                            
                            "x" : -2,
                            "y" : 0,
                            
                            "width" : BOARD_WIDTH - 24,
                            "height" : BOARD_HEIGHT - 143,
                        },
                        {
                            "name" : "AddItemNewBoard",
                            "type" : "thinboard",
                            "horizontal_align" : "center",
                            "vertical_align" : "center",
                            
                            "x" : 0,
                            "y" : 10,
                            
                            "width" : 570,
                            "height" : 380,
                            
                            "children" :
                            (
                                {
                                    "name" : "AddItemVnumSlot",
                                    "type" : "resizable_text_value",
                                    
                                    "x" : 70,
                                    "y" : 2+28,
                                    
                                    "width" : 80,
                                    "height" : 20,
                                    "all_lines" : 1,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemVnumText",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                            
                                            "x" : -35,
                                            "y" : 0,
                                            "outline" : 1,
                                            
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VNUM_TITLE,
                                        },
                                        {
                                            "name" : "AddItemVnum",
                                            "type" : "editline",

                                            "x" : 4,
                                            "y" : 3,
                                            
                                            "width" : 80-3,
                                            "height" : 20,
                                            
                                            "text" : "0",
                                            "outline" : 1,
                                            "input_limit" : 10,
                                            "only_number" : 1,
                                        },
                                    ),
                                },
                                {
                                    "name" : "AddItemCatSlot",
                                    "type" : "resizable_text_value",
                                    
                                    "x" : 70,
                                    "y" : 2+28*2,
                                    
                                    "width" : 80,
                                    "height" : 20,
                                    "all_lines" : 1,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemCatText",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                            
                                            "x" : -53,
                                            "y" : 0,
                                            "outline" : 1,
                                            
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_CATEGORY_TITLE,
                                        },
                                    ),
                                },
                                {
                                    "name" : "AddItemPriceSlot",
                                    "type" : "resizable_text_value",
                                    
                                    "x" : 70,
                                    "y" : 2+28*3,
                                    
                                    "width" : 80,
                                    "height" : 20,
                                    "all_lines" : 1,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemPriceText",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                            
                                            "x" : -36,
                                            "y" : 0,
                                            "outline" : 1,
                                            
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_PRICE_TITLE,
                                        },
                                        {
                                            "name" : "AddItemPrice",
                                            "type" : "editline",

                                            "x" : 4,
                                            "y" : 3,
                                            
                                            "width" : 80-3,
                                            "height" : 20,
                                            
                                            "text" : "0",
                                            "outline" : 1,
                                            "input_limit" : 10,
                                            "only_number" : 1,
                                        },
                                    ),
                                },
                                {
                                    "name" : "AddItemIcon",
                                    "type" : "expanded_image",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 30,
                                    
                                    "image" : "icon/item/00010.tga",
                                },
                                {
                                    "name" : "Attribute0Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus1",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",

                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,

                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 1:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype0Slot",
                                            "type" : "resizable_text_value",

                                            "x" : 48,
                                            "y" : 0,

                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype0",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,

                                                    "width" : 80-3,
                                                    "height" : 20,

                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue1",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",

                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,

                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 1:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue0Slot",
                                            "type" : "resizable_text_value",

                                            "x" : 1+48+45+48,
                                            "y" : 0,

                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue0",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,

                                                    "width" : 80-3,
                                                    "height" : 20,

                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Attribute1Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130+23,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus2",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 2:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype1Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype1",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue2",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 2:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue1Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 1+48+45+48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue1",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Attribute2Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130+23*2,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus3",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                    
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 3:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype2Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype2",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue3",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 3:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue2Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 1+48+45+48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue2",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Attribute3Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130+23*3,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus4",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 4:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype3Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype3",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue4",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 4:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue3Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 1+48+45+48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue3",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Attribute4Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130+23*4,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus5",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 5:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype4Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype4",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                            
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue5",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 5:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue4Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 1+48+45+48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue4",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Attribute5Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130+25*5,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus6",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 6:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype5Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype5",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue6",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 6:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue5Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 1+48+45+48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue5",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Attribute6Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -75,
                                    "y" : 16+130+25*6,
                                    
                                    "width" : 1+48+45+44*2,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemBonus7",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_BONUS_TITLE + " 7:",
                                        },
                                        {
                                            "name" : "AddItemAttrtype6Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrtype6",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                        {
                                            "name" : "AddItemValue7",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1+48+49,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_VALUE_TITLE + " 7:",
                                        },
                                        {
                                            "name" : "AddItemAttrvalue6Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 1+48+45+48,
                                            "y" : 0,
                                        
                                            "width" : 40,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemAttrvalue6",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Socket0Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 120,
                                    "y" : 115+25*2,
                                    
                                    "width" : 1+48+60,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemSocketTxt0",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_STONE_TITLE + " 1:",
                                        },
                                        {
                                            "name" : "AddItemSocket0Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 60,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemSocket0",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Socket1Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 120,
                                    "y" : 115+25*3,
                                    
                                    "width" : 1+48+60,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemSocket1",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_STONE_TITLE + " 2:",
                                        },
                                        {
                                            "name" : "AddItemSocket1Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 60,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemSocket1",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "Socket2Bar",
                                    "type" : "window",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 120,
                                    "y" : 115+25*4,
                                    
                                    "width" : 1+48+60,
                                    "height" : 21,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "AddItemSocket2",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                        
                                            "x" : 1,
                                            "y" : 0,
                                            "outline" : 1,
                                        
                                            "text" : localeInfo.ITEMSHOP_ADD_ITEM_STONE_TITLE + " 3:",
                                        },
                                        {
                                            "name" : "AddItemSocket2Slot",
                                            "type" : "resizable_text_value",
                                        
                                            "x" : 48,
                                            "y" : 0,
                                        
                                            "width" : 60,
                                            "height" : 20,
                                            "all_lines" : 1,
                                            
                                            "children" :
                                            (
                                                {
                                                    "name" : "AddItemSocket2",
                                                    "type" : "editline",

                                                    "x" : 4,
                                                    "y" : 3,
                                                
                                                    "width" : 80-3,
                                                    "height" : 20,
                                                
                                                    "text" : "0",
                                                    "outline" : 1,
                                                    "input_limit" : 10,
                                                    "only_number" : 1,
                                                },
                                            ),
                                        },
                                    ),
                                },
                                {
                                    "name" : "AddItemReset",
                                    "type" : "button",
                                    "horizontal_align" : "center",
                                    
                                    "x" : -62,
                                    "y" : 380 - 35,
                                    
                                    "text" : localeInfo.ITEMSHOP_ADD_ITEM_RESET_TEXT_BUTTON,
                                    
                                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                                },
                                {
                                    "name" : "AddItemApply",
                                    "type" : "button",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 62,
                                    "y" : 380 - 35,
                                    
                                    "text" : localeInfo.ITEMSHOP_ADD_ITEM_ACCEPT_TEXT_BUTTON,
                                    
                                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                                },
                            ),
                        },                        
                    ),
                },
                {
                    "name" : "ReturnButton",
                    "type" : "button",
                    
                    "x" : 29,
                    "y" : 10+8+132,
                    
                    "default_image" : ITEMSHOP_PATH + "arrow_left_1.tga",
                    "over_image" : ITEMSHOP_PATH + "arrow_left_2.tga",
                    "down_image" : ITEMSHOP_PATH + "arrow_left_3.tga",
                    
                    "children" :
                    (
                        {
                            "name" : "ReturnButtonText",
                            "type" : "text",
                            
                            "x" : 27,
                            "y" : 1,
                            
                            "text" : localeInfo.ITEMSHOP_ADMIN_PANEL_RETURN_TOOLTIP_BUTTON,
                        },
                    ),
                },
            ),
        },
    ),
}