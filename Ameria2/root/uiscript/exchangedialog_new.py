# coding: utf-8

import app
import uiScriptLocale
 
ROOT = "d:/ymir work/ui/game/"
FACE_SLOT_FILE = "d:/ymir work/ui/game/windows/box_face.sub"

BOARD_WIDTH_EXTRA = 0

if app.USE_CHEQUE_CURRENCY:
	BOARD_WIDTH_EXTRA += 65

window = {
	"name" : "ExchangeDialog_Window",
	"x" : 0,
	"y" : 0,
	"style" : ("movable", "float",),
	"width" : 400 + 42,
	"height" : 200 + 50 + 90,
	"children" :
	(
		{
			"name" : "ExchangeLogs",
			"type" : "thinboard",
			"style" : ("attach",),
			"x" : 0,
			"y" : 235,
			"width" : 392,
			"height" : 80,
			"horizontal_align" : "center",
		},
		
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),
			"x" : 0,
			"y" : 0,
			"width" : 400 + 42,
			"height" : 200 + 50,
			"children" :
			(
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),
					"x" : 8,
					"y" : 8,
					"width" : 384 + 42,
					"color" : "gray",
					"children" :
					(
						{
							"name" : "TitleName",
							"type" : "text",
							"x" : 192,
							"y" : 3,
							"text" : uiScriptLocale.EXCHANGE_TITLE,
							"text_horizontal_align" : "center"
						},
					),
				},
				{
					"name" : "FaceOwner_Image",
					"type" : "image",
					"x" : 200 + 38 + 11 - 6,
					"y" : 39,
					"image" : "d:/ymir work/ui/game/windows/face_warrior.sub"
				},
				{
					"name" : "FaceOwner_Slot",
					"type" : "image",
					"x" : 200 + 38 + 7 - 6,
					"y" : 34,
					"image" : FACE_SLOT_FILE,
				},
				{
					"name" : "own_Text",
					"type" : "text",
					"x" : 200 + 38 + 7 - 6 + 32 + 32,
					"y" : 32 + 5,
					"r" : 0.8500,
					"g" : 0.2500,
					"b" : 0.2500,
					"text" : uiScriptLocale.NEW_EXCHANGE_YOU,
					"text_horizontal_align" : "center"
				},
				{
					"name" : "FaceTarget_Image",
					"type" : "image",
					"x" : 11 + 4,
					"y" : 39,
					"image" : "d:/ymir work/ui/game/windows/face_warrior.sub"
				},
				{
					"name" : "FaceTarget_Slot",
					"type" : "image",
					"x" : 7 + 4,
					"y" : 34,
					"image" : FACE_SLOT_FILE,
				},
				{
					"name" : "target_LvText",
					"type" : "text",
					"x" : 7 + 4 + 32 + 32 + 7,
					"y" : 32 + 5,
					"r" : 0.0,
					"g" : 0.8500,
					"b" : 0.0,
					"text" : "Lv. 1",
					"text_horizontal_align" : "center"
				},
				{
					"name" : "target_NameText",
					"type" : "text",
					"x" : 7 + 4 + 32 + 32 + 15 + 45,
					"y" : 32 + 5,
					"text" : "Character Name",
					"text_horizontal_align" : "center"
				},
				{
					"name" : "Owner",
					"type" : "window",
					"x" : 200 + 38,
					"y" : 33 + 50 + 2,
					"width" : 200,
					"height" : 150 + 36,
					"children" :
					(
						{
							"name" : "Owner_Slot",
							"type" : "grid_table",
							"start_index" : 0,
							"x" : 0,
							"y" : 25,
							"x_count" : 6,
							"y_count" : 4,
							"x_step" : 32,
							"y_step" : 32,
							"x_blank" : 0,
							"y_blank" : 0,
							"image" : "d:/ymir work/ui/public/slot_base.sub",
						},
						{
							"name" : "owner_cheque_icon_ui",
							"type" : "image",
							"image" : "d:/ymir work/ui/game/windows/cheque_icon.sub",
							"x" : 1,
							"y" : 4,
						},
						{
							"name" : "owner_cheque_slot_ui",
							"type" : "button",
							"default_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
							"over_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
							"down_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
							"x" : 20,
							"y" : 2,
							"x_scale" : 0.8,
							"children" :
							(
								{
									"name" : "owner_cheque_text_ui",
									"type" : "text",
									"text" : "0",
									"x" : 7,
									"y" : 2,
									"horizontal_align" : "right",
									"text_horizontal_align" : "right",
								},
							),
						},
						{
							"name" : "owner_money_icon_ui",
							"type" : "image",
							"image" : "d:/ymir work/ui/game/windows/money_icon.sub",
							"x" : 1 + BOARD_WIDTH_EXTRA,
							"y" : 3,
						},
						{
							"name" : "owner_money_slot_ui",
							"type" : "button",
							"default_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							"over_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							"down_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							"x" : 1 + BOARD_WIDTH_EXTRA + 18,
							"y" : 2,
							"x_scale" : 0.92,
							"children" :
							(
								{
									"name" : "owner_money_text_ui",
									"type" : "text",
									"text" : "0",
									"x" : 5,
									"y" : 2,
									"horizontal_align" : "right",
									"text_horizontal_align" : "right",
								},
							),
						},
					),
				},
				{
					"name" : "Target",
					"type" : "window",
					"x" : 10,
					"y" : 33 + 50 + 2,
					"width" : 200,
					"height" : 150 + 36,
					"children" :
					(
						{
							"name" : "Target_Slot",
							"type" : "grid_table",
							"start_index" : 0,
							"x" : 0,
							"y" : 25,
							"x_count" : 6,
							"y_count" : 4,
							"x_step" : 32,
							"y_step" : 32,
							"x_blank" : 0,
							"y_blank" : 0,
							"image" : "d:/ymir work/ui/public/slot_base.sub",
						},
						{
							"name" : "target_cheque_icon_ui",
							"type" : "image",
							"image" : "d:/ymir work/ui/game/windows/cheque_icon.sub",
							"x" : 1,
							"y" : 4,
						},
						{
							"name" : "target_cheque_slot_ui",
							"type" : "button",
							"default_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
							"over_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
							"down_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
							"x" : 20,
							"y" : 2,
							"x_scale" : 0.8,
							"children" :
							(
								{
									"name" : "target_cheque_text_ui",
									"type" : "text",
									"text" : "0",
									"x" : 7,
									"y" : 2,
									"horizontal_align" : "right",
									"text_horizontal_align" : "right",
								},
							),
						},
						{
							"name" : "target_money_icon_ui",
							"type" : "image",
							"image" : "d:/ymir work/ui/game/windows/money_icon.sub",
							"x" : 1 + BOARD_WIDTH_EXTRA,
							"y" : 3,
						},
						{
							"name" : "target_money_slot_ui",
							"type" : "button",
							"default_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							"over_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							"down_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
							"x" : 1 + BOARD_WIDTH_EXTRA + 18,
							"y" : 2,
							"x_scale" : 0.92,
							"children" :
							(
								{
									"name" : "target_money_text_ui",
									"type" : "text",
									"text" : "0",
									"x" : 5,
									"y" : 2,
									"horizontal_align" : "right",
									"text_horizontal_align" : "right",
								},
							),
						},
					),
				},
				{
					"name" : "Owner_Accept_Button",
					"type" : "toggle_button",
					"text" : " ",
					"x" : 33 + 50 + 2 + 25 + 1 + 64 + 30,
					"y" : 200 + 18 - 96 + 38,
					"default_image" : "d:/ymir work/ui/game/exchange/own_arrow_01.tga",
					"over_image" : "d:/ymir work/ui/game/exchange/own_arrow_02.tga",
					"down_image" : "d:/ymir work/ui/game/exchange/own_arrow_03.tga",
				},
				{
					"name" : "Target_Accept_Button",
					"type" : "toggle_button",
					"text" : " ",
					"x" : 33 + 50 + 2 + 25 + 1 + 64 + 30,
					"y" : 200 + 18 - 96 + 38 + 18,
					"default_image" : "d:/ymir work/ui/game/exchange/target_arrow_01.tga",
					"over_image" : "d:/ymir work/ui/game/exchange/target_arrow_01.tga",
					"down_image" : "d:/ymir work/ui/game/exchange/target_arrow_03.tga",
				},
			),
		},
	),
}
