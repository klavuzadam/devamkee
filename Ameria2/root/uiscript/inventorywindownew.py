# coding: latin_1

import uiScriptLocale
import app

RUTA_IMG_SPECIAL_INV = "buttons_inv/"
RUTA_IMG_NEW_INV = "rework_inv/"
NEW_INV_DESIGN = "d:/ymir work/ui/game/special_inventory/"
INVENTORY_PATH = "d:/ymir work/ui/game/interfaces/special_storage/"
WINDOW_WIDTH = 184
WINDOW_HEIGHT = 328+32+60+20

window = {
	"name" : "SpecialStorageWindow",

	"x" : SCREEN_WIDTH - 400,
	"y" : 10,

	"style" : ("movable", "float",),

	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 0,
			"y" : 0,

			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,

			"children" :
			(
			
				## Separate
				{
					"name" : "SeparateBaseImage",
					"type" : "image",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"image" : INVENTORY_PATH + "btn-settings-1.png",

					"children" :
					(
						## Separate Button (38x24)
						{
							"name" : "ManageInventory",
							"type" : "button",

							"x" : 0,
							"y" : 0,

							"default_image" : INVENTORY_PATH + "btn-settings-1.png",
							"over_image" : INVENTORY_PATH + "btn-settings-2.png",
							"down_image" : INVENTORY_PATH + "btn-settings-3.png",
							"disable_image" : INVENTORY_PATH + "btn-settings-3.png",
						},
					),
				},


				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8 + 38,
					"y" : 7,

					"width" : 161 - 33,

					"color" : "yellow",

					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",

							"x" : 77 - 20,
							"y" : 3,

							"text" : uiScriptLocale.INVENTORY_TITLE,
							"text_horizontal_align" : "center"
						},
					),
				},

				{
					"name" : "loadingImage",
					"type" : "expanded_image",
					"x" : 8+((32*5)/2),
					"y" : 33+((32*9)/2),
					"image" : "d:/ymir work/ui/load_.tga"
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 12,
					"y" : 35,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},
				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",
					"x" : 14,
					"y" : 295+32,
					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,
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
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,
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
					"x" : 14 + 39 + 39,
					"y" : 295+32,
					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_3,
					"children" :
					(
						{
							"name" : "Inventory_Tab_03_Print",
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
					"x" : 14 + 39 + 39 + 39,
					"y" : 295+32,
					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",
					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_4,
					"children" :
					(
						{
							"name" : "Inventory_Tab_04_Print",
							"type" : "text",
							"x" : 0,
							"y" : 0,
							"all_align" : "center",
							"text" : "IV",
						},
					),
				},
				{
					"name" : "CategoryBoard",
					"type" : "expanded_image",
					
					"x" : 12,
					"y" : 350,
					
					"image" : "d:/ymir work/ui/game/interfaces/special_storage/frame.png",
					
					"children" : 
					(
						{
							"name" : "Inventory_Special_1",
							"type" : "radio_button",

							"x" : 12 + (34* 0),
							"y" : 5,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_upgrade_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_upgrade_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_upgrade_4.png",
						},
						{
							"name" : "Inventory_Special_3",
							"type" : "radio_button",

							"x" : 12 + (34 * 1),
							"y" : 5,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_stone_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_stone_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_stone_4.png",
						},
						{
							"name" : "Inventory_Special_2",
							"type" : "radio_button",

							"x" : 12 + (34* 2),
							"y" : 5,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_book_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_book_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_book_4.png",
						},


						{
							"name" : "Inventory_Special_4",
							"type" : "radio_button",

							"x" : 12 + (34 * 3),
							"y" : 5,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_bonus_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_bonus_1.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_bonus_4.png",
						},

						{
							"name" : "Inventory_Special_5",
							"type" : "radio_button",

							"x" : 12 + (34* 0),
							"y" : 39,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_costume_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_costume_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_costume_4.png",
						},
						{
							"name" : "Inventory_Special_6",
							"type" : "radio_button",

							"x" : 12 + (34* 1),
							"y" : 39,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_guild_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_guild_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_guild_4.png",
						},

						{
							"name" : "Inventory_Special_7",
							"type" : "radio_button",

							"x" : 12 + (34* 2),
							"y" : 39,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_safebox_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_safebox_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_safebox_4.png",
						},
						{
							"name" : "Inventory_Special_8",
							"type" : "radio_button",

							"x" : 12 + (34* 3),
							"y" : 39,

							"default_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_ishop_1.png",
							"over_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_ishop_2.png",
							"down_image" : "d:/ymir work/ui/game/interfaces/special_storage/btn_ishop_4.png",
						},

					),
				},
			),
		},
	),
}

