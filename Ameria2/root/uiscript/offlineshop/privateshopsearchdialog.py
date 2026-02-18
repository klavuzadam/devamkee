# coding: latin_1

import localeInfo

ROOT = "d:/ymir work/ui/game/privatesearch/"
IMG_DIR = "d:/ymir work/ui/game/privatesearch/"

TEXT_Y_DIFFERENCE = -5

window = {
    "name" : "PrivateShopSearchDialog",
    "x" : 0,
    "y" : 0,
    "style" : ("movable", "float",),
    "width" : 841,
    "height" : 492,
    "children" :
    (
        {
            "name" : "board",
            "type" : "image",
            "style" : ("attach", "movable",),
            "x" : 0,
            "y" : 0,
            "image":IMG_DIR+"bg.tga",
            "children":
            (
                {"name" : "close","type" : "button","x" : 806,"y" : 4,"default_image" : IMG_DIR+"close_0.tga", "over_image" : IMG_DIR+"close_1.tga", "down_image" : IMG_DIR+"close_2.tga", },
                {"name" : "title","type" : "text","x" : 0,"y" : 14+TEXT_Y_DIFFERENCE ,"text" : localeInfo.PRIVATESHOPSEARCH_SEARCH_BAR,"horizontal_align" : "center",},
                {"name" : "class_text","type" : "text","x" : 85,"y" : 50+TEXT_Y_DIFFERENCE ,"text" : localeInfo.SHOPSEARCH_CLASS,},
                {"name" : "itemNameText","type" : "text","x" : 295,"y" : 50+TEXT_Y_DIFFERENCE ,"text" : localeInfo.PRIVATESHOPSEARCH_ITEMNAME,},
                {"name" : "sellerText","type" : "text","x" : 438,"y" : 50+TEXT_Y_DIFFERENCE,"text" : localeInfo.PRIVATESHOPSEARCH_SELLER,},
                {"name" : "countText","type" : "text","x" : 530,"y" : 50+TEXT_Y_DIFFERENCE,"text" : localeInfo.PRIVATESHOPSEARCH_COUNT,},
                {"name" : "rightpriceText","type" : "text","x" : 650,"y" : 50+TEXT_Y_DIFFERENCE,"text" : localeInfo.SHOPSEARCH_PRICE,},
                {"name" : "objectsText","type" : "text","x" : 81,"y" : 117+TEXT_Y_DIFFERENCE,"text" : localeInfo.SHOPSEARCH_OBJECTS,},
                {"name" : "min-maxText","type" : "text","x" : 51,"y" : 209+TEXT_Y_DIFFERENCE,"text" : localeInfo.SHOPSEARCH_FILTERS,},
                {"name" : "leftpriceText","type" : "text","x" : 86,"y" : 301+TEXT_Y_DIFFERENCE,"text" : localeInfo.SHOPSEARCH_PRICE,},
                {"name" : "leftpriceText","type" : "text","x" : 63,"y" : 396+TEXT_Y_DIFFERENCE,"text" :localeInfo.SHOPSEARCH_SEARCH_TEXT,},
                {"name" : "first~","type" : "text","x" : 97,"y" : 241+TEXT_Y_DIFFERENCE,"text" : "~",},
                {"name" : "second~","type" : "text","x" : 97,"y" : 271+TEXT_Y_DIFFERENCE,"text" : "~",},

                {
                    "name" : "editLineWnd",
                    "type" : "window",
                    "style" : ("not_pick",),
                    "x" : 4,
                    "y" : 226,
                    "width" : 196,
                    "height" : 220,
                    "children":
                    (
                        {
                            "name" : "minLevel",
                            "type" : "editline",
                            "x" : 5+30,
                            "y" : 2+7,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 3,
                            "only_number" : 1,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": "1",
                        },
                        {
                            "name" : "maxLevel",
                            "type" : "editline",
                            "x" : 5+105+30,
                            "y" : 2+7,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 3,
                            "only_number" : 1,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": "120",
                        },
                        
                        {
                            "name" : "minRefine",
                            "type" : "editline",
                            "x" : 5+30,
                            "y" : 26+11,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 3,
                            "only_number" : 1,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": "+0",
                        },

                        {
                            "name" : "maxRefine",
                            "type" : "editline",
                            "x" : 5+105+30,
                            "y" : 26+11,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 3,
                            "only_number" : 1,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": "+9",
                        },
                        
                        {
                            "name" : "minYang",
                            "type" : "editline",
                            "x" : 5+1,
                            "y" : 94+11,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 16,
                            "only_number" : 1,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": "0 Yang",
                        },

                        {
                            "name" : "maxYang",
                            "type" : "editline",
                            "x" : 5+1,
                            "y" : 123+11,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 16,
                            "only_number" : 1,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": "999.999.999.999.999",
                        },

                        {
                            "name" : "searchItemName",
                            "type" : "editline",
                            "x" : 5+1,
                            "y" : 193 + 6,
                            "width" : 194,
                            "height" : 26,
                            "input_limit" : 35,
                            "horizontal_align" : "center",
                            "text":"",
                            "info_msg": localeInfo.PRIVATESHOPSEARCH_ITEMNAME,
                            "children":
                            (
                                {
                                    "name" : "searchClearBtn",
                                    "type" : "button",
                                    "x" : 194-25,
                                    "y" : 1,
                                    "default_image" : ROOT+"clear_button_1.tga",
                                    "over_image" : ROOT+"clear_button_2.tga",
                                    "down_image" : ROOT+"clear_button_1.tga",
                                },
                            ),
                        },
                    ),
                },

                
                {
                    "name" : "ListBox",
                    "type" : "listbox_new",
                    "style" : ("attach",),
                    "x" : 207,
                    "y" : 75,
                    "width" : 638,
                    "height" : 418,
                    "viewcount" : 10,
                    "children":
                    (
                        {"name" : "listboxText","type" : "text", "x" : 0, "y" : 0, "text" : "", "all_align":1,},
                    ),
                },

                {"name" : "searchBtn","type" : "button","x" : 31,"y" : 456,"text" : localeInfo.SHOPSEARCH_SEARCH_BTN,"default_image" : IMG_DIR+"search_0.tga","over_image" : IMG_DIR+"search_1.tga","down_image" : IMG_DIR+"search_2.tga",},
                {"name" : "buyBtn","type" : "button","x" : 685,"y" : 460,"text" : localeInfo.SHOPSEARCH_BUY_BTN,"default_image" : IMG_DIR+"search_0.tga","over_image" : IMG_DIR+"search_1.tga","down_image" : IMG_DIR+"search_2.tga",},

                {"name" : "first_prev_button", "type" : "button","x" : 370, "y" : 469,"default_image" : IMG_DIR+"first_page_0.tga","over_image" : IMG_DIR+"first_page_1.tga","down_image" : IMG_DIR+"first_page_1.tga",},
                {"name" : "prev_button", "type" : "button","x" : 386, "y" : 469,"default_image" : IMG_DIR+"back_page_0.tga","over_image" : IMG_DIR+"back_page_1.tga","down_image" : IMG_DIR+"back_page_2.tga",},
                {"name" : "next_button", "type" : "button","x" : 554, "y" : 469,"default_image" : IMG_DIR+"next_page_0.tga","over_image" : IMG_DIR+"next_page_1.tga","down_image" : IMG_DIR+"next_page_1.tga",},
                {"name" : "last_next_button", "type" : "button","x" : 566, "y" : 469,"default_image" : IMG_DIR+"last_page_0.tga","over_image" : IMG_DIR+"last_page_1.tga","down_image" : IMG_DIR+"last_page_1.tga",},

                {"name" : "page1_button", "type" : "radio_button","x" : 403 + (29 * 0), "y" : 461,"text" : "1","default_image" : IMG_DIR+"page_box_0.tga","over_image" : IMG_DIR+"page_box_1.tga","down_image" : IMG_DIR+"page_box_1.tga",},
                {"name" : "page2_button", "type" : "radio_button","x" : 403 + (29 * 1), "y" : 461,"text" : "2","default_image" : IMG_DIR+"page_box_0.tga","over_image" : IMG_DIR+"page_box_1.tga","down_image" : IMG_DIR+"page_box_1.tga",},
                {"name" : "page3_button", "type" : "radio_button","x" : 403 + (29 * 2), "y" : 461,"text" : "3","default_image" : IMG_DIR+"page_box_0.tga","over_image" : IMG_DIR+"page_box_1.tga","down_image" : IMG_DIR+"page_box_1.tga",},
                {"name" : "page4_button", "type" : "radio_button","x" : 403 + (29 * 3), "y" : 461,"text" : "4","default_image" : IMG_DIR+"page_box_0.tga","over_image" : IMG_DIR+"page_box_1.tga","down_image" : IMG_DIR+"page_box_1.tga",},
                {"name" : "page5_button", "type" : "radio_button","x" : 403 + (29 * 4), "y" : 461,"text" : "5","default_image" : IMG_DIR+"page_box_0.tga","over_image" : IMG_DIR+"page_box_1.tga","down_image" : IMG_DIR+"page_box_1.tga",},
            ),
        },
    ),
}
