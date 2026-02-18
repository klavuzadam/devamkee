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
			"name" : "night_mode_window",
			"type" : "window",
			"x" : 0,
			"y" : 0,
			"width" : 304,
			"height" : SMALL_OPTION_HEIGHT,
			"children" :
			(
				{
					"name" : "night_mode_title_img",
					"type" : "expanded_image",
					"x" : 0,
					"y" : 0,
					"image" : IMG_DIR + "option_title.tga",
					"children":
					(
						{
							"name" : "title_night_mode",
							"type" : "text",
							"x" : TITLE_IMAGE_TEXT_X,
							"y" : TITLE_IMAGE_TEXT_Y,
							"text_horizontal_align":"left",
							"text" : uiScriptLocale.OPTION_NIGHT_MODE,
						},
					),
				},
				{
					"name" : "nightmode_on",
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
					"name" : "nightmode_off",
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
		{
			"name" : "snow_mode_window",
			"type" : "window",
			"x" : 0,
			"y" : SMALL_OPTION_HEIGHT,
			"width" : 304,
			"height" : SMALL_OPTION_HEIGHT,
			"children" :
			(
				{
					"name" : "snow_mode_title_img",
					"type" : "expanded_image",
					"x" : 0,
					"y" : 0,
					"image" : IMG_DIR + "option_title.tga",
					"children":
					(
						{
							"name" : "title_snow_mode",
							"type" : "text",
							"x" : TITLE_IMAGE_TEXT_X,
							"y" : TITLE_IMAGE_TEXT_Y,
							"text_horizontal_align":"left",
							"text" : localeInfo.GAME_OPTIONS_TITLE_ENVIRONMENT_SNOW,
						},
					),
				},
				{
					"name" : "snowmode_on",
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
					"name" : "snowmode_off",
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
		{
			"name" : "fog_mode_window",
			"type" : "window",
			"x" : 0,
			"y" : SMALL_OPTION_HEIGHT * 2,
			"width" : 304,
			"height" : SMALL_OPTION_HEIGHT + 16,
			"children" :
			(
				{
					"name" : "fog_mode_title_img",
					"type" : "expanded_image",
					"x" : 0,
					"y" : 0,
					"image" : IMG_DIR + "option_title.tga",
					"children":
					(
						{
							"name" : "title_fog_mode",
							"type" : "text",
							"x" : TITLE_IMAGE_TEXT_X,
							"y" : TITLE_IMAGE_TEXT_Y,
							"text_horizontal_align":"left",
							"text" : localeInfo.GAME_OPTIONS_TITLE_ENVIRONMENT_FOG,
						},
					),
				},
				{
					"name" : "fogmode_on",
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
					"name" : "fogmode_off",
					"type" : "radio_button",
					"x" : OPTION_START_X + RADIO_BUTTON_RANGE_X * 1,
					"y" : 33,
					"text" : uiScriptLocale.OPTION_NIGHT_MODE_OFF,
					"text_x" : RADIO_BUTTON_TEXT_X,
					"default_image" : IMG_DIR + "radio_unselected.tga",
					"over_image" : IMG_DIR + "radio_unselected.tga",
					"down_image" : IMG_DIR + "radio_selected.tga",
				},
				{
					"name" : "fog_range_text",
					"type" : "text",
					"text_horizontal_align":"center",
					"x" : 304 / 2,
					"y" : SLIDER_START_Y + 16,
					"text" : localeInfo.GAME_OPTIONS_ENVIRONMENT_FOG_RANGE,
				},
				{
					"name" : "fog_range_controller",
					"type" : "sliderbar",
					"x" : OPTION_START_X + SLIDER_POSITION_X,
					"y" : SLIDER_START_Y + 20 + 14,
				},
			),
		},
	),
}
