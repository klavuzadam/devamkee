# coding: utf-8

import uiScriptLocale

BOARD_WIDTH = 215
BOARD_HEIGHT = 270

window = {
	"name" : "Shard_CraftWindow",
	"x" : (SCREEN_WIDTH - BOARD_WIDTH) / 2,
	"y" : (SCREEN_HEIGHT - BOARD_HEIGHT) / 2,
	"style" : ("movable", "float", "animate",),
	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,
	"children" :
	(
		{
			"name" : "board_ui",
			"type" : "board",
			"style" : ("attach",),
			"x" : 0,
			"y" : 0,
			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,
			"children" :
			(
				{
					"name" : "titlebar_ui",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 6,
					"y" : 6,
					"width" : 200,
					"color" : "yellow",
					"children" :
					(
						{
							"name" : "titlename_ui",
							"type" : "text",
							"x" : 95,
							"y" : 3,
							"text" : uiScriptLocale.UI_SHARD_CRAFT_TITLE,
							"text_horizontal_align" : "center"
						},
					),
				},
				{
					"name" : "image_ui_bg",
					"type" : "image",
					"x" : 9,
					"y" : 35,
					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "acce/acce_combine.tga",
					"children" :
					(
						{
							"name" : "slot_ui_materials",
							"type" : "slot",
							"x" : 3,
							"y" : 3,
							"width" : 200,
							"height" : 150,
							"slot" : (
										{
											"index" : SHARD_CRAFT_SLOT_MAIN,
											"x" : 78,
											"y" : 7,
											"width" : 32,
											"height" : 32
										},
										{
											"index" : SHARD_CRAFT_SLOT_MATERIAL,
											"x" : 78,
											"y" : 60,
											"width" : 32,
											"height" : 32
										},
										{
											"index" : SHARD_CRAFT_SLOT_RESULT,
											"x" : 78,
											"y" : 115,
											"width" : 32,
											"height" : 32
										},
							),
						},
						{
							"name" : "text_ui_main",
							"type" : "text",
							"text" : uiScriptLocale.UI_SHARD_MAIN,
							"text_horizontal_align":"center",
							"x" : 85 + 12,
							"y" : 7 + 36,
						},
						{
							"name" : "text_ui_material",
							"type" : "text",
							"text" : uiScriptLocale.UI_SHARD_MATERIAL,
							"text_horizontal_align" : "center",
							"x" : 85 + 12,
							"y" : 60 + 38,
						},
						{
							"name" : "text_ui_result",
							"type" : "text", 
							"text" : uiScriptLocale.UI_SHARD_RESULT,
							"text_horizontal_align":"center",
							"x" : 85 + 12,
							"y" : 115 + 40
						},
					),
				},
				{
					"name" : "text_ui_price",
					"type" : "text",
					"text" : "",
					"text_horizontal_align" : "center",
					"x" : 105,
					"y" : 215,
				},
				{
					"name" : "button_ui_craft",
					"type" : "button",
					"x" : 40,
					"y" : 235,
					"text" : uiScriptLocale.OK,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "button_ui_close",
					"type" : "button",
					"x" : 114,
					"y" : 235,
					"text" : uiScriptLocale.CANCEL,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}
