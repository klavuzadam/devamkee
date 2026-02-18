import localeInfo

PET_UI_ROOT = "d:/ymir work/ui/pet_new/"
PET_UI_ROOT2 = "d:/ymir work/ui/pet/"
PET_UI_BG_WIDTH		= 345
PET_UI_BG_HEIGHT	= 493

## Evol Name, Pet Name, Pet Abilities
LONG_LABEL_WIDTH	= 266
LONG_LABEL_HEIGHT	= 19

## Level, exp, age, life
SHORT_LABLE_WIDTH	= 90
SHORT_LABLE_HEIGHT	= 20

## Defence, Magic Defence, HP
MIDDLE_LABLE_WIDTH	= 168
MIDDLE_LABLE_HEIGHT	= 20

## EXP Gague interval
EXP_GAGUE_INTERVAL	= 2
EXP_IMG_WIDTH		= 16
EXP_IMG_HEIGHT		= 16

## TEXT COLOR
GOLD_COLOR	= 0xFFFEE3AE
WHITE_COLOR = 0xFFFFFFFF
	
window = {
	"name" : "PetInformationWindow",
	"style" : ("movable", "float", "animate",),
	
	"x" : SCREEN_WIDTH - 176 -200 -146 -145,
	"y" : SCREEN_HEIGHT - 37 - 565,

	"width" : PET_UI_BG_WIDTH,
	"height" : PET_UI_BG_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : PET_UI_BG_WIDTH,
			"height" : PET_UI_BG_HEIGHT,
			
			"children" :
			(
				{ "name" : "PetUIBG", "type" : "expanded_image", "style" : ("attach",), "x" : 0, "y" : 0, "image" : PET_UI_ROOT+"Pet_UI_bg.tga" },
				
				{ 
					"name" : "TitleWindow", "type" : "window", "x" : 20, "y" : 5, "width" : PET_UI_BG_WIDTH-10-15, "height" : 15, "style" : ("attach",),
					"children" :
					(
						{"name":"TitleName", "type":"text", "x":0, "y":0, "text":localeInfo.PET_GUI_TITLE, "all_align" : "center"},
					),	
				},

				{ 
					"name" : "CloseButton", 
					"type" : "button", 
					"x" : PET_UI_BG_WIDTH -10-11, 
					"y" : 6, 
					"default_image" : "d:/ymir work/ui/public/close_button_01.sub",	
					"over_image" : "d:/ymir work/ui/public/close_button_02.sub",
					"down_image" : "d:/ymir work/ui/public/close_button_03.sub",
				},

				{
					"name" : "UpBringing_Pet_Slot",
					"type" : "slot",
					"x" : 22,
					"y" : 52,
					"width" : 32,
					"height" : 32,
					
					"slot" : ({"index":0, "x":0, "y":0, "width":32, "height":32},),
				},

				{ 
					"name" : "PetNameWindow", "type" : "window", "x" : 60, "y" : 45, "width" : LONG_LABEL_WIDTH, "height" : LONG_LABEL_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"PetName", "type":"text", "x":0, "y":0, "text": "", "color":GOLD_COLOR, "all_align" : "center","outline":1},
					),
				},

				{ 
					"name" : "PetMobNameWindow", "type" : "window", "x" : 60, "y" : 70, "width" : LONG_LABEL_WIDTH, "height" : LONG_LABEL_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"PetMobName", "type":"text", "x":0, "y":0, "text": "", "color":GOLD_COLOR, "all_align" : "center", "outline":1},
					),	
				},


				{ 
					"name" : "LevelWindow", "type" : "window", "x" : 23, "y" : 119, "width" : SHORT_LABLE_WIDTH, "height" : SHORT_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"LevelTitle", "type":"text", "x":0, "y":0, "text": "Level", "color":GOLD_COLOR, "all_align" : "center", "outline":1},
						{"name":"LevelValue", "type":"text", "x":0, "y":22, "text": "", "color":WHITE_COLOR, "all_align" : "center", "outline":1},
					),	
				},


				{ 
					"name" : "ExpWindow", "type" : "window", "x" : 126, "y" : 119, "width" : SHORT_LABLE_WIDTH, "height" : SHORT_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"ExpTitle", "type":"text", "x":0, "y":0, "text": localeInfo.PET_GUI_EXPERIENCE, "color":GOLD_COLOR, "all_align" : "center", "outline":1},
					),
				},


				{
					"name" : "UpBringing_Pet_EXP_Gauge_Board",
					"type" : "window",
					"style": ("ltr",),

					"x" : 127,
					"y" : 145,
				
					"width"		: EXP_IMG_WIDTH * 5 + EXP_GAGUE_INTERVAL * 4,
					"height"	: EXP_IMG_HEIGHT,

					"children" :
					(
						{
							"name" : "UpBringing_Pet_EXPGauge_01",
							"type" : "expanded_image",
							"style": ("not_pick",),
							"x" : 0,
							"y" : 0,

							"image" : PET_UI_ROOT + "exp_gauge/exp_on.sub",
						},
						{
							"name" : "UpBringing_Pet_EXPGauge_02",
							"type" : "expanded_image",
							"style": ("not_pick",),

							"x" : EXP_IMG_WIDTH + EXP_GAGUE_INTERVAL,
							"y" : 0,

							"image" : PET_UI_ROOT + "exp_gauge/exp_on.sub",
						},
						{
							"name" : "UpBringing_Pet_EXPGauge_03",
							"type" : "expanded_image",
							"style": ("not_pick",),

							"x" : EXP_IMG_WIDTH * 2 + EXP_GAGUE_INTERVAL * 2,
							"y" : 0,

							"image" : PET_UI_ROOT + "exp_gauge/exp_on.sub",
						},
						{
							"name" : "UpBringing_Pet_EXPGauge_04",
							"type" : "expanded_image",
							"style": ("not_pick",),

							"x" : EXP_IMG_WIDTH * 3 + EXP_GAGUE_INTERVAL * 3,
							"y" : 0,

							"image" : PET_UI_ROOT + "exp_gauge/exp_on.sub",
						},
					),
				},


				{ 
					"name" : "AgeWindow", "type" : "window", "x" : 227, "y" : 119, "width" : SHORT_LABLE_WIDTH, "height" : SHORT_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"AgeTitle", "type":"text", "x":0, "y":0, "text": localeInfo.PET_GUI_AGE, "color":GOLD_COLOR, "all_align" : "center", "outline":1},
						{"name":"AgeValue", "type":"text", "x":0, "y":22, "text": "", "color":WHITE_COLOR, "all_align" : "center", "outline":1},
					),	
				},


				{ 
					"name" : "LifeWindow", "type" : "window", "x" : 23, "y" : 164, "width" : 168, "height" : SHORT_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"LifeTitle", "type":"text", "x":0, "y":0, "text": localeInfo.PET_GUI_DURATION, "color":GOLD_COLOR, "all_align" : "center", "outline":1},
						{"name":"LifeTextValue", "type":"text", "x":0, "y":24, "text": "", "color":WHITE_COLOR, "all_align" : "center", "outline":1},
						{
							"name" : "LifeGauge",
							"type" : "ani_image",
							"x" : 10,
							"y" : 49,
							"delay" : 6,
							"images" :
							(
								"D:/Ymir Work/UI/Pattern/HPGauge/01.tga",
								"D:/Ymir Work/UI/Pattern/HPGauge/02.tga",
								"D:/Ymir Work/UI/Pattern/HPGauge/03.tga",
								"D:/Ymir Work/UI/Pattern/HPGauge/04.tga",
								"D:/Ymir Work/UI/Pattern/HPGauge/05.tga",
								"D:/Ymir Work/UI/Pattern/HPGauge/06.tga",
								"D:/Ymir Work/UI/Pattern/HPGauge/07.tga",
							),
						},
					),
				},

				{
					"name" : "FeedEvolButton",
					"type" : "button",
					"x" : 215,
					"y" : 170,

					"default_image" : PET_UI_ROOT2 + "evolve_0.png",
					"over_image" : PET_UI_ROOT2 + "evolve_1.png",
					"down_image" : PET_UI_ROOT2 + "evolve_2.png",
				},

				{
					"name" : "FeedEvol2Button",
					"type" : "button",
					"x" : 215,
					"y" : 203,

					"default_image" : PET_UI_ROOT2 + "evolve_3.png",
					"over_image" : PET_UI_ROOT2 + "evolve_3.png",
					"down_image" : PET_UI_ROOT2 + "evolve_3.png",
				},

				{ 
					"name" : "AbilitiesWindow", "type" : "window", "x" : 43, "y" : 254, "width" : LONG_LABEL_WIDTH, "height" : LONG_LABEL_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"AbilitiesName", "type":"text", "x":0, "y":0, "text": localeInfo.PET_GUI_BONUSES, "color":GOLD_COLOR, "all_align" : "center", "outline":1},
					),
				},

				{
					"name" : "FirstBonusWindow", "type" : "window", "x" : 20, "y" : 279, "width" : MIDDLE_LABLE_WIDTH, "height" : MIDDLE_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"bonus_title_0", "type":"text", "x":0, "y":0, "text": "", "color":GOLD_COLOR, "all_align" : "center", "outline":1},
						{"name":"bonus_value_0", "type":"text", "x":155, "y":0, "text": "", "color":WHITE_COLOR, "all_align" : "center", "outline":1},
					),
				},

				{
					"name" : "SecondBonusWindow", "type" : "window", "x" : 20, "y" : 301, "width" : MIDDLE_LABLE_WIDTH, "height" : MIDDLE_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"bonus_title_1", "type":"text", "x":0, "y":0, "text": "", "color":GOLD_COLOR, "all_align" : "center", "outline":1},
						{"name":"bonus_value_1", "type":"text", "x":155, "y":0, "text": "", "color":WHITE_COLOR, "all_align" : "center", "outline":1},
					),
				},

				{
					"name" : "ThirdBonusWindow", "type" : "window", "x" : 20, "y" : 323, "width" : MIDDLE_LABLE_WIDTH, "height" : MIDDLE_LABLE_HEIGHT, "style" : ("attach",),
					"children" :
					(
						{"name":"bonus_title_2", "type":"text", "x":0, "y":0, "text": "", "color":GOLD_COLOR, "all_align" : "center", "outline":1},
						{"name":"bonus_value_2", "type":"text", "x":155, "y":0, "text": "", "color":WHITE_COLOR, "all_align" : "center", "outline":1},
					),
				},

				{
					"name" : "PetSkillWindow", "type" : "window", "x" : 11, "y" : 366, "width" : 120, "height" : 20, "style" : ("attach",),
					"children" :
					(
						{"name":"PetSkillTitle", "type":"text", "x":0, "y":0, "text": localeInfo.PET_GUI_PETSKILL,"color":GOLD_COLOR, "all_align" : "center", "outline":1},
					),
				},

				{
					"name" : "PetSkillSlot",
					"type" : "slot",

					"x" : 0,
					"y" : 362,
					"width" : 345,
					"height" : 131,
					"image" : "d:/ymir work/ui/public/Slot_Base.sub",
					
					"slot" : (
						{"index":0, "x":153, "y":0, "width":32, "height":32},
						{"index":1, "x":153+64, "y":0, "width":32, "height":32},
						{"index":2, "x":153+128, "y":0, "width":32, "height":32},
						
						{"index":3, "x":27, "y":40, "width":32, "height":32},
						{"index":4, "x":27+52, "y":40, "width":32, "height":32},
						{"index":5, "x":27+52+52, "y":40, "width":32, "height":32},
						{"index":6, "x":27+52+52+52, "y":40, "width":32, "height":32},
						{"index":7, "x":27+52+52+52+52+1, "y":40+1, "width":32, "height":32},
						{"index":8, "x":27+52+52+52+52+52, "y":40, "width":32, "height":32},
						
						{"index":9, "x":27, "y":84, "width":32, "height":32},
						{"index":10, "x":27+52, "y":84, "width":32, "height":32},
						{"index":11, "x":27+52+52, "y":84, "width":32, "height":32},
						{"index":12, "x":27+52+52+52, "y":84, "width":32, "height":32},
						{"index":13, "x":27+52+52+52+52, "y":84, "width":32, "height":32},
						{"index":14, "x":27+52+52+52+52+52, "y":84, "width":32, "height":32},
					
					),
				},
			), 
		},
	),
}
