# coding: latin_1

import uiScriptLocale

window = {
	"name" : "CurrencyExchangeDialog_Wnd",
	"x" : 0,
	"y" : 0,
	"style" : ("movable", "float", "animate",),
	"width" : 170,
	"height" : 90,
	"children" :
	(
		{
			"name" : "board_ui",
			"type" : "board_with_titlebar",
			"x" : 0,
			"y" : 0,
			"width" : 170,
			"height" : 90,
			"title" : "",
			"children" :
			(
				{
					"name" : "input_slot_ui",
					"type" : "slotbar",
					"x" : 0,
					"y" : 34,
					"width" : 26,
					"height" : 18,
					"horizontal_align" : "center",
					"children" :
					(
						{
							"name" : "input_value_ui",
							"type" : "editline",
							"x" : 3,
							"y" : 3,
							"width" : 26,
							"height" : 18,
							"only_number" : True,
							"input_limit" : 3,
						},
					),
				},
				{
					"name" : "accept_button_ui",
					"type" : "button",
					"x" : -61 - 5 + 30,
					"y" : 58,
					"horizontal_align" : "center",
					"text" : uiScriptLocale.OK,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button_ui",
					"type" : "button",
					"x" : 5 + 30,
					"y" : 58,
					"horizontal_align" : "center",
					"text" : uiScriptLocale.CANCEL,
					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}
