import uiScriptLocale
import item
import app
import player

WINDOW_WIDTH = 176
WINDOW_HEIGHT = 547
INVENTORY_PATH = "d:/ymir work/ui/game/interfaces/special_storage/"

EQUIPMENT_START_INDEX = player.EQUIPMENT_SLOT_START
CHANGE_EQUIP_START_INDEX = player.CHANGE_EQUIP_SLOT_START
COSTUME_START_INDEX = item.COSTUME_SLOT_START

window = {
	"name" : "InventoryWindow",

	"x" : SCREEN_WIDTH - WINDOW_WIDTH - 4 - 29,
	"y" : SCREEN_HEIGHT - 37 - WINDOW_HEIGHT - 47,

	"style" : ("movable", "float", "animate",),

	"width" : WINDOW_WIDTH + 29,
	"height" : WINDOW_HEIGHT,

	"children" :
	(		
		{
			"name" : "board",
			"type" : "board",
			"style" : ("attach",),

			"x" : 29,
			"y" : 0,

			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,

			"children" :
			(
				## Separate
				{
					"name" : "SeparateBaseImage",
					"type" : "image",
					"style" : ("attach",),

					"x" : 8,
					"y" : 7,

					"image" : INVENTORY_PATH + "btn-settings-1.png",

					"children" :
					(
						## Separate Button (38x24)
						{
							"name" : "ManageInventory",
							"type" : "button",

							"x" : 0,
							"y" : 0,

							"default_image" : INVENTORY_PATH + "btn-settings-1.png",
							"over_image" : INVENTORY_PATH + "btn-settings-2.png",
							"down_image" : INVENTORY_PATH + "btn-settings-3.png",
							"disable_image" : INVENTORY_PATH + "btn-settings-3.png",
						},
					),
				},

				## Title
				{
					"name" : "TitleBar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8 + 38,
					"y" : 7,

					"width" : 161 - 38,

					"color" : "yellow",

					"children" :
					(
						{ "name":"TitleName", "type":"text", "x":0, "y":3, "text":uiScriptLocale.INVENTORY_TITLE, "text_horizontal_align":"center", "horizontal_align":"center" },
					),
				},

				## Equipment Slot
				{
					"name" : "Equipment_Base_Tab_0",
					"type" : "image",
					"style" : ("attach",),
					"x" : 10,
					"y" : 33,

					"image" : "d:/ymir work/ui/game/inventory/equipment_bg.png",

					"children" :
					(

						{
							"name" : "EquipmentSlot_0",
							"type" : "slot",

							"x" : 3,
							"y" : 3,

							"width" : 150,
							"height" : 182,

							"slot" : (
										{"index":EQUIPMENT_START_INDEX+0, "x":39, "y":37, "width":32, "height":64},
										{"index":EQUIPMENT_START_INDEX+1, "x":39, "y":2, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+2, "x":39, "y":145, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+3, "x":75, "y":67, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+4, "x":3, "y":3, "width":32, "height":96},
										{"index":EQUIPMENT_START_INDEX+5, "x":114, "y":67, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+6, "x":114, "y":35, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+7, "x":2, "y":145, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+8, "x":75, "y":145, "width":32, "height":32},
										{"index":item.COSTUME_SLOT_START + 3, "x":75, "y":2, "width":32, "height":32},
										{"index":EQUIPMENT_START_INDEX+10, "x":75, "y":35, "width":32, "height":32},
										{"index":230, "x":115, "y":2, "width":32, "height":32},
										{"index":208, "x":3, "y":106, "width":32, "height":32},
										{"index":item.EQUIPMENT_BELT-1, "x":39, "y":106, "width":32, "height":32},
										# {"index":item.EQUIPMENT_PENDANT, "x":3, "y":106, "width":32, "height":32},
									),
						},
						##Dragon Soul Button
						{
							"name" : "DSSButton",
							"type" : "button",

							"x" : 112,
							"y" : 107,

							"default_image" : "d:/ymir work/ui/dragonsoul/dss_inventory_button_01.tga",
							"over_image" : "d:/ymir work/ui/dragonsoul/dss_inventory_button_02.tga",
							"down_image" : "d:/ymir work/ui/dragonsoul/dss_inventory_button_03.tga",
						},

						##Offline Shop
						{
							"name" : "OfflineShopButton",
							"type" : "button",

							"x" : 77,
							"y" : 108,

							"default_image" : "d:/ymir work/ui/shop_button_01.png",
							"over_image" : "d:/ymir work/ui/shop_button_02.png",
							"down_image" : "d:/ymir work/ui/shop_button_03.png",
						},

						{
							"name" : "MallButton",
							"type" : "button",

							"x" : 115,
							"y" : 148,

							"default_image" : "d:/ymir work/ui/game/taskbar/mall_button_01.tga",
							"over_image" : "d:/ymir work/ui/game/taskbar/mall_button_02.tga",
							"down_image" : "d:/ymir work/ui/game/taskbar/mall_button_03.tga",
						},
					),
				},

				## Costume Equipment Slot
				{
					"name" : "Equipment_Base_Tab_1",
					"type" : "image",
					"style" : ("attach",),
					"x" : 10,
					"y" : 33,
					"image" : "d:/ymir work/ui/game/inventory/costume_bg.png",
					"children" :
					(
						{
							"name" : "EquipmentSlot_1",
							"type" : "slot",
							"x" : 3,
							"y" : 3,
							"width" : 150,
							"height" : 182,
							"slot" : (
									),
						},
					),
				},
				## Secondary Equipment Slot
				{
					"name" : "Equipment_Base_Tab_2",
					"type" : "image",
					"style" : ("attach",),
					"x" : 10,
					"y" : 33,
					"image" : "d:/ymir work/ui/game/inventory/booster_bg.png",
					"children" :
					(
						{
							"name" : "EquipmentSlot_2",
							"type" : "slot",
							"x" : 3,
							"y" : 3,
							"width" : 150,
							"height" : 182,
							"slot" : (
									),
						},
					),
				},
				{
					"name" : "Equipment_Base_Tab_3",
					"type" : "image",
					"style" : ("attach",),
					"x" : 10,
					"y" : 33,
					"image" : "d:/ymir work/ui/game/inventory/companion_bg.png",
					"children" :
					(
						{
							"name" : "EquipmentSlot_3",
							"type" : "slot",
							"x" : 0,"y" : 0,
							"width" : 165,"height" : 165,
							"slot" : (

							),
						},
					),
				},
				## Talisman Equipment Slot
				{
					"name" : "Equipment_Base_Tab_4",
					"type" : "image",
					"style" : ("attach",),
					"x" : 10,
					"y" : 33,
					"image" : "d:/ymir work/ui/game/inventory/talisman_bg.png",
					"children" :
					(
						{
							"name" : "EquipmentSlot_4",
							"type" : "slot",
							"x" : 3,
							"y" : 3,
							"width" : 150,
							"height" : 182,
							"slot" : (
									),
						},

						{
							"name" : "pendant_page_0",
							"type" : "radio_button",
							"x" : 12,
							"y" : 3+109+10+25,
							"default_image" : "d:/ymir work/ui/game/inventory/talisman1-icon-1.png",
							"over_image" : "d:/ymir work/ui/game/inventory/talisman1-icon-2.png",
							"down_image" : "d:/ymir work/ui/game/inventory/talisman1-icon-3.png",
						},
						{
							"name" : "pendant_page_1",
							"type" : "radio_button",
							"x" : 112,
							"y" : 3+109+10+25,
							"default_image" : "d:/ymir work/ui/game/inventory/talisman2-icon-1.png",
							"over_image" : "d:/ymir work/ui/game/inventory/talisman2-icon-2.png",
							"down_image" : "d:/ymir work/ui/game/inventory/talisman2-icon-3.png",
						},

						{
							"name" : "PendantSetBonusWindow",
							"type" : "window",

							"x" : 62,
							"y" : 76,

							"width" : 34,
							"height" : 34,

							"children":
							(
								{
									"name" : "PendantSetBonusIcon",
									"type" : "special_image",

									"x" : 0,
									"y" : -4,

									"image" : "d:/ymir work/ui/test/pendant_set_inactive.dds",
								},

								{
									"name" : "PendantSetBonusActiveEffect",
									"type" : "ani_image",

									"style" : ("not_pick",),
									
									"x" : 0, "y" : -4,
									
									"delay" : 5,
									
									"images" :
									(
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_0.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_4.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_8.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_12.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_16.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_20.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_24.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_28.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_32.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_36.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_36.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_32.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_28.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_24.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_20.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_16.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_12.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_8.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_4.dds",
										"d:/ymir work/ui/test/pendant_set_bonus_effect/pendant_set_bonus_ani_0.dds",
									),
								},
							),
						},
						
						{
							"name" : "Pendant_Ice_Glow",
							"type" : "ani_image",
							
							"x" : 62 - 7, "y" : 12 - 7,
							"delay" : 5,
							
							"images" :
							(
								"d:/ymir work/ui/test/ice/0_opacity.dds",
								"d:/ymir work/ui/test/ice/15_opacity.dds",
								"d:/ymir work/ui/test/ice/25_opacity.dds",
								"d:/ymir work/ui/test/ice/35_opacity.dds",
								"d:/ymir work/ui/test/ice/45_opacity.dds",
								"d:/ymir work/ui/test/ice/50_opacity.dds",
								"d:/ymir work/ui/test/ice/65_opacity.dds",
								"d:/ymir work/ui/test/ice/75_opacity.dds",
								"d:/ymir work/ui/test/ice/85_opacity.dds",
								"d:/ymir work/ui/test/ice/100_opacity.dds",
								"d:/ymir work/ui/test/ice/100_opacity.dds",
								"d:/ymir work/ui/test/ice/85_opacity.dds",
								"d:/ymir work/ui/test/ice/75_opacity.dds",
								"d:/ymir work/ui/test/ice/65_opacity.dds",
								"d:/ymir work/ui/test/ice/50_opacity.dds",
								"d:/ymir work/ui/test/ice/45_opacity.dds",
								"d:/ymir work/ui/test/ice/35_opacity.dds",
								"d:/ymir work/ui/test/ice/25_opacity.dds",
								"d:/ymir work/ui/test/ice/15_opacity.dds",
								"d:/ymir work/ui/test/ice/0_opacity.dds",
							),
							
							"style" : ("not_pick",),
						},
						{
							"name" : "Pendant_Fire_Glow",
							"type" : "ani_image",
							
							"x" : 113 - 7, "y" : 48 - 7,
							"delay" : 5,
							
							"images" :
							(
								"d:/ymir work/ui/test/fire/0_opacity.dds",
								"d:/ymir work/ui/test/fire/15_opacity.dds",
								"d:/ymir work/ui/test/fire/25_opacity.dds",
								"d:/ymir work/ui/test/fire/35_opacity.dds",
								"d:/ymir work/ui/test/fire/45_opacity.dds",
								"d:/ymir work/ui/test/fire/50_opacity.dds",
								"d:/ymir work/ui/test/fire/65_opacity.dds",
								"d:/ymir work/ui/test/fire/75_opacity.dds",
								"d:/ymir work/ui/test/fire/85_opacity.dds",
								"d:/ymir work/ui/test/fire/100_opacity.dds",
								"d:/ymir work/ui/test/fire/100_opacity.dds",
								"d:/ymir work/ui/test/fire/85_opacity.dds",
								"d:/ymir work/ui/test/fire/75_opacity.dds",
								"d:/ymir work/ui/test/fire/65_opacity.dds",
								"d:/ymir work/ui/test/fire/50_opacity.dds",
								"d:/ymir work/ui/test/fire/45_opacity.dds",
								"d:/ymir work/ui/test/fire/35_opacity.dds",
								"d:/ymir work/ui/test/fire/25_opacity.dds",
								"d:/ymir work/ui/test/fire/15_opacity.dds",
								"d:/ymir work/ui/test/fire/0_opacity.dds",
							),
							
							"style" : ("not_pick",),
						},
						{
							"name" : "Pendant_Earth_Glow",
							"type" : "ani_image",
							
							"x" : 113 - 7, "y" : 109 - 7,
							"delay" : 5,
							
							"images" :
							(
								"d:/ymir work/ui/test/earth/0_opacity.dds",
								"d:/ymir work/ui/test/earth/15_opacity.dds",
								"d:/ymir work/ui/test/earth/25_opacity.dds",
								"d:/ymir work/ui/test/earth/35_opacity.dds",
								"d:/ymir work/ui/test/earth/45_opacity.dds",
								"d:/ymir work/ui/test/earth/50_opacity.dds",
								"d:/ymir work/ui/test/earth/65_opacity.dds",
								"d:/ymir work/ui/test/earth/75_opacity.dds",
								"d:/ymir work/ui/test/earth/85_opacity.dds",
								"d:/ymir work/ui/test/earth/100_opacity.dds",
								"d:/ymir work/ui/test/earth/100_opacity.dds",
								"d:/ymir work/ui/test/earth/85_opacity.dds",
								"d:/ymir work/ui/test/earth/75_opacity.dds",
								"d:/ymir work/ui/test/earth/65_opacity.dds",
								"d:/ymir work/ui/test/earth/50_opacity.dds",
								"d:/ymir work/ui/test/earth/45_opacity.dds",
								"d:/ymir work/ui/test/earth/35_opacity.dds",
								"d:/ymir work/ui/test/earth/25_opacity.dds",
								"d:/ymir work/ui/test/earth/15_opacity.dds",
								"d:/ymir work/ui/test/earth/0_opacity.dds",
							),
							
							"style" : ("not_pick",),
						},
						{
							"name" : "Pendant_Dark_Glow",
							"type" : "ani_image",
							
							"x" : 62 - 7, "y" : 146 - 7,
							"delay" : 5,
							
							"images" :
							(
								"d:/ymir work/ui/test/dark/0_opacity.dds",
								"d:/ymir work/ui/test/dark/15_opacity.dds",
								"d:/ymir work/ui/test/dark/25_opacity.dds",
								"d:/ymir work/ui/test/dark/35_opacity.dds",
								"d:/ymir work/ui/test/dark/45_opacity.dds",
								"d:/ymir work/ui/test/dark/50_opacity.dds",
								"d:/ymir work/ui/test/dark/65_opacity.dds",
								"d:/ymir work/ui/test/dark/75_opacity.dds",
								"d:/ymir work/ui/test/dark/85_opacity.dds",
								"d:/ymir work/ui/test/dark/100_opacity.dds",
								"d:/ymir work/ui/test/dark/100_opacity.dds",
								"d:/ymir work/ui/test/dark/85_opacity.dds",
								"d:/ymir work/ui/test/dark/75_opacity.dds",
								"d:/ymir work/ui/test/dark/65_opacity.dds",
								"d:/ymir work/ui/test/dark/50_opacity.dds",
								"d:/ymir work/ui/test/dark/45_opacity.dds",
								"d:/ymir work/ui/test/dark/35_opacity.dds",
								"d:/ymir work/ui/test/dark/25_opacity.dds",
								"d:/ymir work/ui/test/dark/15_opacity.dds",
								"d:/ymir work/ui/test/dark/0_opacity.dds",
							),
							
							"style" : ("not_pick",),
						},
						{
							"name" : "Pendant_Thunder_Glow",
							"type" : "ani_image",
							
							"x" : 12 - 7, "y" : 109 - 7,
							"delay" : 5,
							
							"images" :
							(
								"d:/ymir work/ui/test/thunder/0_opacity.dds",
								"d:/ymir work/ui/test/thunder/15_opacity.dds",
								"d:/ymir work/ui/test/thunder/25_opacity.dds",
								"d:/ymir work/ui/test/thunder/35_opacity.dds",
								"d:/ymir work/ui/test/thunder/45_opacity.dds",
								"d:/ymir work/ui/test/thunder/50_opacity.dds",
								"d:/ymir work/ui/test/thunder/65_opacity.dds",
								"d:/ymir work/ui/test/thunder/75_opacity.dds",
								"d:/ymir work/ui/test/thunder/85_opacity.dds",
								"d:/ymir work/ui/test/thunder/100_opacity.dds",
								"d:/ymir work/ui/test/thunder/100_opacity.dds",
								"d:/ymir work/ui/test/thunder/85_opacity.dds",
								"d:/ymir work/ui/test/thunder/75_opacity.dds",
								"d:/ymir work/ui/test/thunder/65_opacity.dds",
								"d:/ymir work/ui/test/thunder/50_opacity.dds",
								"d:/ymir work/ui/test/thunder/45_opacity.dds",
								"d:/ymir work/ui/test/thunder/35_opacity.dds",
								"d:/ymir work/ui/test/thunder/25_opacity.dds",
								"d:/ymir work/ui/test/thunder/15_opacity.dds",
								"d:/ymir work/ui/test/thunder/0_opacity.dds",
							),
							
							"style" : ("not_pick",),
						},
						{
							"name" : "Pendant_Wind_Glow",
							"type" : "ani_image",
							
							"x" : 12 - 7, "y" : 48 - 7,
							"delay" : 5,
							
							"images" :
							(
								"d:/ymir work/ui/test/wind/0_opacity.dds",
								"d:/ymir work/ui/test/wind/15_opacity.dds",
								"d:/ymir work/ui/test/wind/25_opacity.dds",
								"d:/ymir work/ui/test/wind/35_opacity.dds",
								"d:/ymir work/ui/test/wind/45_opacity.dds",
								"d:/ymir work/ui/test/wind/50_opacity.dds",
								"d:/ymir work/ui/test/wind/65_opacity.dds",
								"d:/ymir work/ui/test/wind/75_opacity.dds",
								"d:/ymir work/ui/test/wind/85_opacity.dds",
								"d:/ymir work/ui/test/wind/100_opacity.dds",
								"d:/ymir work/ui/test/wind/100_opacity.dds",
								"d:/ymir work/ui/test/wind/85_opacity.dds",
								"d:/ymir work/ui/test/wind/75_opacity.dds",
								"d:/ymir work/ui/test/wind/65_opacity.dds",
								"d:/ymir work/ui/test/wind/50_opacity.dds",
								"d:/ymir work/ui/test/wind/45_opacity.dds",
								"d:/ymir work/ui/test/wind/35_opacity.dds",
								"d:/ymir work/ui/test/wind/25_opacity.dds",
								"d:/ymir work/ui/test/wind/15_opacity.dds",
								"d:/ymir work/ui/test/wind/0_opacity.dds",
							),
							
							"style" : ("not_pick",),
						},
					),
				},

				{
					"name" : "Inventory_Tab_01",
					"type" : "radio_button",

					"x" : 10,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_1,

					"children" :
					(
						{
							"name" : "Inventory_Tab_01_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "I",
						},
					),
				},
				{
					"name" : "Inventory_Tab_02",
					"type" : "radio_button",

					#"x" : 10 + 78,
					"x" : 10 + 39,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_2,

					"children" :
					(
						{
							"name" : "Inventory_Tab_02_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "II",
						},
					),
				},
				
				{
					"name" : "Inventory_Tab_03",
					"type" : "radio_button",

					"x" : 10 + 39 + 39,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_3,

					"children" :
					(
						{
							"name" : "Inventory_Tab_03_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "III",
						},
					),
				},
				
				{
					"name" : "Inventory_Tab_04",
					"type" : "radio_button",

					"x" : 10 + 39 + 39 + 39,
					"y" : 33 + 191,

					"default_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_01.sub",
					"over_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_02.sub",
					"down_image" : "d:/ymir work/ui/game/windows/tab_button_large_half_03.sub",

					"tooltip_text" : uiScriptLocale.INVENTORY_PAGE_BUTTON_TOOLTIP_4,

					"children" :
					(
						{
							"name" : "Inventory_Tab_04_Print",
							"type" : "text",

							"x" : 0,
							"y" : 0,

							"all_align" : "center",

							"text" : "IV",
						},
					),
				},

				## Item Slot
				{
					"name" : "ItemSlot",
					"type" : "grid_table",

					"x" : 8,
					"y" : 246,

					"start_index" : 0,
					"x_count" : 5,
					"y_count" : 9,
					"x_step" : 32,
					"y_step" : 32,

					"image" : "d:/ymir work/ui/public/Slot_Base.sub"
				},
				## Print
				{
					"name":"Money_Slot",
					"type":"button",

					"x":8,
					"y":29,

					"horizontal_align":"center",
					"vertical_align":"bottom",

					"default_image" : "icon_barra/barra_yang.tga",
					"over_image" : "icon_barra/barra_yang.tga",
					"down_image" : "icon_barra/barra_yang.tga",

					"children" :
					(
						{
							"name":"Money_Icon",
							"type":"image",

							"x":-18,
							"y":2,

							"image":"d:/ymir work/ui/game/windows/money_icon.sub",
						},

						{
							"name" : "Money",
							"type" : "text",

							"x" : 3,
							"y" : 2,

							"horizontal_align" : "right",
							"text_horizontal_align" : "right",

							"text" : "123456789",
						},
					),
				},

			),
		},
		{
			"name" : "Equipment_Tab_0",
			"type" : "radio_button",
			"x" : 15,
			"y" : 33,
			"default_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"over_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"down_image" : "d:/ymir work/ui/game/inventory/eqtabopen.png",
			"children" :
			(
				{
					"name" : "Equipment_Tab_01_Print",
					"type" : "text",
					"x" : 0,
					"y" : 0,
					"all_align" : "center",
					"text" : "I",
				},
			),
		},
		{
			"name" : "Equipment_Tab_1",
			"type" : "radio_button",
			"x" : 15,
			"y" : 33 + 22,
			"default_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"over_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"down_image" : "d:/ymir work/ui/game/inventory/eqtabopen.png",
			"children" :
			(
				{
					"name" : "Equipment_Tab_02_Print",
					"type" : "text",
					"x" : 0,
					"y" : 0,
					"all_align" : "center",
					"text" : "II",
				},
			),
		},
		{
			"name" : "Equipment_Tab_2",
			"type" : "radio_button",
			"x" : 15,
			"y" : 33 + 22*2,
			"default_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"over_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"down_image" : "d:/ymir work/ui/game/inventory/eqtabopen.png",
			"children" :
			(
				{
					"name" : "Equipment_Tab_03_Print",
					"type" : "text",
					"x" : 0,
					"y" : 0,
					"all_align" : "center",
					"text" : "III",
				},
			),
		},
		{
			"name" : "Equipment_Tab_3",
			"type" : "radio_button",
			"x" : 15,
			"y" : 33 + 22*3,
			"default_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"over_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"down_image" : "d:/ymir work/ui/game/inventory/eqtabopen.png",
			"children" :
			(
				{
					"name" : "Equipment_Tab_04_Print",
					"type" : "text",
					"x" : 0,
					"y" : 0,
					"all_align" : "center",
					"text" : "IV",
				},
			),
		},
		{
			"name" : "Equipment_Tab_4",
			"type" : "radio_button",
			"x" : 15,
			"y" : 33 + 22*4,
			"default_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"over_image" : "d:/ymir work/ui/game/inventory/eqtabclosed.png",
			"down_image" : "d:/ymir work/ui/game/inventory/eqtabopen.png",
			"children" :
			(
				{
					"name" : "Equipment_Tab_05_Print",
					"type" : "text",
					"x" : 0,
					"y" : 0,
					"all_align" : "center",
					"text" : "V",
				},
			),
		},
	),
}