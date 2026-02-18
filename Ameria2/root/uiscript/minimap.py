# coding: latin_1

ROOT = "d:/ymir work/ui/minimap/"
RUTA_IMGS = "System/Dungeon/design/"
PATCH_IMG = "d_ranking_gremios/"
ROOT2 = "d:/ymir work/ui/game/minimapa/"

import localeInfo
import app

window = {
	"name" : "MiniMap",

	"x" : SCREEN_WIDTH - 136,
	"y" : 0,

	"width" : 136,
	"height" : 137,

	"children" :
	(
		## OpenWindow
		{
			"name" : "OpenWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 136,
			"height" : 137,

			"children" :
			(
				{
					"name" : "OpenWindowBGI",
					"type" : "image",
					"x" : 0,
					"y" : 0,
					"image" : ROOT + "minimap.sub",
				},
				## MiniMapWindow
				{
					"name" : "MiniMapWindow",
					"type" : "window",

					"x" : 4,
					"y" : 5,

					"width" : 128,
					"height" : 128,
				},
				## ScaleUpButton
				{
					"name" : "ScaleUpButton",
					"type" : "button",

					"x" : 101,
					"y" : 116,

					"default_image" : ROOT + "minimap_scaleup_default.sub",
					"over_image" : ROOT + "minimap_scaleup_over.sub",
					"down_image" : ROOT + "minimap_scaleup_down.sub",
				},
				## ScaleDownButton
				{
					"name" : "ScaleDownButton",
					"type" : "button",

					"x" : 115,
					"y" : 103,

					"default_image" : ROOT + "minimap_scaledown_default.sub",
					"over_image" : ROOT + "minimap_scaledown_over.sub",
					"down_image" : ROOT + "minimap_scaledown_down.sub",
				},	
				## MiniMapHideButton
				{
					"name" : "MiniMapHideButton",
					"type" : "button",

					"x" : 111,
					"y" : 6,

					"default_image" : ROOT + "bead_default.sub",
					"over_image" : ROOT + "bead_over.sub",
					"down_image" : ROOT + "bead_down.sub",
				},
				## AtlasShowButton
				{
					"name" : "AtlasShowButton",
					"type" : "button",

					"x" : 11,
					"y" : 11,

					"default_image" : ROOT + "bead_default.sub",
					"over_image" : ROOT + "bead_over.sub",
					"down_image" : ROOT + "bead_down.sub",
				},

				{
					"name" : "DungeonSystemButton",
					"type" : "button",

					"x" : -2,
					"y" : 91,

					"default_image" : ROOT2 + "btn_timer_normal.png",
					"over_image" : ROOT2 + "btn_timer_hover.png",
					"down_image" : ROOT2 + "btn_timer_down.png",
				},

				## ServerInfo
				{
					"name" : "ServerInfo",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 160,

					"text" : "",
				},
				## PositionInfo
				{
					"name" : "Title_New",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 173,

					"text" : localeInfo.SERVER_TIME,
				},

				# ObserverCount
				{
					"name" : "Hora",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 185,

					"text" : "",
				},
				# {"name" : "skill_tree","type" : "button","x" : 4,"y" : 8,"default_image" : "d:/ymir work/ui/game/skill_tree/skilltree_inactive_default.tga","over_image" : "d:/ymir work/ui/game/skill_tree/skilltree_inactive_hover.tga","down_image" : "d:/ymir work/ui/game/skill_tree/skilltree_inactive_down.tga",},

				## ObserverCount
				{
					"name" : "ObserverCount",
					"type" : "text",
					
					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 200,

					"text" : "",
				},
			),
		},

		{
			"name" : "bio",
			"type" : "button",
			"x" : 18,
			"y" : 112,
			"default_image" : ROOT2 + "btn_bio_normal.png",
			"over_image" : ROOT2 + "btn_bio_hover.png",
			"down_image" : ROOT2 + "btn_bio_down.png",
			#"tooltip_text": localeInfo.BIO_TITLE,
		},
		
		{
			"name" : "battlepass",
			"type" : "button",
			"x" : -8,
			"y" : 65,
			"default_image" : ROOT2 + "btn_battlepass_normal.png",
			"over_image" : ROOT2 + "btn_battlepass_hover.png",
			"down_image" : ROOT2 + "btn_battlepass_down.png",
			#"tooltip_text": localeInfo.BATTLEPASS_TITLE,
		},
		{
			"name" : "WikiButton",
			"type" : "button",
			"x" : 75,
			"y" : 122,
			"default_image" :ROOT2 + "wiki_btn_01.png",
			"over_image" : ROOT2 + "wiki_btn_02.png",
			"down_image" : ROOT2 + "wiki_btn_03.png",
			#"tooltip_text": localeInfo.BATTLEPASS_TITLE,
		},
		{
			"name" : "OpenRewardWindow",
			"type" : "button",
			"x" : -8,
			"y" : 36,
			"default_image" : ROOT2 + "btn_ranking_normal.png",
			"over_image" : ROOT2 + "btn_ranking_hover.png",
			"down_image" : ROOT2 + "btn_ranking_down.png",
			#"tooltip_text": localeInfo.BATTLEPASS_TITLE,
		},
		{
			"name" : "SearchButton",
			"type" : "button",
			"x" : 40,
			"y" : 119,
			"default_image" :  ROOT2 + "normal.tga",
			"over_image" :  ROOT2 + "hover.tga",
			"down_image" :  ROOT2 + "press.tga",
			#"tooltip_text": localeInfo.BATTLEPASS_TITLE,
		},
		{
			"name" : "CloseWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 132,
			"height" : 48,

			"children" :
			(
				## ShowButton
				{
					"name" : "MiniMapShowButton",
					"type" : "button",

					"x" : 100,
					"y" : 4,

					"default_image" : ROOT + "minimap_open_default.sub",
					"over_image" : ROOT + "minimap_open_default.sub",
					"down_image" : ROOT + "minimap_open_default.sub",
				},
			),
		},
		 #{"name" : "ButonEveniment","type" : "button","x" : 6,"y" : 8,"default_image" : "d:/ymir work/ui/game/12zi/bead/bead_default.sub","over_image" : "d:/ymir work/ui/game/12zi/bead/bead_down.sub","down_image" : "d:/ymir work/ui/game/12zi/bead/bead_over.sub",},
		 #{"name" : "beadInfo","type" : "text","x" : 15,"y" : 15,"text":"0",},
	),
}

if app.__BL_BATTLE_ROYALE__:
	window["children"][0]["children"] = window["children"][0]["children"] + (
		{
			"name" : "ClientTimerText",
			"type" : "text",
			
			"text_horizontal_align" : "center",

			"fontname" : "Tahoma:12b",
			"outline" : 1,

			"x" : 70,
			"y" : 200,
		},
	)
