# coding: latin_1

import localeInfo

IMG_DIR = "d:/ymir work/ui/game/mission/"
BUTTON_ROOT = "d:/ymir work/ui/public/"
window = {
    "name" : "MissionItem",
    "x" : 0,
    "y" : 0,
    "width" : 620,
    "height" : 85,
    "children" :
    (
        {
            "name": "board",
            "type": "expanded_image",
            "x": 0,
            "y": 0,
            "image":IMG_DIR+"item.tga",
            "children":
            (
                {
                    "name":"name",
                    "type":"text",
                    "style" : ("not_pick",),
                    "x": 35,
                    "y": 12,
                    "text_horizontal_align":"left",
                    "text":"",
                    "color":0xFFF6A800,
                },
                {
                    "name":"icon",
                    "type":"expanded_image",
                    "x": 5,
                    "y": 5,
                    "image": "icon/item/50309.tga",
                },

                {
                    "name":"status_text",
                    "type":"text",
                    "x": 8,
                    "y": 32,
                    "text_horizontal_align":"left",
                    "text":"",
                },
                {
                    "name":"type_text",
                    "type":"text",
                    "x": 8,
                    "y": 32 + 15,
                    "text_horizontal_align":"left",
                    "text":"",
                },
                {
                    "name":"time_text",
                    "type":"text",
                    "x": 8,
                    "y": 32 + 15 + 15,
                    "text_horizontal_align":"left",
                    "text":"",
                },
                {
                    "name" : "reward_btn",
                    "type" : "button",
                    "x" : 170,
                    "y" : 15,
                    "default_image" : IMG_DIR + "reward_btn_0.tga",
                    "over_image" : IMG_DIR + "reward_btn_1.tga",
                    "down_image" : IMG_DIR + "reward_btn_2.tga",
                    "disable_image" : IMG_DIR + "reward_btn_2.tga",
                },
                {
                    "name" : "delete_btn",
                    "type" : "button",
                    "x" : 170,
                    "y" : 15+30,
                    "default_image" : IMG_DIR + "delete_btn_0.tga",
                    "over_image" : IMG_DIR + "delete_btn_1.tga",
                    "down_image" : IMG_DIR + "delete_btn_2.tga",
                },
                {
                    "name":"reward_text",
                    "type":"text",
                    "x": 315,
                    "y": 15,
                    "text_horizontal_align":"center",
                    "text":localeInfo.MISSION_REWARD_TITLE,
                },
                {
                    "name":"reward_experience",
                    "type":"text",
                    "x": 315,
                    "y": 30,
                    "text_horizontal_align":"center",
                    "text":"",
                },
                {
                    "name":"gold_experience",
                    "type":"text",
                    "x": 315,
                    "y": 45,
                    "text_horizontal_align":"center",
                    "text":"",
                },
            ),
        },
    ),
}
