# coding: latin_1

import uiScriptLocale
import localeInfo
import app

ROOT = "d:/ymir work/ui/game/"
NEW_DESIGN_FOLDER = "d:/ymir work/ui/game/new_inventory/"


Y_ADD_POSITION = 0
window = {
	"name" : "ExpandTaskBar",
	"style" : ("ltr", ),

	"x" : SCREEN_WIDTH/2 - 52,
	"y" : SCREEN_HEIGHT - 74,

	"width" : 40 * 12,
	"height" : 37,

	"children" :
	[
		{
			"name" : "ExpanedTaskBar_Board",
			"type" : "window",
			"style" : ("ltr", ),

			"x" : 0,
			"y" : 0,

			"width" : 40 * 12,
			"height" : 37,

			"children" :
			[
				{
					"name" : "DragonSoulButton",
					"type" : "button",
					"style" : ("ltr", ),

					"x" : 0,
					"y" : 0,

					"width" : 37,
					"height" : 37,

					"tooltip_text" : uiScriptLocale.TASKBAR_DRAGON_SOUL,

					"default_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_01.tga",
					"over_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_02.tga",
					"down_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_03.tga",
				},

				{
					"name" : "WikiButton",
					"type" : "button",
					"style" : ("ltr", ),
					"x" : 40 * 1,
					"y" : 0,
					"width" : 37,
					"height" : 37,
					"tooltip_text" : localeInfo.WIKI_TITLE2,
					"default_image" : "d:/ymir work/ui/game/taskbar/btn_wiki_0.png",
					"over_image" : "d:/ymir work/ui/game/taskbar/btn_wiki_1.png",
					"down_image" : "d:/ymir work/ui/game/taskbar/btn_wiki_2.png",
				},

				{
					"name" : "ShopSearchButton",
					"type" : "button",
					"style" : ("ltr", ),
					"x" : 40 * 2,
					"y" : 0,
					"width" : 37,
					"height" : 37,
					"tooltip_text" : uiScriptLocale.SHOP_SEARCH_TITLE,
					"default_image" : NEW_DESIGN_FOLDER+"search_0.tga",
					"over_image" : NEW_DESIGN_FOLDER+"search_1.tga",
					"down_image" : NEW_DESIGN_FOLDER+"search_2.tga",
				},

				{
					"name" : "PetButton",
					"type" : "button",
					"style" : ("ltr", ),
					"x" : 40 * 3,
					"y" : 0,
					"width" : 37,
					"height" : 37,
					"tooltip_text" : "Pet",
					"default_image" : "d:/ymir work/ui/game/taskbar/taskbar_pet_button_01.png",
					"over_image" : "d:/ymir work/ui/game/taskbar/taskbar_pet_button_02.png",
					"down_image" : "d:/ymir work/ui/game/taskbar/taskbar_pet_button_03.png",
				},
				{
					"name" : "SwitchbotButton",
					"type" : "button",
					"style" : ("ltr", ),
					"x" : 40 * 4,
					"y" : 0,
					"width" : 37,
					"height" : 37,
					"tooltip_text" : "Switchbot",
					"default_image" : "d:/ymir work/ui/game/taskbar/btn_switch_0.png",
					"over_image" : "d:/ymir work/ui/game/taskbar/btn_switch_1.png",
					"down_image" : "d:/ymir work/ui/game/taskbar/btn_switch_2.png",
				},
				{
					"name" : "AutoButton",
					"type" : "button",
					"style" : ("ltr", ),
					"x" : 40 * 5,
					"y" : 0,
					"width" : 37,
					"height" : 37,
					"tooltip_text" : "AutoHunt",
					"default_image" : "d:/ymir work/ui/game/taskbar/btn_eq_1.png",
					"over_image" : "d:/ymir work/ui/game/taskbar/btn_eq_2.png",
					"down_image" : "d:/ymir work/ui/game/taskbar/btn_eq_3.png",
				},
			],
		},
	],
}
