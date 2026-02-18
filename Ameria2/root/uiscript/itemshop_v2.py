# coding: latin_1

import uiScriptLocale
import localeInfo

BOARDWIDTH = 750
BOARDHEIGHT = 552

MAINFOLDER = "locale/es/ui/"
IMG_ITEMSHOP = "Itemshop/"

window = {
    "name" : "Itemshop",

    "x" : SCREEN_WIDTH/2-BOARDWIDTH/2,
    "y" : SCREEN_HEIGHT /2-BOARDHEIGHT/2,

    "style" : ("movable", "float", "animate",),

    "width" : BOARDWIDTH,
    "height" : BOARDHEIGHT,

    "children" :
    (
        {
            "name" : "Board",
            "type" : "board",
            "style" : ("attach",),

            "x" : 0,
            "y" : 0,

            "width" : BOARDWIDTH,
            "height" : BOARDHEIGHT,

            "children" :
            (
                {
                    "name": "Menue",
                    
                    "x" : 0,
                    "y" : 0,
                    
                    "width" : BOARDWIDTH,
                    "height" : 200,

                    "style" : ("movable", "attach", "animate",),
                    
                    "children": 
                    (
                        {
                            "name" : "base_inicio",
                            "type" : "image",
                            "x" : 13,
                            "y" : 35,
                            "image" : IMG_ITEMSHOP+"base_inicio.tga",
                        },

                            {
                            "name" : "base_categorias",
                            "type" : "image",
                            "x" : 11,
                            "y" : 35,
                            "image" : "d:/ymir work/ui/itemshop/fondo.tga",
                        },

                        {
                            "name" : "barra_inicio",
                            "type" : "image",
                            "x" : 16,
                            "y" : 38,
                            "image" : IMG_ITEMSHOP+"barra_inicio.tga",
                        },

                        ## Buttons
                        {
                            "name" : "btn_startpage", 
                            "type" : "button",
                            "text" : localeInfo.ITEMSHOP_MAIN_BUTTON,
                            "x" : 27,
                            "y" : 45,
                            "default_image" : IMG_ITEMSHOP+"inicio_1.tga",
                            "over_image" : IMG_ITEMSHOP+"inicio_2.tga",
                            "down_image" : IMG_ITEMSHOP+"inicio_3.tga",
                        },
                        {
                            "name" : "btn_itemshop", 
                            "type" : "button",
                            "text" : localeInfo.ITEMSHOP_SHOP_BUTTON,
                            "x" : 125,
                            "y" : 45,
                            "default_image" : IMG_ITEMSHOP+"btn_items_1.tga",
                            "over_image" : IMG_ITEMSHOP+"btn_items_2.tga",
                            "down_image" : IMG_ITEMSHOP+"btn_items_3.tga",
                        },
                        {
                            "name" : "btn_voteshop", 
                            "type" : "button",
                            "x" : 277,
                            "y" : 45,
                            "default_image" : IMG_ITEMSHOP+"btn_votos_1.tga",
                            "over_image" : IMG_ITEMSHOP+"btn_votos_2.tga",
                            "down_image" : IMG_ITEMSHOP+"btn_votos_3.tga",
                            "children" :
                            (
                                {
                                    "name" : "btn_voteshop_txt",
                                    "type" : "text",
                                    "text" : "Tienda de Tokens",
                                    "x" : 30,
                                    "y" : 5,
                                },
                            ),
                        },
                        {
                            "name" : "btn_achievementshop", 
                            "type" : "button",
                            "text" : "Achievements Shop",
                            "x" : 527,
                            "y" : 64,
                            "default_image" : MAINFOLDER+"itemshop/btn_other_page_norm.sub",
                            "over_image" : MAINFOLDER+"itemshop/btn_other_page_hover.sub",
                            "down_image" : MAINFOLDER+"itemshop/btn_other_page_press.sub",
                        },
                        ## Itemshop coins

                        {
                            "name" : "tx_i_coins_text",
                            "type" : "text",
                            "text" : localeInfo.ITEMSHOP_COINS,
                            "x" : 640-33,
                            "y" : 49,
                        },

                        {
                            "name" : "sb_i_coins",
                            "type" : "image",
                            "x" : 640,
                            "y" : 48,
                            "image" : IMG_ITEMSHOP+"coins.tga",
                            
                            "children" :
                            (

                                {
                                    "name" : "tx_i_coins",
                                    "type" : "text",
                                    "text" : "100.000 I-Coins",
                                    "x" : 0,
                                    "y" : 2,
                                    "color" : 0xffd6bd71,
                                    "horizontal_align" : "center",
                                    "text_horizontal_align" : "center",
                                },
                            ),
                        },

                        {
                            "name" : "sb_v_coins_text",
                            "type" : "text",
                            "text" : "Tokens:",
                            "x" : 640-43,
                            "y" : 49,
                        },

                        ## Voteshop coins
                        {
                            "name" : "sb_v_coins",
                            "type" : "image",
                            "x" : 640,
                            "y" : 48,
                            "image" : IMG_ITEMSHOP+"vcoins.tga",
                            
                            "children" :
                            (
                                {
                                    "name" : "tx_v_coins",
                                    "type" : "text",
                                    "text" : "100 Tokens",
                                    "x" : 0,
                                    "y" : 2,
                                    "color" : 0xffd6bd71,
                                    "horizontal_align" : "center",
                                    "text_horizontal_align" : "center",
                                },
                            ),
                        },
                        ## Achievementshop coins
                        {
                            "name" : "sb_a_coins",
                            "type" : "image",
                            "x" : 460,
                            "y" : 35,
                            "image" : MAINFOLDER+"itemshop/black_large_slotbar.sub",
                            
                            "children" :
                            (
                                {
                                    "name" : "tx_a_coins",
                                    "type" : "text",
                                    "text" : "100 L-Coins",
                                    "x" : 0,
                                    "y" : 3,
                                    "horizontal_align" : "center",
                                    "text_horizontal_align" : "center",
                                },
                            ),
                        },
                    ),
                },
                
                {
                    "name": "Startpage",
                    
                    "x" : 0,
                    "y" : 78,
                    
                    "width" : BOARDWIDTH,
                    "height" : 500,
                    "style" : ("movable", "attach", "animate",),
                    
                    "children": 
                    (
                        ## BANNER 
                        {
                            "name": "Banner",
                    
                            "x" : 0,
                            "y" : 0,
                    
                            "width" : BOARDWIDTH,
                            "height" : 250,

                            "style" : ("movable", "attach", "animate",),

                            "children": 
                            (
                                {
                                    "name" : "image_banner",
                                    "type" : "image",
                                    "style" : ("movable", "attach", "animate",),
                                    "x" : 18,
                                    "y" : 0,
                                    "image" : MAINFOLDER+"itemshop/banner/banner_0.jpg",
                                },
                                {
                                    "name" : "image_fade_banner",
                                    "type" : "image",
                                    "style" : ("movable", "attach", "animate",),
                                    "x" : 18,
                                    "y" : 0,
                                    "image" : MAINFOLDER+"itemshop/banner/banner_0.jpg",
                                },
                                ## Banner page one
                                {
                                    "name" : "btn_banner_0", 
                                    "type" : "toggle_button",
                                    "x" : 525+15,
                                    "y" : 230,
                                    "default_image" :  MAINFOLDER+"itemshop/dot_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/dot_hover.sub",
                                    "down_image" :  MAINFOLDER+"itemshop/dot_press.sub",
                                },
                                ## Banner page two
                                {
                                    "name" : "btn_banner_1", 
                                    "type" : "toggle_button",
                                    "x" : 545+15,
                                    "y" : 230,
                                    "default_image" :  MAINFOLDER+"itemshop/dot_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/dot_hover.sub",
                                    "down_image" :  MAINFOLDER+"itemshop/dot_press.sub",
                                },
                            ),
                        },

                        {
                            "name" : "barra_items_vendidos",
                            "type" : "image",
                            "x" : 16,
                            "y" : 254,
                            "image" : IMG_ITEMSHOP+"barra_items_vendidos.tga",
                            "children":
                            (
                                {
                                    "name" : "tx_MostBought",
                                    "type" : "text",
                                    "text" : localeInfo.ITEMSHOP_TOP_SELL,
                                    "x" : 170,
                                    "y" : 2,
                                },

                                {
                                    "name" : "tx_hotOffers",
                                    "type" : "text",
                                    "text" : localeInfo.ITEMSHOP_DISCOUNT_WEEK,
                                    "x" : 170+363,
                                    "y" : 2,
                                },

                            ),
                        },

                        ## HOT OFFERS
                        {
                            "name": "HotOffers",
                    
                            "x" : 490,
                            "y" : 260+20,
                    
                            "width" : 230,
                            "height" : 114+60,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (


                                {
                                    "name" : "btn_hotOffers_up", 
                                    "type" : "button",
                                    "x" : 30+8,
                                    "y" : 48,
                                    "default_image" : IMG_ITEMSHOP+"atras_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"atras_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"atras_3.tga",
                                },
                                {
                                    "name" : "btn_hotOffers_down", 
                                    "type" : "button",
                                    "x" : 182,
                                    "y" : 48,
                                    "default_image" : IMG_ITEMSHOP+"delante_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"delante_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"delante_3.tga",
                                },
                                    
                            ),
                        },
                        ## MOST BOUGHT
                        {
                            "name": "MostBought",
                    
                            "x" : 30,
                            "y" : 260+20,
                    
                            "width" : 470,
                            "height" : 114+60,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (

                                        
                                {
                                    "name" : "btn_mostBought_left", 
                                    "type" : "button",
                                    "x" : 11+10,
                                    "y" : 48,
                                    "default_image" : IMG_ITEMSHOP+"atras_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"atras_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"atras_3.tga",
                                },
                                {
                                    "name" : "btn_mostBought_right", 
                                    "type" : "button",
                                    "x" : 510-70,
                                    "y" : 48,
                                    "default_image" : IMG_ITEMSHOP+"delante_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"delante_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"delante_3.tga",
                                },
                            ),
                        },
                        ## Coins buy button
                        {
                            "name" : "btn_buy_coins", 
                            "type" : "button",
                            "text" : "Donation",

                            "x" : 592,
                            "y" : 356,

                            "default_image" : MAINFOLDER+"itemshop/btn_coins_buy_norm.sub",
                            "over_image" : MAINFOLDER+"itemshop/btn_coins_buy_hover.sub",
                            "down_image" : MAINFOLDER+"itemshop/btn_coins_buy_press.sub",
                        },
                        ## Vote button
                        # {
                            # "name" : "btn_vote", 
                            # "type" : "button",
                            # "text" : "VOTEN",

                            # "x" : 592,
                            # "y" : 356,

                            # "default_image" : MAINFOLDER+"itemshop/btn_coins_buy_norm.sub",
                            # "over_image" : MAINFOLDER+"itemshop/btn_coins_buy_hover.sub",
                            # "down_image" : MAINFOLDER+"itemshop/btn_coins_buy_press.sub",
                        # },
                    ),
                },
                {
                    "name": "Itemshop",
                    


                    "x" : 0,
                    "y" : 75,
                    
                    "width" : BOARDWIDTH,
                    "height" : 476,
                    "style" : ("movable", "attach", "animate",),
                    
                    "children": 
                    (

                        ## On Sale
                        {
                            "name": "IsOnSale",

                            #"type":"board",

                            "x" : 320,
                            "y" : 430,
                    
                            "width" : 300,
                            "height" : 30,
                            #"style" : ("movable", "attach",),
                            "children": 
                            (

                                {
                                    "name" : "btn_IsOnSale_left", 
                                    "type" : "button",
                                    "x" : 140,
                                    "y" : 14,
                                    "default_image" : IMG_ITEMSHOP+"btn_atras_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"btn_atras_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"btn_atras_3.tga",
                                },
                                {
                                    "name" : "btn_IsOnSale_right", 
                                    "type" : "button",
                                    "x" : 250,
                                    "y" : 14,
                                    "default_image" : IMG_ITEMSHOP+"btn_delante_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"btn_delante_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"btn_delante_3.tga",
                                },
                            ),
                        },
                        {
                            "name": "IsCategory",
                            
                            "x" : 29,
                            "y" : 6,
                    
                            "width" : 164,
                            "height" : 373,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (
                                {
                                    "name" : "btn_IsCategory_up", 
                                    "type" : "button",
                                    "x" : 68,
                                    "y" : 4,
                                    "default_image" : MAINFOLDER+"itemshop/arrow_up_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/arrow_up_hover.sub",
                                    "down_image" : MAINFOLDER+"itemshop/arrow_up_press.sub",
                                },
                                {
                                    "name" : "btn_IsCategory_down", 
                                    "type" : "button",
                                    "x" : 68,
                                    "y" : 335,
                                    "default_image" : MAINFOLDER+"itemshop/arrow_down_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/arrow_down_hover.sub",
                                    "down_image" : MAINFOLDER+"itemshop/arrow_down_press.sub",
                                },
                            ),
                        },

                        {
                            "name" : "tx_IsOnSale_pageNr_img",
                            "type" : "image",
                            "x" : 480,
                            "y" : 443,
                            "image" :  IMG_ITEMSHOP+"slot_paginas.tga",
                            
                        },

                        {
                            "name" : "tx_IsOnSale_pageNr",
                            "type" : "text",
                            "style" : ("movable", "attach", "animate",),
                            "text" : "0 / 1",
                            "x" : 525,
                            "y" : 443+3,
                            "text_horizontal_align":"center",
                        },

                    ),
                },
                {
                    "name": "Voteshop",
                    
                    "x" : 0,
                    "y" : 75,
                    
                    "width" : BOARDWIDTH,
                    "height" : 476,
                    "style" : ("movable", "attach", "animate",),
                    
                    "children": 
                    (
                        ## On Sale
                        {
                            "name": "VsOnSale",
                    
                            "x" : 320,
                            "y" : 430,
                    
                            "width" : 300,
                            "height" : 30,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (
                                

                                {
                                    "name" : "btn_VsOnSale_left", 
                                    "type" : "button",
                                    "x" : 140,
                                    "y" : 14,
                                    "default_image" : IMG_ITEMSHOP+"btn_atras_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"btn_atras_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"btn_atras_3.tga",
                                },
                                {
                                    "name" : "btn_VsOnSale_right", 
                                    "type" : "button",
                                    "x" : 250,
                                    "y" : 14,
                                    "default_image" : IMG_ITEMSHOP+"btn_delante_1.tga",
                                    "over_image" :IMG_ITEMSHOP+"btn_delante_2.tga",
                                    "down_image" : IMG_ITEMSHOP+"btn_delante_3.tga",
                                },
                            ),
                        },
                        {
                            "name": "VsCategory",
                    
                            "x" : 29,
                            "y" : 6,
                    
                            "width" : 164,
                            "height" : 373,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (
                                ## Category

                                {
                                    "name" : "btn_VsCategory_up", 
                                    "type" : "button",
                                    "x" : 68,
                                    "y" : 4,
                                    "default_image" : MAINFOLDER+"itemshop/arrow_up_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/arrow_up_hover.sub",
                                    "down_image" : MAINFOLDER+"itemshop/arrow_up_press.sub",
                                },
                                {
                                    "name" : "btn_VsCategory_down", 
                                    "type" : "button",
                                    "x" : 68,
                                    "y" : 335,
                                    "default_image" : MAINFOLDER+"itemshop/arrow_down_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/arrow_down_hover.sub",
                                    "down_image" : MAINFOLDER+"itemshop/arrow_down_press.sub",
                                },
                            ),
                        },

                        {
                            "name" : "tx_VsOnSale_pageNr_img",
                            "type" : "image",
                            "x" : 480,
                            "y" : 443,
                            "image" :  IMG_ITEMSHOP+"slot_paginas.tga",
                            
                        },


                        {
                            "name" : "tx_VsOnSale_pageNr",
                            "type" : "text",
                            "text" : "0 / 1",
                            "x" : 525,
                            "y" : 443+3,
                            
                            "text_horizontal_align":"center",
                        },
                    ),
                },
                
                {
                    "name": "Achievementshop",
                    
                    "x" : 0,
                    "y" : 100,
                    
                    "width" : BOARDWIDTH,
                    "height" : 500,
                    "style" : ("movable", "attach", "animate",),
                    
                    "children": 
                    (
                        ## On Sale
                        {
                            "name": "AsOnSale",
                    
                            "x" : 210,
                            "y" : 4,
                    
                            "width" : 513,
                            "height" : 376,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (
                                {
                                    "name" : "image_AsOnSale_background",
                                    "type" : "image",
                                    "x" : 0,
                                    "y" : 0,
                                    "image" :  MAINFOLDER+"itemshop/on_sale_box.sub",
                                    "children": 
                                    (
                                        {
                                            "name" : "btn_AsOnSale_left", 
                                            "type" : "button",
                                            "x" : 7,
                                            "y" : 170,
                                            "default_image" : MAINFOLDER+"itemshop/arrow_left_norm.sub",
                                            "over_image" : MAINFOLDER+"itemshop/arrow_left_hover.sub",
                                            "down_image" : MAINFOLDER+"itemshop/arrow_left_press.sub",
                                        },
                                        {
                                            "name" : "btn_AsOnSale_right", 
                                            "type" : "button",
                                            "x" : 469,
                                            "y" : 170,
                                            "default_image" : MAINFOLDER+"itemshop/arrow_right_norm.sub",
                                            "over_image" : MAINFOLDER+"itemshop/arrow_right_hover.sub",
                                            "down_image" : MAINFOLDER+"itemshop/arrow_right_press.sub",
                                        },
                                    ),
                                },
                            ),
                        },
                        {
                            "name": "AsCategory",
                    
                            "x" : 29,
                            "y" : 6,
                    
                            "width" : 164,
                            "height" : 373,
                            "style" : ("movable", "attach", "animate",),
                            "children": 
                            (
                                ## Category
                                {
                                    "name" : "image_AsCategory_background",
                                    "type" : "image",
                                    "x" : 0,
                                    "y" : 0,
                                    "image" :  MAINFOLDER+"itemshop/category_box.sub",
                                },
                                {
                                    "name" : "btn_AsCategory_up", 
                                    "type" : "button",
                                    "x" : 68,
                                    "y" : 4,
                                    "default_image" : MAINFOLDER+"itemshop/arrow_up_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/arrow_up_hover.sub",
                                    "down_image" : MAINFOLDER+"itemshop/arrow_up_press.sub",
                                },
                                {
                                    "name" : "btn_AsCategory_down", 
                                    "type" : "button",
                                    "x" : 68,
                                    "y" : 335,
                                    "default_image" : MAINFOLDER+"itemshop/arrow_down_norm.sub",
                                    "over_image" : MAINFOLDER+"itemshop/arrow_down_hover.sub",
                                    "down_image" : MAINFOLDER+"itemshop/arrow_down_press.sub",
                                },
                            ),
                        },
                        {
                            "name" : "tx_AsOnSale_pageNr",
                            "type" : "text",
                            "text" : "0 / 1",
                            "x" : 690,
                            "y" : 355,
                        },
                    ),
                },
                {
                    "name" : "titlebar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 8,
                    "y" : 8,

                    "width" : BOARDWIDTH-15,

                    "children" :
                    (
                        { "name":"TitleName", "type":"text", "x":370, "y":3, "text":localeInfo.ITEMSHOP_TITLE_PANEL,"color":0xFFE6D0A2, "text_horizontal_align":"center" },
                    ),
                },
            ),
        },
    ),
}