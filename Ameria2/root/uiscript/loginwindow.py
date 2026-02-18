import uiScriptLocale

PATH = "d:/ymir work/ui/game/interfaces/login/"
ROOT_PATH = "d:/ymir work/ui/game/login/"

ID_LIMIT_COUNT = 19
PW_LIMIT_COUNT = 16
SCALE_X = 1.0
SCALE_Y = 1.0

ACCOUNT_SPACER = 45

window = {
	"name" : "LoginWindow",
	"style" : ("movable",),

	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		## Board
		{
			"name" : "bg1", "type" : "expanded_image", "x" : 0, "y" : 0,
			"x_scale" : float(SCREEN_WIDTH) / 1920.0, "y_scale" : float(SCREEN_HEIGHT) / 1080.0,
			"image" : PATH + "rework/bg.png",
		},
		
		## LoginBoard
		{
			"name" : "LoginBoard",
			"type" : "image",

			"x" : 0,
			"y" : 50,
			
			"vertical_align" : "center",
			"horizontal_align" : "center",
			
			"image" : PATH + "rework/login.png",

			"children" :
			(
	
				# {"name":"ForgotLink", "type":"textlink", "x":192, "y":295, "text":uiScriptLocale.LOGIN_FORGOT,},
				
				# {"name":"RegisterLink", "type":"textlink", "x":0, "y":230, "text":uiScriptLocale.LOGIN_REGISTER, "horizontal_align" : "center",},
				
				{"name":"RegisterLink", "type":"textlink", "x":185, "y":28, "text":"Create a New Account!", },
				{"name":"ForgotLink", "type":"textlink", "x":215, "y":85, "text":"Reset Password?",},

				{
					"name" : "ID_EditLine",
					"type" : "editline",

					"x" : 90,
					"y" : 53,
					
					"width" : 120,
					"height" : 18,

					"overlay_text" : "Username",
					"input_limit" : ID_LIMIT_COUNT,
					"enable_codepage" : 0,

					"r" : 0.875,
					"g" : 0.839,
					"b" : 0.733,
					"a" : 1.0,
				},
				{
					"name" : "Password_EditLine",
					"type" : "editline",

					"x" : 90,
					"y" : 110,

					"width" : 120,
					"height" : 18,
					
					"overlay_text" : "Password",

					"input_limit" : PW_LIMIT_COUNT,
					"secret_flag" : 1,
					"enable_codepage" : 0,

					"r" : 0.875,
					"g" : 0.839,
					"b" : 0.733,
					"a" : 1.0,
				},
				{
					"name" : "LoginButton",
					"type" : "button",

					"x" : 80,
					"y" : 165,
					
					"default_image" : PATH + "rework/button_1.png",
					"over_image" : PATH + "rework/button_2.png",
					"down_image" : PATH + "rework/button_1.png",
				},
				{
					"name" : "LoginExitButton",
					"type" : "button",

					"x" : 110,
					"y" : 165,

					# "default_image" : PATH + "button_1.png",
					# "over_image" : PATH + "button_2.png",
					# "down_image" : PATH + "button_1.png",
					
					# "text" : uiScriptLocale.LOGIN_EXIT,
				},
				{
					"name" : "Ch1Button",
					"type" : "radio_button",

					"x" : 343, "y": 7,

					"default_image" : PATH + "rework/ch_button_1.png",
					"over_image" : PATH + "rework/ch_button_2.png",
					"down_image" : PATH + "rework/ch_button_2.png",
					
					"text" : "CH1",
					"fontname" : "Tahoma:18",
				},
				{
					"name" : "Ch2Button",
					"type" : "radio_button",

					"x" : 343 + (50 * 1), "y": 7,

					"default_image" : PATH + "rework/ch_button_1.png",
					"over_image" : PATH + "rework/ch_button_2.png",
					"down_image" : PATH + "rework/ch_button_2.png",
					
					"text" : "CH2",
					"fontname" : "Tahoma:18",
				},
				{
					"name" : "Ch3Button",
					"type" : "radio_button",

					"x" : 343 + (50 * 2), "y": 7,

					"default_image" : PATH + "rework/ch_button_1.png",
					"over_image" : PATH + "rework/ch_button_2.png",
					"down_image" : PATH + "rework/ch_button_2.png",
					
					"text" : "CH3",
					"fontname" : "Tahoma:18",
				},
				{
					"name" : "Ch4Button",
					"type" : "radio_button",

					"x" : 343 + (50 * 3), "y": 7,

					"default_image" : PATH + "rework/ch_button_1.png",
					"over_image" : PATH + "rework/ch_button_2.png",
					"down_image" : PATH + "rework/ch_button_2.png",
					
					"text" : "CH4",
					"fontname" : "Tahoma:18",
				},
				{
					"name" : "Ch5Button",
					"type" : "radio_button",

					"x" : 343 + (50 * 4), "y": 7,
					
					"default_image" : PATH + "rework/ch_button_1.png",
					"over_image" : PATH + "rework/ch_button_2.png",
					"down_image" : PATH + "rework/ch_button_2.png",
					
					"text" : "CH5",
					"fontname" : "Tahoma:18",
				},
				
				{
					"name" : "SlotLine1_Button",
					"type" : "button",

					"x" : 360, "y": 72,

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					
					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F1",
						},

						{
							"name" : "SlotLine1_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine1_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine1_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},		
				{
					"name" : "SlotLine2_Button",
					"type" : "button",

					"x" : 360, "y": 72 + (38 * 1),
					
					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F2",
						},

						{
							"name" : "SlotLine2_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine2_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine2_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine3_Button",
					"type" : "button",

					"x" : 360, "y": 72 + (38 * 2),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F3",
						},

						{
							"name" : "SlotLine3_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine3_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine3_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine4_Button",
					"type" : "button",

					"x" : 360, "y": 72 + (38 * 3),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F4",
						},

						{
							"name" : "SlotLine4_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine4_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine4_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine5_Button",
					"type" : "button",

					"x" : 360, "y": 72 + (38 * 4),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F5",
						},

						{
							"name" : "SlotLine5_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine5_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine5_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine6_Button",
					"type" : "button",

					"x" : 360, "y": 72 + (38 * 5),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F6",
						},

						{
							"name" : "SlotLine6_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine6_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine6_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				
				{
					"name" : "SlotLine7_Button",
					"type" : "button",

					"x" : 360 + 190, "y": 72,

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					
					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F7",
						},

						{
							"name" : "SlotLine7_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine7_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine7_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},		
				{
					"name" : "SlotLine8_Button",
					"type" : "button",

					"x" : 360 + 190, "y": 72 + (38 * 1),
					
					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F8",
						},

						{
							"name" : "SlotLine8_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine8_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine8_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine9_Button",
					"type" : "button",

					"x" : 360 + 190, "y": 72 + (38 * 2),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F9",
						},

						{
							"name" : "SlotLine9_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine9_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine9_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine10_Button",
					"type" : "button",

					"x" : 360 + 190, "y": 72 + (38 * 3),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F10",
						},

						{
							"name" : "SlotLine10_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine10_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine10_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine11_Button",
					"type" : "button",

					"x" : 360 + 190, "y": 72 + (38 * 4),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F11",
						},

						{
							"name" : "SlotLine11_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine11_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine11_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				{
					"name" : "SlotLine12_Button",
					"type" : "button",

					"x" : 360 + 190, "y": 72 + (38 * 5),

					"default_image" : PATH + "rework/acc_button_1.png",
					"over_image" : PATH + "rework/acc_button_2.png",
					"down_image" : PATH + "rework/acc_button_1.png",
					

					"children" :
					(
						{
							"name" : "F_Text",
							"type" : "text",
							
							"x" : 8,
							"y" : 9,
		
							"text" : "F12",
						},

						{
							"name" : "SlotLine12_Text",
							"type" : "text",
							
							"x" : 0,
							"y" : -2,
		
							"text" : uiScriptLocale.ACCOUNT_SAVED,
							"all_align" : "center",
						},
						{
							"name" : "SlotLine12_Button_Delete",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/delete_1.png",
							"over_image" : PATH + "rework/delete_2.png",
							"down_image" : PATH + "rework/delete_1.png",
						},
						{
							"name" : "SlotLine12_Button_Save",
							"type" : "button",

							"x" : 149,
							"y" : 8,

							"default_image" : PATH + "rework/check_1.png",
							"over_image" : PATH + "rework/check_2.png",
							"down_image" : PATH + "rework/check_1.png",
						},
					),
				},
				
				{
					"name": "language_eu_button",
					"type": "button",

					"x": 24 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "English",

					"default_image": ROOT_PATH + "uk_button_01.sub",
					"over_image": ROOT_PATH + "uk_button_02.sub",
					"down_image": ROOT_PATH + "uk_button_03.sub",
				},
				{
					"name": "language_de_button",
					"type": "button",

					"x": 51 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Deutsch",

					"default_image": ROOT_PATH + "de_button_01.sub",
					"over_image": ROOT_PATH + "de_button_02.sub",
					"down_image": ROOT_PATH + "de_button_03.sub",
				},
				{
					"name": "language_it_button",
					"type": "button",

					"x": 78 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Italiano",

					"default_image": ROOT_PATH + "it_button_01.sub",
					"over_image": ROOT_PATH + "it_button_02.sub",
					"down_image": ROOT_PATH + "it_button_03.sub",
				},
				{
					"name": "language_tr_button",
					"type": "button",

					"x": 159 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Turkce",

					"default_image": ROOT_PATH + "tr_button_01.sub",
					"over_image": ROOT_PATH + "tr_button_02.sub",
					"down_image": ROOT_PATH + "tr_button_03.sub",
				},
				{
					"name": "language_ro_button",
					"type": "button",

					"x": 132 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Romana",

					"default_image": ROOT_PATH + "ro_button_01.sub",
					"over_image": ROOT_PATH + "ro_button_02.sub",
					"down_image": ROOT_PATH + "ro_button_03.sub",
				},
				{
					"name": "language_pt_button",
					"type": "button",

					"x": 186 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Portugues",

					"default_image": ROOT_PATH + "pt_button_01.sub",
					"over_image": ROOT_PATH + "pt_button_02.sub",
					"down_image": ROOT_PATH + "pt_button_03.sub",
				},
				{
					"name": "language_pl_button",
					"type": "button",

					"x": 213 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Polski",

					"default_image": ROOT_PATH + "pl_button_01.sub",
					"over_image": ROOT_PATH + "pl_button_02.sub",
					"down_image": ROOT_PATH + "pl_button_03.sub",
				},
				{
					"name": "language_es_button",
					"type": "button",

					"x": 105 + 35,
					"y": 85,

					"vertical_align": "center",
					"tooltip_text": "Espanol",

					"default_image": ROOT_PATH + "es_button_01.sub",
					"over_image": ROOT_PATH + "es_button_02.sub",
					"down_image": ROOT_PATH + "es_button_03.sub",
				},
				
			),
		},
	),
}
