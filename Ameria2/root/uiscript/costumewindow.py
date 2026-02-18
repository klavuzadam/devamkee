# coding: latin_1

import item
import localeInfo
COSTUME_START_INDEX = item.COSTUME_SLOT_START
#SHINING_START_INDEX = item.SHINING_SLOT_START

WINDOW_SIZE = [8+165+20, 58 + 165 + 8]
IMG_WIDTH = 177
IMG_HEIGHT = 165
Y_EXTRA = 30
X_EXTRA = 7

window = {
        "name" : "CostumeWindow",

        "x" : SCREEN_WIDTH - 175 - (IMG_WIDTH+(X_EXTRA*2)) - 33,
        "y" : SCREEN_HEIGHT - 37 - 565 + 5,
        "style" : ("movable", "float",),
        "width" : WINDOW_SIZE[0],
        "height" : WINDOW_SIZE[1],
        "children" :
        (
            {
                "name" : "board",
                "type" : "board_with_titlebar",
                "style" : ("attach",),
                "x" : 0,
                "y" : 0,
                "width" : WINDOW_SIZE[0],
                "height" : WINDOW_SIZE[1],
                "title" : "Costume",
                "children" :
                (
                    {
                        "name" : "costumeBtn",
                        "type" : "radio_button",
                        "x" : 25,
                        "y" : 34,
                        "text" : "Costume",
                        "default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
                        "over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
                        "down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
                    },
                    {
                        "name" : "buffiBtn",
                        "type" : "radio_button",
                        "x" : 32 + 76,
                        "y" : 34,
                        "text" : "Buffi",
                        "default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
                        "over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
                        "down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
                    },
        {
                        "name" : "Costume_Page",
                        "type" : "image",
                        "x" : 8,
                        "y" : 58,
                        "image" : "d:/ymir work/ui/game/costume/new_costume_window.tga",
                        "children" :
                        (
                            {
                                "name" : "CostumeSlot",
                                "type" : "slot",
                                "x" : 0,"y" : 0,
                                "width" : 165,"height" : 165,
                                "slot" : (
                                    {"index":item.COSTUME_SLOT_BODY, "x":50, "y":62, "width":32, "height":64},
                                    {"index":item.COSTUME_SLOT_HAIR, "x":50, "y": 25, "width":32, "height":32},
                                    {"index":item.COSTUME_SLOT_WEAPON, "x":8, "y":11, "width":32, "height":96},
                                    {"index":item.EQUIPMENT_PET, "x":135, "y": 81, "width":32, "height":32},
                                    {"index":item.COSTUME_SLOT_MOUNT, "x":94, "y":45, "width":32, "height":32},
                                    {"index":item.SLOT_MOUNT_SKIN, "x":135, "y":45, "width":32, "height":32},
                                    {"index":item.EQUIPMENT_OLDPET, "x":94, "y":81, "width":32, "height":32},
                                    {"index":item.SKIN_SASH_SLOT_START, "x":135, "y":8, "width":32, "height":32},
                                    {"index":item.EQUIPMENT_ARROW, "x":94, "y":8, "width":32, "height":32},
                                    {"index":item.SHINING_SLOT_START, "x":94, "y":120, "width":32, "height":32},
                                    {"index":item.SHINING_SLOT_START+1, "x":135, "y":120, "width":32, "height":32},
                                    {"index":item.SHINING_SLOT_START+6, "x":169, "y":130, "width":32, "height":32},
                                ),
                            },
                        ),
                    },
                    {
                        "name" : "Buffi_Page",
                        "type" : "image",
                        "x" : 8,
                        "y" : 58,
                        "image" : "d:/ymir work/ui/game/costume/buffy_bg.tga",
                        "children" :
                        (
                            {
                                "name" : "BuffiSlot",
                                "type" : "slot",
                                "x" : 0,"y" : 0,
                                "width" : 165,"height" : 165,
                                "slot" : (
                                    # {"index":item.BUFFI_SLOT_BODY, "x":50, "y":62, "width":32, "height":64},
                                    # {"index":item.BUFFI_SLOT_HEAD, "x":50, "y": 25, "width":32, "height":32},
                                    # {"index":item.BUFFI_SLOT_WEAPON, "x":122, "y":222, "width":32, "height":32},
                                    # {"index":item.BUFFI_SLOT_SASH, "x":116, "y":11, "width":32, "height":32},

                                    #{"index":item.BUFFI_SLOT_SHININGWEAPON, "x":116, "y":47, "width":32, "height":32},
                                    #{"index":item.BUFFI_SLOT_SHININGARMOR, "x":116, "y":79, "width":32, "height":32},


                                    # {"index":item.BUFFI_SLOT_RING_0, "x":18, "y":124, "width":32, "height":32},
                                    # {"index":item.BUFFI_SLOT_RING_1, "x":64, "y":124, "width":32, "height":32},
                                    # {"index":item.BUFFI_SLOT_RING_2, "x":116, "y":124, "width":32, "height":32},

                                ),
                            },
                        ),
                    },
                ),
            },
        ),
    }
