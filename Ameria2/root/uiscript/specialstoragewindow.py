# coding: latin_1

import uiScriptLocale

window = {
    "name" : "SpecialStorageWindow",

    "x" : SCREEN_WIDTH - 360,
    "y" : 88,

    "style" : ("movable", "float", "animate",),

    "width" : 184,
    "height" : 328+32+48,

    "children" :
    (
        {
            "name" : "board",
            "type" : "board",
            "style" : ("attach",),

            "x" : 0,
            "y" : 0,

            "width" : 184,
            "height" : 328+32+48,

            "children" :
            (
                # Separate
                {
                    "name" : "SeparateBaseImage",
                    "type" : "image",
                    "style" : ("attach",),

                    "x" : 8,
                    "y" : 7,

                    "image" : "d:/ymir work/ui/pattern/titlebar_inv_refresh_baseframe.tga",

                    "children" :
                    (
                        # Separate Button (38x24)
                        {
                            "name" : "SeparateButton",
                            "type" : "button",

                            "x" : 11,
                            "y" : 3,

                            "tooltip_text" : uiScriptLocale.INVENTORY_SEPARATE,

                            "default_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_01.sub",
                            "over_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_02.sub",
                            "down_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_03.sub",
                            "disable_image" : "d:/ymir work/ui/game/inventory/refresh_small_button_04.sub",
                        },
                    ),
                },
                {
                    "name" : "TitleBar",
                    "type" : "titlebar",
                    "style" : ("attach",),

                    "x" : 48,
                    "y" : 7,

                    "width" : 175-48,
                    "color" : "yellow",

                    "children" :
                    (
                        { "name":"TitleName", "type":"text", "x":0, "y":3, "text":uiScriptLocale.UPGRADE_STORAGE_TITLE, "text_horizontal_align":"center", "horizontal_align":"center"},
                    ),
                },
                {
                    "name" : "ItemSlot",
                    "type" : "grid_table",

                    "x" : 12,
                    "y" : 34,

                    "start_index" : 0,
                    "x_count" : 5,
                    "y_count" : 9,
                    "x_step" : 32,
                    "y_step" : 32,

                    "image" : "d:/ymir work/ui/public/Slot_Base.sub",
                },
                {
                    "name" : "Inventory_Tab_01",
                    "type" : "radio_button",

                    "x" : 14,
                    "y" : 295+32,

                    "default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
                    "over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
                    "down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
                    "children" :
                    (
                        {
                            "name" : "Inventory_Tab_01_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "I",
                        },
                    ),
                },
                {
                    "name" : "Inventory_Tab_02",
                    "type" : "radio_button",

                    "x" : 14 + 39,
                    "y" : 295+32,

                    "default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
                    "over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
                    "down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

                    "children" :
                    (
                        {
                            "name" : "Inventory_Tab_02_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "II",
                        },
                    ),
                },
                {
                    "name" : "Inventory_Tab_03",
                    "type" : "radio_button",

                    "x" : 14 + 39+39,
                    "y" : 295+32,

                    "default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
                    "over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
                    "down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

                    "children" :
                    (
                        {
                            "name" : "Inventory_Tab_02_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "III",
                        },
                    ),
                },
                {
                    "name" : "Inventory_Tab_04",
                    "type" : "radio_button",

                    "x" : 14 + 39+39+39,
                    "y" : 295+32,

                    "default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
                    "over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
                    "down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

                    "children" :
                    (
                        {
                            "name" : "Inventory_Tab_02_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "IV",
                        },
                    ),
                },
                {
                    "name" : "Category_Tab_01",
                    "type" : "radio_button",

                    "x" : 26,
                    "y" : 295+32+30,

                    "default_image" : "flag/items_1.tga",
                    "over_image" : "flag/items_2.tga",
                    "down_image" : "flag/items_3.tga",

                    "children" :
                    (
                        {
                            "name" : "Category_Tab_01_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "",
                        },
                    ),
                },
                {
                    "name" : "Category_Tab_02",
                    "type" : "radio_button",

                    "x" : 26+50,
                    "y" : 295+32+30,

                    "default_image" : "flag/book_1.tga",
                    "over_image" : "flag/book_2.tga",
                    "down_image" : "flag/book_3.tga",

                    "children" :
                    (
                        {
                            "name" : "Category_Tab_02_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "",
                        },
                    ),
                },
                {
                    "name" : "Category_Tab_03",
                    "type" : "radio_button",

                    "x" : 26+50+50,
                    "y" : 295+32+30,

                    "default_image" : "flag/stone_1.tga",
                    "over_image" : "flag/stone_2.tga",
                    "down_image" : "flag/stone_3.tga",

                    "children" :
                    (
                        {
                            "name" : "Category_Tab_03_Print",
                            "type" : "text",

                            "x" : 0,
                            "y" : 0,

                            "all_align" : "center",

                            "text" : "",
                        },
                    ),
                },
            ),
        },
    ),
}