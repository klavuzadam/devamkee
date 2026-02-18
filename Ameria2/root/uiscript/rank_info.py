import uiScriptLocale

WINDOW_WIDTH = 579
WINDOW_HEIGHT = 445
Y_SPACER = 4

window = {
	"name" : "RankInfo",

	"x" : SCREEN_WIDTH - 175 - 650,
	"y" : SCREEN_HEIGHT - 37 - 575,

	"style" : ("movable", "float","animation",),

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
				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 6,
					"y" : 7,

					"width" : WINDOW_WIDTH - 15,

					"children" :
					(
						{ 
							"name": "TitleName", 
							"type": "text", 
							"x": 0, 
							"y": 3, 
							"text": "Top Global", 
							"horizontal_align": "center", 
							"text_horizontal_align": "center" 
						},
					),
				},

				{
					"name" : "rank_board",
					"type" : "image",
					"style" : ("attach",),
					
					"x" : 8, 
					"y" : 32,
					
					"image" : "d:/ymir work/ui/game/ranking/background.png",
				},
			),
		},
	),
}
