# coding: utf-8

import uiScriptLocale

window = {
	"name" : "PickChequeDialogNew_Window",
	"x" : 0,
	"y" : 0,
	"style" : ("movable", "float", "animate",),
	"width" : 152,
	"height" : 90,
	"children" :
	(
		{
			"name" : "board_ui",
			"type" : "board_with_titlebar",
			"x" : 0,
			"y" : 0,
			"width" : 152,
			"height" : 90,
			"title" : "",
			"children" :
			(
				{
					"name" : "input_slot_ui",
					"type" : "image",
					"x" : 20,
					"y" : 34,
					"image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
					"x_scale" : 0.8,
					"children" :
					(
						{
							"name" : "input_value_ui",
							"type" : "editline",
							"x" : 3,
							"y" : 2,
							"width" : 50,
							"height" : 18,
							"input_limit" : 4,
							"text" : "0",
						},
						{
							"name" : "max_value_text_ui",
							"type" : "text",
							"x" : 53,
							"y" : 2,
							"text" : "/ 0",
						},
					),
				},
				{
					"name" : "accept_button_ui",
					"type" : "button",
					"x" : 152 / 2 - 61 - 5,
					"y" : 58,
					"text" : uiScriptLocale.OK,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button_ui",
					"type" : "button",
					"x" : 152 / 2 + 5,
					"y" : 58,
					"text" : uiScriptLocale.CANCEL,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}
