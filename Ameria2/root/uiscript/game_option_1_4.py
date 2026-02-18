# coding: latin_1

import localeInfo
import uiScriptLocale

IMG_DIR = "d:/ymir work/ui/game/gameoption/"

TITLE_IMAGE_TEXT_X = 5
TITLE_IMAGE_TEXT_Y = 4

OPTION_START_X = 17
SLIDER_POSITION_X = 50

SLIDER_START_Y = 40
BUTTON_START_Y = 33
BUTTON_NEXT_Y = 20

RADIO_BUTTON_RANGE_X = 65
TOGGLE_BUTTON_RANGE_X = 65

RADIO_BUTTON_TEXT_X = 25
TOGGLE_BUTTON_TEXT_X = 20

SMALL_OPTION_HEIGHT = 65
NORMAL_OPTION_HEIGHT = 80
SLIDER_OPTION_HEIGHT = 65

window = {
	"name" : "GameOptionDialog",
	"style" : (),
	"x" : 171,
	"y" : 3,
	"width" : 300,
	"height" : 324,
	"children" :
	(
		{
			"name" : "snow_terrain_window",
			"type" : "window",
			"x" : 0,
			"y" : 0,
			"width" : 304,
			"height" : SMALL_OPTION_HEIGHT,
			"children" :
			(
				{
					"name" : "snow_terrain_title_img",
					"type" : "expanded_image",
					"x" : 0,
					"y" : 0,
					"image" : IMG_DIR + "option_title.tga",
					"children":
					(
						{
							"name" : "title_snow_terrain",
							"type" : "text",
							"x" : TITLE_IMAGE_TEXT_X,
							"y" : TITLE_IMAGE_TEXT_Y,
							"text_horizontal_align":"left",
							"text" : localeInfo.GAME_OPTIONS_TITLE_TERRAIN_SNOW,
						},
					),
				},
				{
					"name" : "snowterrain_on",
					"type" : "radio_button",
					"x" : OPTION_START_X,
					"y" : 33,
					"text" : uiScriptLocale.OPTION_NIGHT_MODE_ON,
					"text_x" : RADIO_BUTTON_TEXT_X,
					"default_image" : IMG_DIR + "radio_unselected.tga",
					"over_image" : IMG_DIR + "radio_unselected.tga",
					"down_image" : IMG_DIR + "radio_selected.tga",
				},
				{
					"name" : "snowterrain_off",
					"type" : "radio_button",
					"x" : OPTION_START_X + RADIO_BUTTON_RANGE_X * 1,
					"y" : 33,
					"text" : uiScriptLocale.OPTION_NIGHT_MODE_OFF,
					"text_x" : RADIO_BUTTON_TEXT_X,
					"default_image" : IMG_DIR + "radio_unselected.tga",
					"over_image" : IMG_DIR + "radio_unselected.tga",
					"down_image" : IMG_DIR + "radio_selected.tga",
				},
			),
		},
	),
}
