# coding: latin_1

import uiScriptLocale

IMG_DIR = "d:/ymir work/ui/game/gameoption/"
IMG_PICKUP_DIR = "d:/ymir work/ui/game/gameoption/pickup/"

TITLE_IMAGE_TEXT_X = 5
TITLE_IMAGE_TEXT_Y = 4

OPTION_START_X = 17
SLIDER_POSITION_X = 50

SLIDER_START_Y = 40
BUTTON_START_Y = 33
BUTTON_NEXT_Y = 20

RADIO_BUTTON_RANGE_X = 65
TOGGLE_BUTTON_RANGE_X = 85
TOGGLE_BUTTON_RANGE_X_NEW = 70

RADIO_BUTTON_TEXT_X = 25
TOGGLE_BUTTON_TEXT_X = 20

SMALL_OPTION_HEIGHT = 65
NORMAL_OPTION_HEIGHT = 80
SLIDER_OPTION_HEIGHT = 65


window = {
    "name" : "GameOptionDialog",
    # Dont touch these lines!
    "style" : (),
    "x" : 171,
    "y" : 3,
    "width" : 300,
    "height" : 324,
    # Dont touch these lines!
    "children" :
    (
        {
            "name" : "pickup_premimum_image",
            "type" : "expanded_image",
            "x" : 3,
            "y" : 0,
            "image" : IMG_PICKUP_DIR+"not_activated.tga",
            "children":
            (
                {
                    "name" : "pickup_premium_item",
                    "type" : "expanded_image",
                    "x" : 34,
                    "y" : 13,
                    "image" : "icon/item/70002.tga",
                },
                
                {
                    "name" : "pickup_premium_text_0",
                    "type" : "text",
                    "x" : 81,
                    "y" : 24,
                    "text_horizontal_align":"left",
                    "text":uiScriptLocale.AUTOMATIC_PICKUP_TEXT_0,
                    "outline":1,
                    "color":0xFFDADADA,
                },
                
                {
                    "name" : "pickup_premium_text_1",
                    "type" : "text",
                    "x" : 81,
                    "y" : 24+14,
                    "text_horizontal_align":"left",
                    "text":uiScriptLocale.AUTOMATIC_PICKUP_TEXT_1,
                    "outline":1,
                },

                {
                    "name" : "pickup_premium_text_2",
                    "type" : "text",
                    "x" : 7,
                    "y" : 52,
                    "text_horizontal_align":"left",
                    "text":uiScriptLocale.AUTOMATIC_PICKUP_TEXT_3,
                    "outline":1,
                    "color":0xFFFFA900,
                },

                {
                    "name" : "pickup_premium_text_3",
                    "type" : "text",
                    "x" : 7,
                    "y" : 70,
                    "text_horizontal_align":"left",
                    "text":uiScriptLocale.AUTOMATIC_PICKUP_TEXT_4,
                    "outline":1,
                    "color":0xFFDADADA,
                },
                
                {
                    "name" : "pickup_premium_text_4",
                    "type" : "text",
                    "x" : 7,
                    "y" : 80,
                    "text_horizontal_align":"left",
                    "text":uiScriptLocale.AUTOMATIC_PICKUP_TEXT_5,
                    "outline":1,
                    "color":0xFFDADADA,
                },
            ),
        },

        {
            "name" : "pickup_window",
            "type" : "window",
            "x" : 0,
            "y" : 107+8,
            "width":304,
            "height":SMALL_OPTION_HEIGHT,
            "children":
            (

                {
                    "name" : "pickup_title_img",
                    "type" : "expanded_image",
                    "x" : 0,
                    "y" : 0,
                    "image" : IMG_DIR+"option_title.tga",
                    "children":
                    (
                        {
                            "name" : "title_pickup",
                            "type" : "text",
                            "x" : TITLE_IMAGE_TEXT_X,
                            "y" : TITLE_IMAGE_TEXT_Y,
                            "text_horizontal_align":"left",
                            "text" : uiScriptLocale.AUTOMATIC_PICK_UP,
                        },
                    ),
                },
                {
                    "name" : "pick_up_button_activate",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*0,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_ACTIVATE,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_button_deactivate",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*1,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_DEACTIVATE,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_DIR + "toggle_selected.tga",
                },
            ),
        },

        {
            "name" : "filter_window",
            "type" : "window",
            "x" : 0,
            "y" : 107+8+SMALL_OPTION_HEIGHT,
            "width":304,
            "height":NORMAL_OPTION_HEIGHT+NORMAL_OPTION_HEIGHT-20,
            "children":
            (
                {
                    "name" : "filter_title_img",
                    "type" : "expanded_image",
                    "x" : 0,
                    "y" : 0,
                    "image" : IMG_DIR+"option_title.tga",
                    "children":
                    (
                        {
                            "name" : "filter_pickup",
                            "type" : "text",
                            "x" : TITLE_IMAGE_TEXT_X,
                            "y" : TITLE_IMAGE_TEXT_Y,
                            "text_horizontal_align":"left",
                            "text" : uiScriptLocale.AUTOMATIC_PICK_UP_FILTER,
                        },
                    ),
                },
                {
                    "name" : "pick_up_weapons",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*0,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_WEAPONS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_armors",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*1,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_ARMORS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_shield",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*2,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_SHIELD,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_ring",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*3,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_RING,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_helmets",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*0,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_HELMETS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_bracelets",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*1,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_BRACELETS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_necklace",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*2,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_NECKLACE,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_earrings",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*3,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_EARRINGS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                
                {
                    "name" : "pick_up_costume",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*0,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_COSTUME,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_petnmount",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*1,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_PETNMOUNT,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_sash",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*2,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_SASH,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_talisman",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*3,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_TALISMAN,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                
                {
                    "name" : "pick_up_books",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*0,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_BOOKS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_stones",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*1,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_STONES,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_yang",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*2,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_YANG,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_chests",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*3,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_CHESTS,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                
                {
                    "name" : "pick_up_foots",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*0,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_SHOES,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "pick_up_belt",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X_NEW*1,
                    "y" : 33+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_BELT,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
            ),
        },
        {
            "name" : "filter_rarity_window",
            "type" : "window",
            "x" : 0,
            "y" : 107+8+SMALL_OPTION_HEIGHT+NORMAL_OPTION_HEIGHT+NORMAL_OPTION_HEIGHT-20,
            "width":304,
            "height":NORMAL_OPTION_HEIGHT+SMALL_OPTION_HEIGHT-25,
            "children":
            (
                {
                    "name" : "filter_rarity_title_img",
                    "type" : "expanded_image",
                    "x" : 0,
                    "y" : 0,
                    "image" : IMG_DIR+"option_title.tga",
                    "children":
                    (
                        {
                            "name" : "filter_rarity_pickup",
                            "type" : "text",
                            "x" : TITLE_IMAGE_TEXT_X,
                            "y" : TITLE_IMAGE_TEXT_Y,
                            "text_horizontal_align":"left",
                            "text" : uiScriptLocale.AUTOMATIC_PICK_UP_FILTER_RARITY,
                        },
                    ),
                },
                {
                    "name" : "rarity_normal",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*0,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_RARITY_NORMAL,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "text_color":0xFF6B6869,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "rarity_uncommun",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*1,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_RARITY_UNCOMMUN,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "text_color":0xFFDADADA,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "rarity_rare",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*2,
                    "y" : 33,
                    "text" : uiScriptLocale.AUTOMATIC_RARITY_RARE,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "text_color":0xFF0071C3,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "rarity_epic",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*0,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_RARITY_EPIC,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "text_color":0xFFDA443D,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "rarity_relic",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*1,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_RARITY_RELIC,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "text_color":0xFF84CD4A,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                {
                    "name" : "rarity_legendary",
                    "type" : "toggle_button",
                    "x" : OPTION_START_X+TOGGLE_BUTTON_RANGE_X*2,
                    "y" : 33+BUTTON_NEXT_Y,
                    "text" : uiScriptLocale.AUTOMATIC_RARITY_LEGENDARY,
                    "text_x" : TOGGLE_BUTTON_TEXT_X,
                    "text_color":0xFFDE55D0,
                    "default_image" : IMG_DIR + "toggle_unselected.tga",
                    "over_image" : IMG_DIR + "toggle_unselected.tga",
                    "down_image" : IMG_PICKUP_DIR + "toggle_selected.tga",
                },
                
                {
                    "name" : "rarity_info",
                    "type" : "expanded_image",
                    "x" : 3,
                    "y" : 33+BUTTON_NEXT_Y+25,
                    "image" : IMG_PICKUP_DIR + "box_info.tga",
                    "children":
                    (
                        {
                            "name" : "filter_rarity_info",
                            "type" : "text",
                            "x" : 0,
                            "y" : 0,
                            "outline":1,
                            "all_align":1,
                            "text" : uiScriptLocale.AUTOMATIC_RARITY_INFO,
                            "color":0xFFFF4141,
                        },
                    ),
                },
            ),
        },
    ),
}