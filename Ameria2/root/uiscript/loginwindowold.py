# coding: latin_1

ROOT_PATH = "d:/ymir work/ui/game/login/"
BG_PATH = ROOT_PATH
FONT_COLOR = 0xffdaf1ff
SEPARATOR_COLOR = 0xff1a2f61

SCALE_X = 1.0
SCALE_Y = 1.0

if SCREEN_WIDTH < 1200:
    ROOT_PATH += "small_"
    SCALE_X = 0.7
    SCALE_Y = 0.7


window = {
    "style" : ("movable", "float", "animate",),
    "x": 0, "y": 0,
    "width": SCREEN_WIDTH,
    "height": SCREEN_HEIGHT,
    "children":
        (
            {
                "name": "background",
                "type": "expanded_image",
                "x": 0, "y": 0,
                "x_scale": float(SCREEN_WIDTH) / 1920.0,
                "y_scale": float(SCREEN_HEIGHT) / 1080.0,
                "image": BG_PATH + "background.sub",
            },
            {
                "name": "logo",
                "type": "expanded_image",

                "x": 0,
                "y": 83 * float(SCREEN_HEIGHT) / 1080.0,

                "horizontal_align": "center",

                "x_scale": float(SCREEN_WIDTH) / 1920.0,
                "y_scale": float(SCREEN_HEIGHT) / 1080.0,

                "image": BG_PATH + "logo.sub",
            },
            {
                "name": "board_main",
                "type": "window",

                "x": -7 * SCALE_X,
                "y": 80 * float(SCREEN_HEIGHT) / 1080.0,

                "horizontal_align": "center",
                "vertical_align": "center",

                "width": 1247 * SCALE_X,
                "height": 591 * SCALE_Y,

                "children":
                    (
                        {
                            "name": "channel_board",
                            "type": "image",

                            "x": 0,
                            "y": 122 * SCALE_Y,

                            "image": ROOT_PATH + "channel_board.sub",

                            "children":
                                (
                                    {
                                        "name": "channel_header",
                                        "type": "image",

                                        "x": 0,
                                        "y": -42 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "channel_header.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "channel_header_text",
                                                    "type": "image",

                                                    "x": 0,
                                                    "y": 56 * SCALE_Y,

                                                    "horizontal_align": "center",

                                                    "image": ROOT_PATH + "channel_header_text.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "channel_button_01",
                                        "type": "radio_button",

                                        "x": 0,
                                        "y": 49 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "channel1_button_01.sub",
                                        "over_image": ROOT_PATH + "channel1_button_02.sub",
                                        "down_image": ROOT_PATH + "channel1_button_03.sub",
                                    },
                                    {
                                        "name": "channel_button_02",
                                        "type": "radio_button",

                                        "x": 0,
                                        "y": 112 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "channel2_button_01.sub",
                                        "over_image": ROOT_PATH + "channel2_button_02.sub",
                                        "down_image": ROOT_PATH + "channel2_button_03.sub",
                                    },
                                    {
                                        "name": "channel_button_03",
                                        "type": "radio_button",

                                        "x": 0,
                                        "y": 174 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "channel3_button_01.sub",
                                        "over_image": ROOT_PATH + "channel3_button_02.sub",
                                        "down_image": ROOT_PATH + "channel3_button_03.sub",
                                    },
                                    {
                                        "name": "channel_button_04",
                                        "type": "radio_button",

                                        "x": 0,
                                        "y": 236 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "channel4_button_01.sub",
                                        "over_image": ROOT_PATH + "channel4_button_02.sub",
                                        "down_image": ROOT_PATH + "channel4_button_03.sub",
                                    },
                                    {
                                        "name": "separator_01",
                                        "type": "bar",

                                        "x": 0,
                                        "y": 106 * SCALE_Y,

                                        "width": 280 * SCALE_X,
                                        "height": 1 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "color": SEPARATOR_COLOR,
                                    },
                                    {
                                        "name": "separator_02",
                                        "type": "bar",

                                        "x": 0,
                                        "y": 169 * SCALE_Y,

                                        "width": 280 * SCALE_X,
                                        "height": 1 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "color": SEPARATOR_COLOR,
                                    },
                                    {
                                        "name": "separator_03",
                                        "type": "bar",

                                        "x": 0,
                                        "y": 231 * SCALE_Y,

                                        "width": 280 * SCALE_X,
                                        "height": 1 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "color": SEPARATOR_COLOR,
                                    },
                                ),
                        },
                                    # {
                                        # "name": "server1_button",
                                        # "type": "radio_button",

                                        # "x": (-388) * SCALE_X,
                                        # "y": 430 * SCALE_Y,

                                        # "horizontal_align": "center",

                                        # "default_image": ROOT_PATH + "3.png",
                                        # "over_image": ROOT_PATH + "1.png",
                                        # "down_image": ROOT_PATH + "2.png",
                                    # },
                        {
                            "name": "account_board",
                            "type": "image",

                            "x": 495 * SCALE_X,
                            "y": 122 * SCALE_Y,

                            "horizontal_align": "right",

                            "image": ROOT_PATH + "account_board.sub",

                            "children":
                                (
                                    {
                                        "name": "account_header",
                                        "type": "image",

                                        "x": 25 * SCALE_X,
                                        "y": -42 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "account_header.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "account_header_text",
                                                    "type": "image",

                                                    "x": 0,
                                                    "y": 56 * SCALE_Y,

                                                    "horizontal_align": "center",

                                                    "image": ROOT_PATH + "account_header_text.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "account_slot_01",
                                        "type": "image",

                                        "x": 25 * SCALE_X,
                                        "y": 49 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "account_slot.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "account_value_01",
                                                    "type": "text",

                                                    "x": 60 * SCALE_X,
                                                    "y": 20 * SCALE_Y ** 2,

                                                    "color": FONT_COLOR,

                                                    "text": "FilipDev.com",
                                                },
                                                {
                                                    "name": "account_delete_button_01",
                                                    "type": "button",

                                                    "x": 274 * SCALE_X,
                                                    "y": 25 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_delete_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_delete_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_delete_button_03.sub",
                                                },
                                                {
                                                    "name": "account_load_button_01",
                                                    "type": "button",

                                                    "x": 255 * SCALE_X,
                                                    "y": 24 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_load_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_load_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_load_button_03.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "account_slot_02",
                                        "type": "image",

                                        "x": 25 * SCALE_X,
                                        "y": 112 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "account_slot.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "account_value_02",
                                                    "type": "text",

                                                    "x": 60 * SCALE_X,
                                                    "y": 20 * SCALE_Y ** 2,

                                                    "color": FONT_COLOR,

                                                    "text": "FilipDev.com",
                                                },
                                                {
                                                    "name": "account_delete_button_02",
                                                    "type": "button",

                                                    "x": 274 * SCALE_X,
                                                    "y": 25 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_delete_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_delete_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_delete_button_03.sub",
                                                },
                                                {
                                                    "name": "account_load_button_02",
                                                    "type": "button",

                                                    "x": 255 * SCALE_X,
                                                    "y": 24 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_load_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_load_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_load_button_03.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "account_slot_03",
                                        "type": "image",

                                        "x": 25 * SCALE_X,
                                        "y": 174 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "account_slot.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "account_value_03",
                                                    "type": "text",

                                                    "x": 60 * SCALE_X,
                                                    "y": 20 * SCALE_Y ** 2,

                                                    "color": FONT_COLOR,

                                                    "text": "FilipDev.com",
                                                },
                                                {
                                                    "name": "account_delete_button_03",
                                                    "type": "button",

                                                    "x": 274 * SCALE_X,
                                                    "y": 25 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_delete_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_delete_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_delete_button_03.sub",
                                                },
                                                {
                                                    "name": "account_load_button_03",
                                                    "type": "button",

                                                    "x": 255 * SCALE_X,
                                                    "y": 24 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_load_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_load_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_load_button_03.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "account_slot_04",
                                        "type": "image",

                                        "x": 25 * SCALE_X,
                                        "y": 236 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "account_slot.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "account_value_04",
                                                    "type": "text",

                                                    "x": 60 * SCALE_X,
                                                    "y": 20 * SCALE_Y ** 2,

                                                    "color": FONT_COLOR,

                                                    "text": "FilipDev.com",
                                                },
                                                {
                                                    "name": "account_delete_button_04",
                                                    "type": "button",

                                                    "x": 274 * SCALE_X,
                                                    "y": 25 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_delete_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_delete_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_delete_button_03.sub",
                                                },
                                                {
                                                    "name": "account_load_button_04",
                                                    "type": "button",

                                                    "x": 255 * SCALE_X,
                                                    "y": 24 * SCALE_Y,

                                                    "default_image": ROOT_PATH + "account_load_button_01.sub",
                                                    "over_image": ROOT_PATH + "account_load_button_02.sub",
                                                    "down_image": ROOT_PATH + "account_load_button_03.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "separator_04",
                                        "type": "bar",

                                        "x": 25 * SCALE_X,
                                        "y": 106 * SCALE_Y,

                                        "width": 280 * SCALE_X,
                                        "height": 1 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "color": SEPARATOR_COLOR,
                                    },
                                    {
                                        "name": "separator_05",
                                        "type": "bar",

                                        "x": 25 * SCALE_X,
                                        "y": 169 * SCALE_Y,

                                        "width": 280 * SCALE_X,
                                        "height": 1 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "color": SEPARATOR_COLOR,
                                    },
                                    {
                                        "name": "separator_06",
                                        "type": "bar",

                                        "x": 25 * SCALE_X,
                                        "y": 231 * SCALE_Y,

                                        "width": 280 * SCALE_X,
                                        "height": 1 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "color": SEPARATOR_COLOR,
                                    },
                                ),
                        },
                                    # {
                                        # "name": "server2_button",
                                        # "type": "radio_button",

                                        # "x": (403) * SCALE_X,
                                        # "y": 430 * SCALE_Y,

                                        # "horizontal_align": "center",

                                        # "default_image": ROOT_PATH + "33.png",
                                        # "over_image": ROOT_PATH + "11.png",
                                        # "down_image": ROOT_PATH + "22.png",
                                    # },
                        {
                            "name": "login_board",
                            "type": "image",

                            "x": 0,
                            "y": 42 * SCALE_Y,

                            "horizontal_align": "center",

                            "image": ROOT_PATH + "login_board.sub",

                            "children":
                                (
                                    {
                                        "name": "login_header",
                                        "type": "image",

                                        "x": 7 * SCALE_X,
                                        "y": -42 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "login_header.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "login_header_text",
                                                    "type": "image",

                                                    "x": 0,
                                                    "y": 56 * SCALE_Y,

                                                    "horizontal_align": "center",

                                                    "image": ROOT_PATH + "login_header_text.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "login_text",
                                        "type": "image",

                                        "x": 7 * SCALE_X,
                                        "y": 63 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "login_text.sub",
                                    },
                                    {
                                        "name": "id_slotbar",
                                        "type": "image",

                                        "x": 7 * SCALE_X,
                                        "y": 89 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "input_slot_login.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "id",
                                                    "type": "editline",

                                                    "x": 70 * SCALE_X,
                                                    "y": 20 * SCALE_Y ** 2,

                                                    "width": 200 * SCALE_X,
                                                    "height": 16 * SCALE_Y,

                                                    "color": FONT_COLOR,
                                                    "input_limit": 16,
                                                },
                                            ),
                                    },
                                    {
                                        "name": "pwd_slotbar",
                                        "type": "image",

                                        "x": 7 * SCALE_X,
                                        "y": 139 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "image": ROOT_PATH + "input_slot_login.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "pwd",
                                                    "type": "editline",

                                                    "x": 70 * SCALE_X,
                                                    "y": 20 * SCALE_Y ** 2,

                                                    "width": 200 * SCALE_X,
                                                    "height": 16 * SCALE_Y,

                                                    "color": FONT_COLOR,

                                                    "input_limit": 16,
                                                    "secret_flag": 1,
                                                },
                                            ),
                                    },

                                    {
                                        "name": "login_button",
                                        "type": "button",

                                        "x": 7 * SCALE_X,
                                        "y": 237 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "login_button_01.sub",
                                        "over_image": ROOT_PATH + "login_button_02.sub",
                                        "down_image": ROOT_PATH + "login_button_03.sub",
                                    },

                                    {
                                        "name": "create_account_button",
                                        "type": "button",

                                        "x": 0 * SCALE_X,
                                        "y": 204 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "create_account_button_01.sub",
                                        "over_image": ROOT_PATH + "create_account_button_02.sub",
                                        "down_image": ROOT_PATH + "create_account_button_03.sub",
                                    },
                                    {
                                        "name": "language_button",
                                        "type": "button",

                                        "x": 0 * SCALE_X,
                                        "y": 299 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "language_button_01.sub",
                                        "over_image": ROOT_PATH + "language_button_02.sub",
                                        "down_image": ROOT_PATH + "language_button_03.sub",

                                        "children":
                                            (
                                                {
                                                    "name": "language_image",
                                                    "type": "image",

                                                    "x": 31 * SCALE_X,
                                                    "y": 24 * SCALE_Y,

                                                    "image": ROOT_PATH + "eu_button_01.sub",
                                                },
                                            ),
                                    },
                                    {
                                        "name": "discord_button",
                                        "type": "button",

                                        "x": -71 * SCALE_X,
                                        "y": 299 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "discord_button_01.sub",
                                        "over_image": ROOT_PATH + "discord_button_02.sub",
                                        "down_image": ROOT_PATH + "discord_button_03.sub",
                                    },
                                    {
                                        "name": "homepage_button",
                                        "type": "button",

                                        "x": 71 * SCALE_X,
                                        "y": 299 * SCALE_Y,

                                        "horizontal_align": "center",

                                        "default_image": ROOT_PATH + "homepage_button_01.sub",
                                        "over_image": ROOT_PATH + "homepage_button_02.sub",
                                        "down_image": ROOT_PATH + "homepage_button_03.sub",
                                    },
                                ),
                        },
                    ),
            },
            {
                "name": "language_board",
                "type": "image",

                "x": 0,
                "y": 170 * ((float(SCREEN_HEIGHT) / 1080.0) ** 2),

                "horizontal_align": "center",
                "vertical_align": "bottom",

                "image": ROOT_PATH + "language_board.sub",

                "children":
                    (
                        {
                            "name": "language_eu_button",
                            "type": "button",

                            "x": 24 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "English",

                            "default_image": ROOT_PATH + "uk_button_01.sub",
                            "over_image": ROOT_PATH + "uk_button_02.sub",
                            "down_image": ROOT_PATH + "uk_button_03.sub",
                        },
                        # {
                            # "name": "language_us_button",
                            # "type": "button",

                            # "x": 51 * SCALE_X,
                            # "y": 0,

                            # "vertical_align": "center",
                            # "tooltip_text": "English (US)",

                            # "default_image": ROOT_PATH + "us_button_01.sub",
                            # "over_image": ROOT_PATH + "us_button_02.sub",
                            # "down_image": ROOT_PATH + "us_button_03.sub",
                        # },
                        # {
                            # "name": "language_fr_button",
                            # "type": "button",

                            # "x": 78 * SCALE_X,
                            # "y": 0,

                            # "vertical_align": "center",
                            # "tooltip_text": "Francais",

                            # "default_image": ROOT_PATH + "fr_button_01.sub",
                            # "over_image": ROOT_PATH + "fr_button_02.sub",
                            # "down_image": ROOT_PATH + "fr_button_03.sub",
                        # },
                        {
                            "name": "language_de_button",
                            "type": "button",

                            "x": 51 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Deutsch",

                            "default_image": ROOT_PATH + "de_button_01.sub",
                            "over_image": ROOT_PATH + "de_button_02.sub",
                            "down_image": ROOT_PATH + "de_button_03.sub",
                        },
                        # {
                            # "name": "language_cn_button",
                            # "type": "button",

                            # "x": 132 * SCALE_X,
                            # "y": 0,

                            # "vertical_align": "center",
                            # "tooltip_text": "Zhongguo ren",

                            # "default_image": ROOT_PATH + "cn_button_01.sub",
                            # "over_image": ROOT_PATH + "cn_button_02.sub",
                            # "down_image": ROOT_PATH + "cn_button_03.sub",
                        # },
                        # {
                            # "name": "language_at_button",
                            # "type": "button",

                            # "x": 159 * SCALE_X,
                            # "y": 0,

                            # "vertical_align": "center",
                            # "tooltip_text": "Deutsch",

                            # "default_image": ROOT_PATH + "at_button_01.sub",
                            # "over_image": ROOT_PATH + "at_button_02.sub",
                            # "down_image": ROOT_PATH + "at_button_03.sub",
                        # },
                        {
                            "name": "language_it_button",
                            "type": "button",

                            "x": 78 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Italiano",

                            "default_image": ROOT_PATH + "it_button_01.sub",
                            "over_image": ROOT_PATH + "it_button_02.sub",
                            "down_image": ROOT_PATH + "it_button_03.sub",
                        },
                        # {
                            # "name": "language_eu_button",
                            # "type": "button",

                            # "x": 213 * SCALE_X,
                            # "y": 0,

                            # "vertical_align": "center",
                            # "tooltip_text": "English",

                            # "default_image": ROOT_PATH + "eu_button_01.sub",
                            # "over_image": ROOT_PATH + "eu_button_02.sub",
                            # "down_image": ROOT_PATH + "eu_button_03.sub",
                        # },
                        {
                            "name": "language_tr_button",
                            "type": "button",

                            "x": 159 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Turkce",

                            "default_image": ROOT_PATH + "tr_button_01.sub",
                            "over_image": ROOT_PATH + "tr_button_02.sub",
                            "down_image": ROOT_PATH + "tr_button_03.sub",
                        },
                        {
                            "name": "language_ro_button",
                            "type": "button",

                            "x": 132 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Romana",

                            "default_image": ROOT_PATH + "ro_button_01.sub",
                            "over_image": ROOT_PATH + "ro_button_02.sub",
                            "down_image": ROOT_PATH + "ro_button_03.sub",
                        },
                        {
                            "name": "language_pt_button",
                            "type": "button",

                            "x": 186 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Portugues",

                            "default_image": ROOT_PATH + "pt_button_01.sub",
                            "over_image": ROOT_PATH + "pt_button_02.sub",
                            "down_image": ROOT_PATH + "pt_button_03.sub",
                        },
                        {
                            "name": "language_pl_button",
                            "type": "button",

                            "x": 213 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Polski",

                            "default_image": ROOT_PATH + "pl_button_01.sub",
                            "over_image": ROOT_PATH + "pl_button_02.sub",
                            "down_image": ROOT_PATH + "pl_button_03.sub",
                        },
                        {
                            "name": "language_es_button",
                            "type": "button",

                            "x": 105 * SCALE_X,
                            "y": 0,

                            "vertical_align": "center",
                            "tooltip_text": "Espanol",

                            "default_image": ROOT_PATH + "es_button_01.sub",
                            "over_image": ROOT_PATH + "es_button_02.sub",
                            "down_image": ROOT_PATH + "es_button_03.sub",
                        },
                    ),
            },
        ),
}
