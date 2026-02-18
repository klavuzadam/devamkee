# coding: latin_1

import uiScriptLocale

window = {
	"name" : "GameWindow",
	"style" : ("not_pick",),

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		{ 
			"name":"HelpButton", 
			"type":"button", 
			"x" : 80,
			"y" : SCREEN_HEIGHT-170,
			"default_image" : "d:/ymir work/ui/game/windows/btn_bigplus_up.sub",
			"over_image" : "d:/ymir work/ui/game/windows/btn_bigplus_over.sub",
			"down_image" : "d:/ymir work/ui/game/windows/btn_bigplus_down.sub",

			"children" : 
			(
				{ 
					"name":"HelpButtonLabel", 
					"type":"text", 
					"x": 16, 
					"y": 40, 
					"text":uiScriptLocale.GAME_HELP, 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},
			),
		},
		{ 
			"name":"QuestButton", 
			"type":"button", 
			"x" : SCREEN_WIDTH-50-32,
			"y" : SCREEN_HEIGHT-170,
			"default_image" : "d:/ymir work/ui/game/windows/btn_bigplus_up.sub",
			"over_image" : "d:/ymir work/ui/game/windows/btn_bigplus_over.sub",
			"down_image" : "d:/ymir work/ui/game/windows/btn_bigplus_down.sub",

			"children" : 
			(
				{ 
					"name":"QuestButtonLabel", 
					"type":"text", 
					"x": 16, 
					"y": 40, 
					"text":uiScriptLocale.GAME_QUEST, 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},
			),
		},
		{
			"name" : "GiftIcon",
			"type" : "button",

			"x" : 5, 
			"y" : SCREEN_HEIGHT-135, 
			"default_image" : "D:/Ymir Work/UI/Pattern/GiftBox/present_for_fruends_icon.tga",
			"over_image" : "D:/Ymir Work/UI/Pattern/GiftBox/present_for_fruends_icon.tga",
			"down_image" : "D:/Ymir Work/UI/Pattern/GiftBox/present_for_fruends_icon.tga",
		},
		
		{
			"name":"Vote4Buff",
			"type" : "button",
			"x" : 40,
			"y" : SCREEN_HEIGHT-260,
			"default_image" : "d:/ymir work/ui/game/interfaces/vote4buff/effect/vote_placer.png",
			"over_image" : "d:/ymir work/ui/game/interfaces/vote4buff/effect/vote_placer.png",
			"down_image" : "d:/ymir work/ui/game/interfaces/vote4buff/effect/vote_placer.png",
		},		
		{
			"name":"Vote4BuffEffect",
			"type" : "ani_image",
			"style" : ("not_pick",),
			"x" : 40,
			"y" : SCREEN_HEIGHT-260,
			"delay" : 30,

			"images" :
			(
				"d:/ymir work/ui/game/interfaces/vote4buff/effect/v4b_35.png",
				"d:/ymir work/ui/game/interfaces/vote4buff/effect/vote_placer.png",
			),
		},


		{ 
			"name":"StatusPlusButton", 
			"type" : "button", 
			"x" : 80, 
			"y" : SCREEN_HEIGHT-100, 
			"default_image" : "d:/ymir work/ui/game/windows/btn_bigplus_up.sub",
			"over_image" : "d:/ymir work/ui/game/windows/btn_bigplus_over.sub",
			"down_image" : "d:/ymir work/ui/game/windows/btn_bigplus_down.sub",

			"children" :
			(
				{ 
					"name":"StatusPlusLabel", 
					"type":"text", 
					"x": 16, 
					"y": 40, 
					"text":uiScriptLocale.GAME_STAT_UP, 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},		
			),
		},			
		{ 
			"name":"SkillPlusButton", 
			"type" : "button", 
			"x" : SCREEN_WIDTH-50-32,
			"y" : SCREEN_HEIGHT-170,
			"default_image" : "d:/ymir work/ui/game/windows/btn_bigplus_up.sub",
			"over_image" : "d:/ymir work/ui/game/windows/btn_bigplus_over.sub",
			"down_image" : "d:/ymir work/ui/game/windows/btn_bigplus_down.sub",

			"children" : 
			(
				{ 
					"name":"SkillPlusLabel", 
					"type":"text", 
					"x": 16, 
					"y": 40, 
					"text":uiScriptLocale.GAME_SKILL_UP, 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},	
			),
		},			
		{ 
			"name":"ExitObserver", 
			"type" : "button", 
			"x" : SCREEN_WIDTH-50-32,
			"y" : SCREEN_HEIGHT-170,
			"default_image" : "d:/ymir work/ui/game/windows/btn_bigplus_up.sub",
			"over_image" : "d:/ymir work/ui/game/windows/btn_bigplus_over.sub",
			"down_image" : "d:/ymir work/ui/game/windows/btn_bigplus_down.sub",

			"children" : 
			(
				{ 
					"name":"ExitObserverButtonName", 
					"type":"text", 
					"x": 16, 
					"y": 40, 
					"text": uiScriptLocale.GAME_EXIT_OBSERVER, 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},	
			),
		},
		{
			"name":"CombatZone", 
			"type" : "button", 
			"x" : 60,
			"y" : SCREEN_HEIGHT-170-70,
			"default_image" : "flag/combat_1.tga",
			"over_image" : "flag/combat_2.tga",
			"down_image" : "flag/combat_3.tga",

			"children" : 
			(
				{ 
					"name":"ExitObserverButtonName", 
					"type":"text", 
					"x": 35, 
					"y": 40, 
					"text": "click", 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},	
			),
		},
		{
			"name":"BossEvent", 
			"type" : "button", 
			"x" : 60,
			"y" : SCREEN_HEIGHT-170-70,
			"default_image" : "flag/boss_1.tga",
			"over_image" : "flag/boss_2.tga",
			"down_image" : "flag/boss_3.tga",

			"children" : 
			(
				{ 
					"name":"ExitObserverButtonName", 
					"type":"text", 
					"x": 35, 
					"y": 40, 
					"text": "click", 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},	
			),
		},

		{
			"name":"MetinEvent", 
			"type" : "button", 
			"x" : 60,
			"y" : SCREEN_HEIGHT-170-70-70,
			"default_image" : "flag/metin_1.tga",
			"over_image" : "flag/metin_2.tga",
			"down_image" : "flag/metin_3.tga",

			"children" : 
			(
				{ 
					"name":"TextMetinEvent", 
					"type":"text", 
					"x": 35, 
					"y": 40, 
					"text": "click", 
					"r":1.0, "g":1.0, "b":1.0, "a":1.0, 
					"text_horizontal_align":"center" 
				},	
			),
		},

		{ 
			"name":"BuildGuildBuilding",
			"type" : "button",
			"x" : SCREEN_WIDTH-50-32,
			"y" : SCREEN_HEIGHT-170,
			"default_image" : "d:/ymir work/ui/game/windows/btn_bigplus_up.sub",
			"over_image" : "d:/ymir work/ui/game/windows/btn_bigplus_over.sub",
			"down_image" : "d:/ymir work/ui/game/windows/btn_bigplus_down.sub",

			"children" : 
			(
				{ 
					"name":"BuildGuildBuildingButtonName",
					"type":"text",
					"x": 16,
					"y": 40,
					"text": uiScriptLocale.GUILD_BUILDING_TITLE,
					"r":1.0, "g":1.0, "b":1.0, "a":1.0,
					"text_horizontal_align":"center"
				},	
			),
		},
	),
}
