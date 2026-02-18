# coding: latin_1

import uiScriptLocale
import app

ROOT_PATH = "d:/ymir work/ui/public/"

TEMPORARY_X = +13
BUTTON_TEMPORARY_X = 5
PVP_X = -10

LINE_LABEL_X = 30
LINE_DATA_X = 90
LINE_BEGIN = 40
LINE_STEP = 25
SMALL_BUTTON_WIDTH = 45
MIDDLE_BUTTON_WIDTH = 65
NEW_POSITION_X = 45
NEW_SIZE_X = 60

window = {
    "name" : "GameOptionDialog",
    "style" : ("movable", "float", "animate",),

    "x" : 0,
    "y" : 0,

    "width" : 300+NEW_SIZE_X,
    "height" : 25*11+8+45+30,

    "children" :
    (
        {
            "name" : "board",
            "type" : "board",

            "x" : 0,
            "y" : 0,

            "width" : 300+NEW_SIZE_X,
            "height" : 25*11+8+45+30,

            "children" :
            (
                ## Title
                {
                    "name" : "titlebar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 8,
                    "y" : 8,

                    "width" : 284+NEW_SIZE_X,
                    "color" : "gray",

                    "children" :
                    (
                        { "name":"titlename", "type":"text", "x":0, "y":3, 
                        "text" : uiScriptLocale.GAMEOPTION_TITLE, 
                        "horizontal_align":"center", "text_horizontal_align":"center" },
                    ),
                },

                ## ÀÌ¸§»ö
                {
                    "name" : "name_color",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 40+2,

                    "text" : uiScriptLocale.OPTION_NAME_COLOR,
                },
                {
                    "name" : "name_color_normal",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                    "y" : 40,

                    "text" : uiScriptLocale.OPTION_NAME_COLOR_NORMAL,

                    "default_image" : ROOT_PATH + "Middle_Button_01.sub",
                    "over_image" : ROOT_PATH + "Middle_Button_02.sub",
                    "down_image" : ROOT_PATH + "Middle_Button_03.sub",
                },
                {
                    "name" : "name_color_empire",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1+NEW_POSITION_X,
                    "y" : 40,

                    "text" : uiScriptLocale.OPTION_NAME_COLOR_EMPIRE,

                    "default_image" : ROOT_PATH + "Middle_Button_01.sub",
                    "over_image" : ROOT_PATH + "Middle_Button_02.sub",
                    "down_image" : ROOT_PATH + "Middle_Button_03.sub",
                },

                ## Å¸°ÙÃ¢
                {
                    "name" : "target_board",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 65+2,

                    "text" : uiScriptLocale.OPTION_TARGET_BOARD,
                },
                {
                    "name" : "target_board_no_view",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                    "y" : 65,

                    "text" : uiScriptLocale.OPTION_TARGET_BOARD_NO_VIEW,

                    "default_image" : ROOT_PATH + "Middle_Button_01.sub",
                    "over_image" : ROOT_PATH + "Middle_Button_02.sub",
                    "down_image" : ROOT_PATH + "Middle_Button_03.sub",
                },
                {
                    "name" : "target_board_view",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1+NEW_POSITION_X,
                    "y" : 65,

                    "text" : uiScriptLocale.OPTION_TARGET_BOARD_VIEW,

                    "default_image" : ROOT_PATH + "Middle_Button_01.sub",
                    "over_image" : ROOT_PATH + "Middle_Button_02.sub",
                    "down_image" : ROOT_PATH + "Middle_Button_03.sub",
                },

                
                ## PvP Mode
                {
                    "name" : "pvp_mode",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 90+2,

                    "text" : uiScriptLocale.OPTION_PVPMODE,
                },
                {
                    "name" : "pvp_peace",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*0+NEW_POSITION_X,
                    "y" : 90,

                    "text" : uiScriptLocale.OPTION_PVPMODE_PEACE,
                    "tooltip_text" : uiScriptLocale.OPTION_PVPMODE_PEACE_TOOLTIP,

                    "default_image" : ROOT_PATH + "small_Button_01.sub",
                    "over_image" : ROOT_PATH + "small_Button_02.sub",
                    "down_image" : ROOT_PATH + "small_Button_03.sub",
                },
                {
                    "name" : "pvp_revenge",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*1+NEW_POSITION_X,
                    "y" : 90,

                    "text" : uiScriptLocale.OPTION_PVPMODE_REVENGE,
                    "tooltip_text" : uiScriptLocale.OPTION_PVPMODE_REVENGE_TOOLTIP,

                    "default_image" : ROOT_PATH + "small_Button_01.sub",
                    "over_image" : ROOT_PATH + "small_Button_02.sub",
                    "down_image" : ROOT_PATH + "small_Button_03.sub",
                },
                {
                    "name" : "pvp_guild",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*2+NEW_POSITION_X,
                    "y" : 90,

                    "text" : uiScriptLocale.OPTION_PVPMODE_GUILD,
                    "tooltip_text" : uiScriptLocale.OPTION_PVPMODE_GUILD_TOOLTIP,

                    "default_image" : ROOT_PATH + "small_Button_01.sub",
                    "over_image" : ROOT_PATH + "small_Button_02.sub",
                    "down_image" : ROOT_PATH + "small_Button_03.sub",
                },
                {
                    "name" : "pvp_free",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*3+NEW_POSITION_X,
                    "y" : 90,

                    "text" : uiScriptLocale.OPTION_PVPMODE_FREE,
                    "tooltip_text" : uiScriptLocale.OPTION_PVPMODE_FREE_TOOLTIP,

                    "default_image" : ROOT_PATH + "small_Button_01.sub",
                    "over_image" : ROOT_PATH + "small_Button_02.sub",
                    "down_image" : ROOT_PATH + "small_Button_03.sub",
                },

                ## Block
                {
                    "name" : "block",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 115+2,

                    "text" : uiScriptLocale.OPTION_BLOCK,
                },
                {
                    "name" : "block_exchange_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                    "y" : 115,

                    "text" : uiScriptLocale.OPTION_BLOCK_EXCHANGE,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "block_party_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1+NEW_POSITION_X,
                    "y" : 115,

                    "text" : uiScriptLocale.OPTION_BLOCK_PARTY,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "block_guild_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2+NEW_POSITION_X,
                    "y" : 115,

                    "text" : uiScriptLocale.OPTION_BLOCK_GUILD,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "block_whisper_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                    "y" : 140,

                    "text" : uiScriptLocale.OPTION_BLOCK_WHISPER,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "block_friend_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1+NEW_POSITION_X,
                    "y" : 140,

                    "text" : uiScriptLocale.OPTION_BLOCK_FRIEND,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "block_party_request_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2+NEW_POSITION_X,
                    "y" : 140,

                    "text" : uiScriptLocale.OPTION_BLOCK_PARTY_REQUEST,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },

                ## Chat
                {
                    "name" : "chat",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 165+2,

                    "text" : uiScriptLocale.OPTION_VIEW_CHAT,
                },
                {
                    "name" : "view_chat_on_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : 165,

                    "text" : uiScriptLocale.OPTION_VIEW_CHAT_ON,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "view_chat_off_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : 165,

                    "text" : uiScriptLocale.OPTION_VIEW_CHAT_OFF,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },

                ## Always Show Name
                {
                    "name" : "always_show_name",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 190+2,

                    "text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME,
                },
                {
                    "name" : "always_show_name_on_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : 190,

                    "text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME_ON,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "always_show_name_off_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : 190,

                    "text" : uiScriptLocale.OPTION_ALWAYS_SHOW_NAME_OFF,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },

                ## Effect On/Off
                {
                    "name" : "effect_on_off",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 215+2,

                    "text" : uiScriptLocale.OPTION_EFFECT,
                },
                {
                    "name" : "show_damage_on_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : 215,

                    "text" : uiScriptLocale.OPTION_VIEW_CHAT_ON,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "show_damage_off_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : 215,

                    "text" : uiScriptLocale.OPTION_VIEW_CHAT_OFF,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },

                ## ÆÇ¸Å¹®±¸
                {
                    "name" : "salestext_on_off",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : 240+2,

                    "text" : uiScriptLocale.OPTION_SALESTEXT,
                },
                {
                    "name" : "salestext_on_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : 240,

                    "text" : uiScriptLocale.OPTION_SALESTEXT_VIEW_ON,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "salestext_off_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : 240,

                    "text" : uiScriptLocale.OPTION_SALESTEXT_VIEW_OFF,

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",                                
                },
                {
                    "name" : "salestext_range",
                    "type" : "text",
                    "x" : LINE_LABEL_X,
                    "y" : 269+30,
                    "text" : uiScriptLocale.OPTION_SALESTEXT_RANGE,
                },
                {
                    "name" : "salestext_range_controller",
                    "type" : "sliderbar",
                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH - 37,
                    "y" : 271+30,
                },
            ),
        },
    ),
}

CUR_LINE_Y = LINE_BEGIN + LINE_STEP * 8

if app.WJ_SHOW_MOB_INFO:
    CUR_LINE_Y += LINE_STEP
    window["height"] = window["height"]
    window["children"][0]["height"] = window["children"][0]["height"]
    window["children"][0]["children"] = window["children"][0]["children"] + (
                    ## Show Mob Info List
                    {
                        "name" : "show_mob_info",
                        "type" : "text",
                        
                        "multi_line" : 1,
                        
                        "x" : LINE_LABEL_X,
                        "y" : CUR_LINE_Y+2,

                        "text" : uiScriptLocale.OPTION_MOB_INFO,
                    },
                    {
                        "name" : "show_mob_level_button",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+NEW_POSITION_X,
                        "y" : CUR_LINE_Y,

                        "text" : uiScriptLocale.OPTION_MOB_INFO_LEVEL,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "show_mob_AI_flag_button",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                        "y" : CUR_LINE_Y,

                        "text" : uiScriptLocale.OPTION_MOB_INFO_AGGR,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },)

if app.ENABLE_OPTION_COLLECT_EQUIPMENT:
    CUR_LINE_Y += LINE_STEP + 40

    window["height"] = window["height"] + 25
    window["children"][0]["height"] = window["children"][0]["height"] + 25
    window["children"][0]["children"] = window["children"][0]["children"] + (

                {
                    "name" : "pick_options",
                    "type" : "text",

                    "multi_line" : 1,

                    "x" : LINE_LABEL_X,
                    "y" : CUR_LINE_Y + 2,

                    "text" : "Pick Options",
                },
                {
                    "name" : "pick_up_weapon_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "Weapon",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "pick_up_armor_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X + MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "Armor",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "pick_up_costume_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X + MIDDLE_BUTTON_WIDTH * 2+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "Costume",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },)

    CUR_LINE_Y += LINE_STEP

    window["height"] = window["height"] + 25
    window["children"][0]["height"] = window["children"][0]["height"] + 25
    window["children"][0]["children"] = window["children"][0]["children"] + (

                {
                    "name" : "pick_up_book_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "book",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                
                {
                    "name" : "pick_up_stones_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X + MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "stones",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                
                {
                    "name" : "pick_up_upgrade_button",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X + MIDDLE_BUTTON_WIDTH * 2+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "upgrade",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },)
                
    CUR_LINE_Y += LINE_STEP
    window["height"] = window["height"]
    window["children"][0]["height"] = window["children"][0]["height"]
    window["children"][0]["children"] = window["children"][0]["children"] + (

                {
                    "name" : "pick_up_fast_normal",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : CUR_LINE_Y + 2,

                    "text" :"Pick Speed",
                },
                {
                    "name" : "pick_up_fast_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "Fast",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "pick_up_normal_button",
                    "type" : "radio_button",

                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "Normal",

                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },)

if app.ENABLE_SNOW_MODE:
    CUR_LINE_Y += LINE_STEP 
    window["height"] = window["height"]
    window["children"][0]["height"] = window["children"][0]["height"]
    window["children"][0]["children"] = window["children"][0]["children"] + (
                    ## Show Mob Info List
                    {
                        "name" : "show_snow_info",
                        "type" : "text",
                        
                        "multi_line" : 1,
                        
                        "x" : LINE_LABEL_X,
                        "y" : CUR_LINE_Y+2,

                        "text" : "Textura",
                    },
                    {
                        "name" : "show_snow_button",
                        "type" : "radio_button",

                        "x" : LINE_DATA_X+NEW_POSITION_X,
                        "y" : CUR_LINE_Y,

                        "text" : "Winter",

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "hide_snow_button",
                        "type" : "radio_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                        "y" : CUR_LINE_Y,

                        "text" : "Normal",

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },)

if app.ENABLE_HIDE_OBJECTS:        
    CUR_LINE_Y += LINE_STEP 
    window["height"] = window["height"]
    window["children"][0]["height"] = window["children"][0]["height"]
    window["children"][0]["children"] = window["children"][0]["children"] + (
                    ## Show Mob Info List
                    # SetVisible
                {
                    "name" : "visible_board",
                    "type" : "text",

                    "x" : LINE_LABEL_X,
                    "y" : CUR_LINE_Y,

                    "text" : "Visible",
                },
                {
                    "name" : "vsble0",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*0+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "TREE",

                    "default_image" : ROOT_PATH + "small_Button_01.sub",
                    "over_image" : ROOT_PATH + "small_Button_02.sub",
                    "down_image" : ROOT_PATH + "small_Button_03.sub",
                },
                {
                    "name" : "vsble1",
                    "type" : "toggle_button",

                    "x" : LINE_DATA_X+SMALL_BUTTON_WIDTH*1+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,

                    "text" : "CLOUD",

                    "default_image" : ROOT_PATH + "small_Button_01.sub",
                    "over_image" : ROOT_PATH + "small_Button_02.sub",
                    "down_image" : ROOT_PATH + "small_Button_03.sub",
                },)

if app.ENABLE_AUTOMATIC_PICK_UP_SYSTEM:
    CUR_LINE_Y += LINE_STEP
    window["height"] = window["height"] + 100
    window["children"][0]["height"] = window["children"][0]["height"] + 100
    window["children"][0]["children"] = window["children"][0]["children"] + (
                    ## Show Mob Info List
                    {
                        "name" : "automatic_pick_up_text",
                        "type" : "text",

                        "multi_line" : 1,

                        "x" : LINE_LABEL_X,
                        "y" : CUR_LINE_Y+2,

                        "text" : uiScriptLocale.AUTOMATIC_PICK_UP,
                    },
                    {
                        "name" : "pick_up_button_activate",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+NEW_POSITION_X,
                        "y" : CUR_LINE_Y,

                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_ACTIVATE,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_button_deactivate",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                        "y" : CUR_LINE_Y,

                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_DEACTIVATE,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_weapons",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 1),

                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_WEAPONS,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_armors",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 1),

                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_ARMORS,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_accessories",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 1),

                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_ACCESSORIES,
                        
                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_stones",
                        "type" : "toggle_button",
                        
                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 2),
                        
                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_STONES,
                        
                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_books",
                        "type" : "toggle_button",
                        
                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*1+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 2),
                        
                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_BOOKS,
                        
                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_blacksmith",
                        "type" : "toggle_button",
                        
                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*2+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 2),
                        
                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_BLACKSMITH,
                        
                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },
                    {
                        "name" : "pick_up_gold",
                        "type" : "toggle_button",

                        "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH*0+NEW_POSITION_X,
                        "y" : CUR_LINE_Y + (LINE_STEP * 3),

                        "text" : uiScriptLocale.AUTOMATIC_PCIK_UP_GOLD,

                        "default_image" : ROOT_PATH + "middle_button_01.sub",
                        "over_image" : ROOT_PATH + "middle_button_02.sub",
                        "down_image" : ROOT_PATH + "middle_button_03.sub",
                    },)

CUR_LINE_Y += LINE_STEP + (LINE_STEP * 2)
window["height"] = window["height"] + 10
window["children"][0]["height"] = window["children"][0]["height"] + 10
window["children"][0]["children"] = window["children"][0]["children"] + (

                ## Outline Info List
                {
                    "name" : "show_outline_info",
                    "type" : "text",
                    "multi_line" : 1,
                    "x" : LINE_LABEL_X,
                    "y" : CUR_LINE_Y+2,
                    "text" : uiScriptLocale.OPTION_OUTLINE_INFO,
                },
                {
                    "name" : "outline_on_button",
                    "type" : "radio_button",
                    "x" : LINE_DATA_X+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,
                    "text" : uiScriptLocale.OPTION_OUTLINE_ON,
                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },
                {
                    "name" : "outline_off_button",
                    "type" : "radio_button",
                    "x" : LINE_DATA_X+MIDDLE_BUTTON_WIDTH+NEW_POSITION_X,
                    "y" : CUR_LINE_Y,
                    "text" : uiScriptLocale.OPTION_OUTLINE_OFF,
                    "default_image" : ROOT_PATH + "middle_button_01.sub",
                    "over_image" : ROOT_PATH + "middle_button_02.sub",
                    "down_image" : ROOT_PATH + "middle_button_03.sub",
                },)
