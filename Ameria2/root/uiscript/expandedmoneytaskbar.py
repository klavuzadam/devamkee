# coding: utf-8

import app

ROOT = "d:/ymir work/ui/game/"

BOARD_WIDTH_EXTRA = 0

if app.USE_CHEQUE_CURRENCY:
	BOARD_WIDTH_EXTRA += 60

if app.ENABLE_GEM_SYSTEM:
	BOARD_WIDTH_EXTRA += 73

BOARD_WIDTH = 160 + BOARD_WIDTH_EXTRA
BOARD_HEIGHT = 40

window = {
	"name" : "ExpandedMoneyTaskbar_Window",
	"x" : SCREEN_WIDTH - BOARD_WIDTH,
	"y" : SCREEN_HEIGHT - 65,
	"width" : BOARD_WIDTH,
	"height" : BOARD_HEIGHT,
	"style" : ("float",),
	"children" :
	[
		{
			"name" : "ExpandedMoneyTaskBar_Board",
			"type" : "board",
			"x" : 0,
			"y" : 0,
			"width" : BOARD_WIDTH,
			"height" : BOARD_HEIGHT,
			"children" :
			[
				{
					"name" : "money_icon_ui",
					"type" : "image",
					"image" : "d:/ymir work/ui/game/windows/money_icon.sub",
					"x" : 20 + BOARD_WIDTH_EXTRA,
					"y" : 10,
				},
				{
					"name" : "money_slot_ui",
					"type" : "button",
					"default_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
					"over_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
					"down_image" : "d:/ymir work/ui/public/parameter_slot_04.sub",
					"x" : 39 + BOARD_WIDTH_EXTRA,
					"y" : 9,
					"x_scale" : 0.92,
					"children" :
					(
						{
							"name" : "money_text_ui",
							"type" : "text",
							"text" : "0",
							"x" : 5,
							"y" : 2,
							"horizontal_align" : "right",
							"text_horizontal_align" : "right",
						},
					),
				},
			],
		},
	],
}

if app.USE_CHEQUE_CURRENCY:
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{
			"name" : "cheque_icon_ui",
			"type" : "image",
			"image" : "d:/ymir work/ui/game/windows/cheque_icon.sub",
			"x" : 0,
			"y" : 0,
		},
		{
			"name" : "cheque_slot_ui",
			"type" : "button",
			"default_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
			"over_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
			"down_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
			"x" : 0,
			"y" : 0,
			"x_scale" : 0.8,
			"children" :
			(
				{
					"name" : "cheque_text_ui",
					"type" : "text",
					"text" : "0",
					"x" : 7,
					"y" : 2,
					"horizontal_align" : "right",
					"text_horizontal_align" : "right",
				},
			),
		},
	]

if app.ENABLE_GEM_SYSTEM:
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{
			"name" : "gem_icon_ui",
			"type" : "image",
			"image" : "d:/ymir work/ui/gemshop/gemshop_gemicon.sub",
			"x" : 0,
			"y" : 0,
		},
		{
			"name" : "gem_slot_ui",
			"type" : "button",
			"default_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
			"over_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
			"down_image" : "d:/ymir work/ui/public/parameter_slot_01.sub",
			"x" : 0,
			"y" : 0,
			"x_scale" : 0.9,
			"children" :
			(
				{
					"name" : "gem_text_ui",
					"type" : "text",
					"text" : "0",
					"x" : 5,
					"y" : 2,
					"horizontal_align" : "right",
					"text_horizontal_align" : "right",
				},
			),
		},
	]
