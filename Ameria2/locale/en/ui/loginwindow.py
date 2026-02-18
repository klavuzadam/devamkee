# coding: latin_1

import uiScriptLocale
import localeInfo

ROOT_PATH = "d:/ymir work/ui/public/"



LOCALE_PATH = uiScriptLocale.LOGIN_PATH
#Big-List
#SERVER_BOARD_HEIGHT = 180 + 390
#SERVER_LIST_HEIGHT = 171 + 350
#Small list like german
SERVER_BOARD_HEIGHT = 220 + 180
SERVER_LIST_HEIGHT = 171 + 180
SERVER_BOARD_WEIGHT = 375 

ID_LIMIT_COUNT = 19
PW_LIMIT_COUNT = 16

window = {
    "name" : "LoginWindow",
    "sytle" : ("movable", "animate",),

    "x" : 0,
    "y" : 0,

    "width" : SCREEN_WIDTH,
    "height" : SCREEN_HEIGHT,

    "children" :
    (

        ## Board
        {
            "name" : "bg", "type" : "expanded_image", "x" : 0, "y" : 0,
            "x_scale" : float(SCREEN_WIDTH) / 1920.0, "y_scale" : float(SCREEN_HEIGHT) / 1080.0,
            "image" : "locale/en/ui/login/background.tga",
        },

        ## VirtualKeyboard
        # {
            # 'name' : 'VirtualKeyboard',
            # 'type' : 'thinboard',
            # 'x' : (SCREEN_WIDTH - 564) / 2,
            # 'y' : SCREEN_HEIGHT - 300,
            # 'width' : 564,
            # 'height' : 254,
            # 'children' : 
            # (
                # {
                    # 'name' : 'key_at',
                    # 'type' : 'toggle_button',
                    # 'x' : 40,
                    # 'y' : 186,
                    # 'default_image' : 'locale/en/ui/vkey/key_at.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_at_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_at_over.tga',
                # },
                # {
                    # 'name' : 'key_backspace',
                    # 'type' : 'button',
                    # 'x' : 498,
                    # 'y' : 186,
                    # 'default_image' : 'locale/en/ui/vkey/key_backspace.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_backspace_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_backspace_over.tga',
                # },
                # {
                    # 'name' : 'key_enter',
                    # 'type' : 'button',
                    # 'x' : 439,
                    # 'y' : 186,
                    # 'default_image' : 'locale/en/ui/vkey/key_enter.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_enter_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_enter_over.tga',
                # },
                # {
                    # 'name' : 'key_shift',
                    # 'type' : 'toggle_button',
                    # 'x' : 86,
                    # 'y' : 186,
                    # 'default_image' : 'locale/en/ui/vkey/key_shift.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_shift_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_shift_over.tga',
                # },
                # {
                    # 'name' : 'key_space',
                    # 'type' : 'button',
                    # 'x' : 145,
                    # 'y' : 186,
                    # 'default_image' : 'locale/en/ui/vkey/key_space.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_space_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_space_over.tga',
                # },
                # {
                    # 'name' : 'key_1',
                    # 'type' : 'button',
                    # 'x' : 40,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_2',
                    # 'type' : 'button',
                    # 'x' : 80,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_3',
                    # 'type' : 'button',
                    # 'x' : 120,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_4',
                    # 'type' : 'button',
                    # 'x' : 160,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_5',
                    # 'type' : 'button',
                    # 'x' : 200,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_6',
                    # 'type' : 'button',
                    # 'x' : 240,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_7',
                    # 'type' : 'button',
                    # 'x' : 280,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_8',
                    # 'type' : 'button',
                    # 'x' : 320,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_9',
                    # 'type' : 'button',
                    # 'x' : 360,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_10',
                    # 'type' : 'button',
                    # 'x' : 400,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_11',
                    # 'type' : 'button',
                    # 'x' : 440,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_12',
                    # 'type' : 'button',
                    # 'x' : 480,
                    # 'y' : 24,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_13',
                    # 'type' : 'button',
                    # 'x' : 40,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_14',
                    # 'type' : 'button',
                    # 'x' : 80,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_15',
                    # 'type' : 'button',
                    # 'x' : 120,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_16',
                    # 'type' : 'button',
                    # 'x' : 160,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_17',
                    # 'type' : 'button',
                    # 'x' : 200,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_18',
                    # 'type' : 'button',
                    # 'x' : 240,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_19',
                    # 'type' : 'button',
                    # 'x' : 280,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_20',
                    # 'type' : 'button',
                    # 'x' : 320,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_21',
                    # 'type' : 'button',
                    # 'x' : 360,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_22',
                    # 'type' : 'button',
                    # 'x' : 400,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_23',
                    # 'type' : 'button',
                    # 'x' : 440,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_24',
                    # 'type' : 'button',
                    # 'x' : 480,
                    # 'y' : 63,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_25',
                    # 'type' : 'button',
                    # 'x' : 60,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_26',
                    # 'type' : 'button',
                    # 'x' : 100,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_27',
                    # 'type' : 'button',
                    # 'x' : 140,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_28',
                    # 'type' : 'button',
                    # 'x' : 180,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_29',
                    # 'type' : 'button',
                    # 'x' : 220,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_30',
                    # 'type' : 'button',
                    # 'x' : 260,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_31',
                    # 'type' : 'button',
                    # 'x' : 300,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_32',
                    # 'type' : 'button',
                    # 'x' : 340,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_33',
                    # 'type' : 'button',
                    # 'x' : 380,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_34',
                    # 'type' : 'button',
                    # 'x' : 420,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_35',
                    # 'type' : 'button',
                    # 'x' : 460,
                    # 'y' : 104,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_36',
                    # 'type' : 'button',
                    # 'x' : 60,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_37',
                    # 'type' : 'button',
                    # 'x' : 100,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_38',
                    # 'type' : 'button',
                    # 'x' : 140,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_39',
                    # 'type' : 'button',
                    # 'x' : 180,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_40',
                    # 'type' : 'button',
                    # 'x' : 220,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_41',
                    # 'type' : 'button',
                    # 'x' : 260,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_42',
                    # 'type' : 'button',
                    # 'x' : 300,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_43',
                    # 'type' : 'button',
                    # 'x' : 340,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_44',
                    # 'type' : 'button',
                    # 'x' : 380,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_45',
                    # 'type' : 'button',
                    # 'x' : 420,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
                # {
                    # 'name' : 'key_46',
                    # 'type' : 'button',
                    # 'x' : 460,
                    # 'y' : 144,
                    # 'default_image' : 'locale/en/ui/vkey/key_normal.tga',
                    # 'down_image' : 'locale/en/ui/vkey/key_normal_dn.tga',
                    # 'over_image' : 'locale/en/ui/vkey/key_normal_over.tga',
                # },
            # )
        # },

        # ConnectBoard
        {
            "name" : "ConnectBoard",
            "type" : "thinboard",

            "x" : (SCREEN_WIDTH - 564) / 2,
            "y" : (SCREEN_HEIGHT - 410 - 35),
            "width" : 208,
            "height" : 30,

            "children" :
            (
                {
                    "name" : "ConnectName",
                    "type" : "text",

                    "x" : 15,
                    "y" : 0,
                    "vertical_align" : "center",
                    "text_vertical_align" : "center",

                    "text" : uiScriptLocale.LOGIN_DEFAULT_SERVERADDR,
                },
                {
                    "name" : "SelectConnectButton",
                    "type" : "button",

                    "x" : 150,
                    "y" : 0,
                    "vertical_align" : "center",

                    "default_image" : "d:/ymir work/ui/public/small_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/small_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/small_button_03.sub",

                    "text" : uiScriptLocale.LOGIN_SELECT_BUTTON,
                },
            ),
        },

        ## LoginBoard
        {
            "name" : "LoginBoard",
            "type" : "image",

            "x" : (SCREEN_WIDTH - 564) / 2,
            "y" : (SCREEN_HEIGHT - 410),

            "image" : LOCALE_PATH + "loginwindow.sub",

            "children" :
            (
                {
                    "name" : "ID_EditLine",
                    "type" : "editline",

                    "x" : 77,
                    "y" : 16,

                    "width" : 120,
                    "height" : 18,

                    "input_limit" : ID_LIMIT_COUNT,
                    "enable_codepage" : 0,

                    "r" : 1.0,
                    "g" : 1.0,
                    "b" : 1.0,
                    "a" : 1.0,
                },
                {
                    "name" : "Password_EditLine",
                    "type" : "editline",

                    "x" : 77,
                    "y" : 43,

                    "width" : 120,
                    "height" : 18,

                    "input_limit" : PW_LIMIT_COUNT,
                    "secret_flag" : 1,
                    "enable_codepage" : 0,

                    "r" : 1.0,
                    "g" : 1.0,
                    "b" : 1.0,
                    "a" : 1.0,
                },
                {
                    "name" : "LoginButton",
                    "type" : "button",

                    "x" : 15,
                    "y" : 65,

                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",

                    "text" : uiScriptLocale.LOGIN_CONNECT,
                },
                {
                    "name" : "LoginExitButton",
                    "type" : "button",

                    "x" : 105,
                    "y" : 65,

                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",

                    "text" : uiScriptLocale.LOGIN_EXIT,
                },
            ),
        },

        {
            "name" : "BoardSaveAccount",
            "type" : "thinboard",

            "x" : (SCREEN_WIDTH - 110) / 2,
            "y" : (SCREEN_HEIGHT - 410 - 35),
            'width' : 337,
            "height" : 70+62,

            "children":

            (

                {
                    "name" : "Title",
                    "type" : "text",

                    "x" : 0,
                    "y" : 5,
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text" : localeInfo.SAVED_ACCOUNT,
                },

                ##Account 1
                {
                    "name" : "save_1_input_img","type" : "image","x": 7,"y": 15+13,"image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
                    "children":
                    ({
                        "name" : "save_1_input_text", "type" : "text", "x" :68, "y" : 3, "text_horizontal_align":"center", "text" : "----",
                    },),
                },

                {
                    "name" : "button_save_1","type" : "button",
                    "x" : 150,"y" : 14+13,"text" : localeInfo.SAVE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_login_1","type" : "button",
                    "x" : 150,"y" : 14+13,"text" : localeInfo.CONNECT_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_delete_1","type" : "button",
                    "x" : 150+90,"y" : 14+13,"text" : localeInfo.DELETE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                ##Account 2
                {
                    "name" : "save_2_input_img","type" : "image","x": 7,"y": 15+13+25,"image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
                    "children":
                    ({
                        "name" : "save_2_input_text", "type" : "text", "x" :68, "y" : 3, "text_horizontal_align":"center", "text" : "----",
                    },),
                },

                {
                    "name" : "button_save_2","type" : "button",
                    "x" : 150,"y" : 14+13+25,"text" : localeInfo.SAVE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_login_2","type" : "button",
                    "x" : 150,"y" : 14+13+25,"text" : localeInfo.CONNECT_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_delete_2","type" : "button",
                    "x" : 150+90,"y" : 14+13+25,"text" : localeInfo.DELETE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },


                ##Account 3
                {
                    "name" : "save_3_input_img","type" : "image","x": 7,"y": 15+13+(25*2),"image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
                    "children":
                    ({
                        "name" : "save_3_input_text", "type" : "text", "x" :68, "y" : 3, "text_horizontal_align":"center", "text" : "----",
                    },),
                },

                {
                    "name" : "button_save_3","type" : "button",
                    "x" : 150,"y" : 14+13+(25*2),"text" : localeInfo.SAVE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_login_3","type" : "button",
                    "x" : 150,"y" : 14+13+(25*2),"text" : localeInfo.CONNECT_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },


                {
                    "name" : "button_delete_3","type" : "button",
                    "x" : 150+90,"y" : 14+13+(25*2),"text" : localeInfo.DELETE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },


                ##Account 4
                {
                    "name" : "save_4_input_img","type" : "image","x": 7,"y": 15+13+(25*3),"image" : "d:/ymir work/ui/public/parameter_slot_05.sub",
                    "children":
                    ({
                        "name" : "save_4_input_text", "type" : "text", "x" :68, "y" : 3, "text_horizontal_align":"center", "text" : "----",
                    },),
                },

                {
                    "name" : "button_save_4","type" : "button",
                    "x" : 150,"y" : 14+13+(25*3),"text" : localeInfo.SAVE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_login_4","type" : "button",
                    "x" : 150,"y" : 14+13+(25*3),"text" : localeInfo.CONNECT_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

                {
                    "name" : "button_delete_4","type" : "button",
                    "x" : 150+90,"y" : 14+13+(25*3),"text" : localeInfo.DELETE_ACCOUNT,
                    "default_image" : ROOT_PATH + "Large_Button_01.sub","over_image" : ROOT_PATH + "Large_Button_02.sub","down_image" : ROOT_PATH + "Large_Button_03.sub",
                },

            ),
        },

        {
            "name" : "BoardLenguaje",
            "type" : "thinboard",
            # 'x' : (SCREEN_WIDTH - 564) / 2,
            # 'y' : SCREEN_HEIGHT - 300,
            # 'width' : 564,
            # 'height' : 254,
            "x" : (SCREEN_WIDTH - 565) / 2,
            "y" : SCREEN_HEIGHT - 300,
            "width" : 565,
            "height" : 60,

            "children":
            (
                ## Title
                {
                    "name" : "Title",
                    "type" : "text",

                    "x" : 0,
                    "y" : 12-8,
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    # "text" : uiScriptLocale.LENGUAJE_SELECT_TITLE,
                },
                {
                    "name" : "Lenguaje_en","type":"button",
                    "x" : 31-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/en_0.tga",
                    "over_image": "d:/ymir work/ui/flags/en_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/en_2.tga",
                },
                {
                    "name" : "Lenguaje_es","type":"button",
                    "x" : 31+50-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/es_0.tga",
                    "over_image": "d:/ymir work/ui/flags/es_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/es_2.tga",
                },
                {
                    "name" : "Lenguaje_de","type":"button",
                    "x" : 31+100-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/de_0.tga",
                    "over_image": "d:/ymir work/ui/flags/de_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/de_2.tga",
                },
                {
                    "name" : "Lenguaje_fr","type":"button",
                    "x" : 31+150-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/fr_0.tga",
                    "over_image": "d:/ymir work/ui/flags/fr_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/fr_2.tga",
                },
                {
                    "name" : "Lenguaje_pt","type":"button",
                    "x" : 31+200-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/pt_0.tga",
                    "over_image": "d:/ymir work/ui/flags/pt_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/pt_2.tga",
                },
                {
                    "name" : "Lenguaje_ro","type":"button",
                    "x" : 31+250-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/ro_0.tga",
                    "over_image": "d:/ymir work/ui/flags/ro_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/ro_2.tga",
                },
                {
                    "name" : "Lenguaje_it","type":"button",
                    "x" : 31+300-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/it_0.tga",
                    "over_image": "d:/ymir work/ui/flags/it_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/it_2.tga",
                },
                {
                    "name" : "Lenguaje_pl","type":"button",
                    "x" : 31+350-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/pl_0.tga",
                    "over_image": "d:/ymir work/ui/flags/pl_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/pl_2.tga",
                },
                {
                    "name" : "Lenguaje_tr","type":"button",
                    "x" : 31+400-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/tr_0.tga",
                    "over_image": "d:/ymir work/ui/flags/tr_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/tr_2.tga",
                },
                {
                    "name" : "Lenguaje_cz","type":"button",
                    "x" : 31+450-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/cz_0.tga",
                    "over_image": "d:/ymir work/ui/flags/cz_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/cz_2.tga",
                },
                {
                    "name" : "Lenguaje_hu","type":"button",
                    "x" : 31+500-15,"y" : -4+15+3,
                    "default_image" : "d:/ymir work/ui/flags/hu_0.tga",
                    "over_image": "d:/ymir work/ui/flags/hu_1.tga",
                    "down_image" : "d:/ymir work/ui/flags/hu_2.tga",
                },
            ),
        },


        ## ServerBoard
        {
            "name" : "ServerBoard",
            "type" : "thinboard",

            "x" : 0,
            "y" : SCREEN_HEIGHT - SERVER_BOARD_HEIGHT - 72,
            "width" : 375,
            "height" : SERVER_BOARD_HEIGHT,
            "horizontal_align" : "center",

            "children" :
            (

                ## Title
                {
                    "name" : "Title",
                    "type" : "text",

                    "x" : 0,
                    "y" : 12,
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    "text" : uiScriptLocale.LOGIN_SELECT_TITLE,
                },

                ## Horizontal
                {
                    "name" : "HorizontalLine1",
                    "type" : "line",

                    "x" : 10,
                    "y" : 34,
                    "width" : 354,
                    "height" : 0,
                    "color" : 0xff777777,
                },
                {
                    "name" : "HorizontalLine2",
                    "type" : "line",

                    "x" : 10,
                    "y" : 35,
                    "width" : 355,
                    "height" : 0,
                    "color" : 0xff111111,
                },

                ## Vertical
                {
                    "name" : "VerticalLine1",
                    "type" : "line",

                    "x" : 246,
                    "y" : 38,
                    "width" : 0,
                    "height" : SERVER_LIST_HEIGHT + 4,
                    "color" : 0xff777777,
                },
                {
                    "name" : "VerticalLine2",
                    "type" : "line",

                    "x" : 247,
                    "y" : 38,
                    "width" : 0,
                    "height" : SERVER_LIST_HEIGHT + 4,
                    "color" : 0xff111111,
                },

                ## ListBox
                {
                    "name" : "ServerList",
                    "type" : "listbox2",

                    "x" : 10,
                    "y" : 40,
                    "width" : 232,
                    "height" : SERVER_LIST_HEIGHT,
                    "row_count" : 15,
                    "item_align" : 0,
                },
                {
                    "name" : "ChannelList",
                    "type" : "listbox",

                    "x" : 255,
                    "y" : 40,
                    "width" : 109,
                    "height" : SERVER_LIST_HEIGHT,

                    "item_align" : 0,
                },

                ## Buttons
                {
                    "name" : "ServerSelectButton",
                    "type" : "button",

                    "x" : 267,
                    "y" : SERVER_LIST_HEIGHT,

                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",

                    "text" : uiScriptLocale.OK,
                },
                {
                    "name" : "ServerExitButton",
                    "type" : "button",

                    "x" : 267,
                    "y" : SERVER_LIST_HEIGHT + 22,

                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",

                    "text" : uiScriptLocale.LOGIN_SELECT_EXIT,
                },

            ),

        },

    ),
}
