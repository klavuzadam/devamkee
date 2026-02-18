import uiScriptLocale
window = {
	"name" : "ChatBlockDialog",

	"x" : 100,
	"y" : 100,

	"style" : ("movable", "float",),

	"width" : 280,
	"height" : 120,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",

			"x" : 0,
			"y" : 0,

			"width" : 280,
			"height" : 120,
			"title" : "ChatBlock",

			"children" :
			(

				## Stunden
				{
					"name" : "hour_slot",
					"type" : "image",

					"x" : 10,
					"y" : 34,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "hour_value",
							"type" : "editline",

							"x" : 3,
							"y" : 2,

							"width" : 40,
							"height" : 18,

							"input_limit" : 6,
							"only_number" : 1,

							"text" : "0",
						},
						{
							"name" : "h",
							"type" : "text",

							"x" : 63,
							"y" : 2,

							"text" : "(h)",
						},
					),
				},


				## Minuten
				{
					"name" : "min_slot",
					"type" : "thinboard_circle",

					"x" : 130,
					"y" : 34,

					"width" : 130,
					"height" : 20,

					"children" :
					(
						{
							"name" : "min_value",
							"type" : "editline",

							"x" : 3,
							"y" : 2,

							"width" : 127,
							"height" : 32,

							"input_limit" : 24,

							"text" : "",
						},
						{
							"name" : "m",
							"type" : "text",

							"x" : -37,
							"y" : 1,

							"text" : "Reason:",
						},
					),
				},



				## Button
				{
					"name" : "accept_button",
					"type" : "button",

					"x" : 280/2 - 61 - 5,
					"y" : 88,

					"text" : uiScriptLocale.OK,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 280/2 + 5,
					"y" : 88,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : "d:/ymir work/ui/public/middle_button_01.sub",
					"over_image" : "d:/ymir work/ui/public/middle_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/middle_button_03.sub",
				},
			),
		},
	),
}