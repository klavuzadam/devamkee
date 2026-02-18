import localeInfo

ROOT_PATH = "d:/ymir work/ui/game/battle_pass/"

WINDOW_WIDTH = 470
WINDOW_HEIGHT = 406 - 110

MOVING_POS = 2
MOVING_POS_DOWN = 50
MOVING_POS_UP = 65

window = {
	"name" : "BattlePassWindow",
	"style" : ("movable", "float",),

	"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2 - 125,
	"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
	
	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT + 36,
	
	"children" :
	(
        
		{
			"name" : "background",
			"type" : "image",
			
			"x" : 4,
			"y" : 3,
			
			"image" : ROOT_PATH + "board.png",
		},

		{
			"name" : "board_01",
			"type" : "window",
			
			"x" : 3,
			"y" : 3,

			"width" : 470,
			"height" : 406 - 110,

			"children" :
			(
				{
					"name" : "background",
					"type" : "image",
					
					"x" : 8,
					"y" : 32,
					
					"image" : ROOT_PATH + "inner_1.png",
                    
					"children": 
                    (
                        
						{"name":"bg_free_01","type":"image","x":92 + MOVING_POS,"y":65,"image":ROOT_PATH+"main/slot_free.png","children":[{"name":"slot_free_01","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_free_01","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_free_02","type":"image","x":92 + 58 + MOVING_POS,"y":65,"image":ROOT_PATH+"main/slot_free.png","children":[{"name":"slot_free_02","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_free_02","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_free_03","type":"image","x":92 + 58 * 2 + MOVING_POS,"y":65,"image":ROOT_PATH+"main/slot_free.png","children":[{"name":"slot_free_03","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_free_03","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_free_04","type":"image","x":92 + 58 * 3 + MOVING_POS,"y":65,"image":ROOT_PATH+"main/slot_free.png","children":[{"name":"slot_free_04","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_free_04","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_free_05","type":"image","x":92 + 58 * 4 + MOVING_POS,"y":65,"image":ROOT_PATH+"main/slot_free.png","children":[{"name":"slot_free_05","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_free_05","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_free_06","type":"image","x":92 + 58 * 5 + MOVING_POS,"y":65,"image":ROOT_PATH+"main/slot_free.png","children":[{"name":"slot_free_06","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_free_06","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
                        
						{"name":"bg_premium_01","type":"image","x":92 + MOVING_POS,"y":150,"image":ROOT_PATH+"main/slot_premium.png","children":[{"name":"slot_premium_01","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_premium_01","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_premium_02","type":"image","x":92 + 58 + MOVING_POS,"y":150,"image":ROOT_PATH+"main/slot_premium.png","children":[{"name":"slot_premium_02","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_premium_02","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_premium_03","type":"image","x":92 + 58 * 2 + MOVING_POS,"y":150,"image":ROOT_PATH+"main/slot_premium.png","children":[{"name":"slot_premium_03","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_premium_03","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_premium_04","type":"image","x":92 + 58 * 3 + MOVING_POS,"y":150,"image":ROOT_PATH+"main/slot_premium.png","children":[{"name":"slot_premium_04","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_premium_04","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_premium_05","type":"image","x":92 + 58 * 4 + MOVING_POS,"y":150,"image":ROOT_PATH+"main/slot_premium.png","children":[{"name":"slot_premium_05","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_premium_05","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},
						{"name":"bg_premium_06","type":"image","x":92 + 58 * 5 + MOVING_POS,"y":150,"image":ROOT_PATH+"main/slot_premium.png","children":[{"name":"slot_premium_06","type":"slot","x":12,"y":5,"width":32,"height":32,"slot":[{"index":0,"x":0,"y":0,"width":32,"height":32}]}, {"name": "status_premium_06","type": "image","x": 20,"y": 38,	"image" : ROOT_PATH + "main/locked.png",},]},


						{
							"name" : "ExpGauge",
							"type" : "expanded_image",

							"x" : 92 - 11,
							"y" : 73 - 35,

							"image" : ROOT_PATH + "fill_experience.png",
						},

						{
							"name" : "ExpGaugeBG",
							"type" : "expanded_image",

							"x" : 15 - 11,
							"y" : 70 - 35,

							"image" : ROOT_PATH + "exp_img.png",
						},

						{
							"name" : "page_0",
							"type" : "toggle_button",
							"x" : 100 + MOVING_POS_DOWN,
							"y" : 168 + MOVING_POS_UP,

							"default_image" : ROOT_PATH+"main/button_over.png",
							"over_image" : ROOT_PATH+"main/button_normal.png",
							"down_image" : ROOT_PATH+"main/button_selected.png",
                            
							"children":
							(
								{
									"name" : "text_0",
									"type" : "text",

									"x" : 10,
									"y" : 4,

									"text": "1",
                                    
									"outline" : 1,

								},                               
							),							

						},
                        
						{
							"name" : "page_1",
							"type" : "toggle_button",
							"x" : 130 + MOVING_POS_DOWN,
							"y" : 168 + MOVING_POS_UP,

							"default_image" : ROOT_PATH+"main/button_over.png",
							"over_image" : ROOT_PATH+"main/button_normal.png",
							"down_image" : ROOT_PATH+"main/button_selected.png",
                            
							"children":
							(
								{
									"name" : "text_1",
									"type" : "text",

									"x" : 10,
									"y" : 4,

									"text": "2",
                                    
									"outline" : 1,

								},                               
							),	

						},

						{
							"name" : "page_2",
							"type" : "toggle_button",
							"x" : 160 + MOVING_POS_DOWN,
							"y" : 168 + MOVING_POS_UP,
                            
							"default_image" : ROOT_PATH+"main/button_over.png",
							"over_image" : ROOT_PATH+"main/button_normal.png",
							"down_image" : ROOT_PATH+"main/button_selected.png",
                            
							"children":
							(
								{
									"name" : "text_2",
									"type" : "text",

									"x" : 10,
									"y" : 4,

									"text": "3",
                                    
									"outline" : 1,

								},                               
							),
						},

						{
							"name" : "page_3",
							"type" : "toggle_button",
							"x" : 190 + MOVING_POS_DOWN,
							"y" : 168 + MOVING_POS_UP,
                            
							"default_image" : ROOT_PATH+"main/button_over.png",
							"over_image" : ROOT_PATH+"main/button_normal.png",
							"down_image" : ROOT_PATH+"main/button_selected.png",
                            
							"children":
							(
								{
									"name" : "text_3",
									"type" : "text",

									"x" : 10,
									"y" : 4,

									"text": "4",
                                    
									"outline" : 1,

								},                               
							),
						},


						{
							"name" : "page_4",
							"type" : "toggle_button",
							"x" : 220 + MOVING_POS_DOWN,
							"y" : 168 + MOVING_POS_UP,
                            
							"default_image" : ROOT_PATH+"main/button_over.png",
							"over_image" : ROOT_PATH+"main/button_normal.png",
							"down_image" : ROOT_PATH+"main/button_selected.png",
                            
							"children":
							(
								{
									"name" : "text_4",
									"type" : "text",

									"x" : 10,
									"y" : 4,

									"text": "5",
                                    
									"outline" : 1,

								},                               
							),
						},

						{
							"name" : "content_locked",
							"type" : "image",
							
							"x" : 82,
							"y" : 135,
							
							"image" : ROOT_PATH + "content_blocked.png",
                            
							"children":
							(
								{
									"name" : "ContentButton",
									"type" : "button",

									"x" : 0,
									"y" : 60,
									"horizontal_align" : "center",

									"default_image" : ROOT_PATH + "content_blocked_normal.png",
									"over_image" : ROOT_PATH + "content_blocked_normal.png",
									"down_image" : ROOT_PATH + "content_blocked_normal.png",
                                    
									"children":
									(
										{
											"name" : "content",
											"type" : "text",

											"x" : 16,
											"y" : 5,

											"text": "View Content",
											"outline" : 1,

										},                               
									),

								},
							),
						},
					),

				}, 

			),            
		},

		{
			"name" : "board_02",
			"type" : "window",
			
			"x" : 3,
			"y" : 3,

			"width" : 470,
			"height" : 406 - 110,

			"children" :
			(
				
				{
					"name" : "background",
					"type" : "image",
					
					"x" : 8,
					"y" : 65,
					
					"image" : ROOT_PATH + "inner_2.png",
				},

				{
					"name" : "background_set",
					"type" : "image",
					
					"x" : 8,
					"y" : 32,
					
					"image" : ROOT_PATH + "top.png",
				},

				{
					"name" : "window_daily",
					"type" : "window",

					"width": 220,
					"height": 200,
					"x" : 10,
					"y" : 90,
				},

				{
					"name" : "window_weekly",
					"type" : "window",

					"width": 220,
					"height": 200,
					"x" : 237,
					"y" : 90,
				},

				{
					"name": "ItemScrollbardaily",
					"type": "modern_scrollbar",
					
					"x": 221,
					"y": 92,
					
					"width": 6,
					"size": 193,
					"content_height": 358 - 112 + 5,
				},	

				{
					"name": "ItemScrollbarweekly",
					"type": "modern_scrollbar",
					
					"x": 448,
					"y": 92,
					
					"width": 6,
					"size": 193,
					"content_height": 150,
				},	

				{
					"name" : "daily",
					"type" : "text",

					"x" : 115,
					"y" : 70,

					"text": "Daily Missions",
					
					"color" : 0xFFb8e8e8e,
					"outline" : 1,
                    
					"fontsize" : "LARGE",
					
					"text_horizontal_align" : "center",
				}, 

				{
					"name" : "weekly",
					"type" : "text",

					"x" : 342,
					"y" : 70,

					"text": "Weekly Missions",
					
					"color" : 0xFFb8e8e8e,
					"outline" : 1,
                    
					"fontsize" : "LARGE",
					
					"text_horizontal_align" : "center",
				}, 

			),            
		},

		{
			"name" : "board_03",
			"type" : "window",
			
			"x" : 3,
			"y" : 3,

			"width" : 470,
			"height" : 406 - 110,

			"children" :
			(
				
				{
					"name" : "background",
					"type" : "image",
					
					"x" : 8,
					"y" : 65,
					
					"image" : ROOT_PATH + "inner_3.png",
				},

			),            
		},

		{
			"name" : "TitleBar",
			"type" : "titlebar",
			"style" : ("attach",),

			"x" : 8,
			"y" : 9,

			"width" : WINDOW_WIDTH - 15,
			"children" :
			(
				{ "name" : "TitleName", "type" : "text", "x" : WINDOW_WIDTH / 2, "y" : 3, "text" : "BattlePass", "text_horizontal_align" : "center" },
			),
		},    

		{
			"name" : "BpInfo",
			"type" : "image",

			"x" : WINDOW_WIDTH - 45,
			"y" : 11,
			"image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
			"children":
			(
				{
					"name":"BpInfoText",
					"type":"text",
					"x":0,
					"y":0,
					"all_align":"center",
				},
			),
		},

		{
			"name" : "battlePoints",
			"type" : "expanded_image",
			
			"x" : 124,
			"y" : 43,
			
			"image" : ROOT_PATH + "fill_battle.png",

			"children" :
			(
				{
					"name" : "timeGauge",
					"type" : "expanded_image",

					"x" : 167,
					"y" : 0,

					"image" : ROOT_PATH + "fill_time.png",
				},

				{
					"name" : "timeGaugeBG",
					"type" : "expanded_image",
                    
					"x" : 156,
					"y" : -6,
                    
					"image" : ROOT_PATH + "time_img.png",
				},

				{
					"name" : "premiumPoints",
					"type" : "text",

					"x" : 28,
					"y" : -1,

					"text": "30",

					"outline" : 1,
					
					"text_horizontal_align" : "center",
				},  

				{
					"name" : "expPoints",
					"type" : "text",

					"x" : 111,
					"y" : -1,

					"text": "5 / 5",

					"outline" : 1,
					
					"text_horizontal_align" : "center",
				}, 

				{
					"name" : "battlePassEnd",
					"type" : "text",

					"x" : 250,
					"y" : -1,

					"text": "Ends in: 30Days 23Hours 59Min 59Sec",

					"outline" : 1,
					
					"text_horizontal_align" : "center",
				},

			),            
		},

		{
			"name" : "Premium", "type" : "window", "x" : 13, "y":37, "width" : 89, "height" : 26, "style" : ("attach",),
			"children" :
			(
				{
					"name" : "LifeGauge",
					"type" : "ani_image",

					"x" : 0,
					"y" : 0,

					"delay" : 3,

					"images" :
					(
						ROOT_PATH + "premium_btn/premium_0.png",
						ROOT_PATH + "premium_btn/premium_1.png",
						ROOT_PATH + "premium_btn/premium_2.png",
						ROOT_PATH + "premium_btn/premium_3.png",
						ROOT_PATH + "premium_btn/premium_4.png",
						ROOT_PATH + "premium_btn/premium_5.png",
						ROOT_PATH + "premium_btn/premium_6.png",
						ROOT_PATH + "premium_btn/premium_7.png",
						ROOT_PATH + "premium_btn/premium_8.png",
						ROOT_PATH + "premium_btn/premium_9.png",
						ROOT_PATH + "premium_btn/premium_10.png",
						ROOT_PATH + "premium_btn/premium_11.png",
						ROOT_PATH + "premium_btn/premium_12.png",
						ROOT_PATH + "premium_btn/premium_13.png",
						ROOT_PATH + "premium_btn/premium_14.png",
						ROOT_PATH + "premium_btn/premium_15.png",
						ROOT_PATH + "premium_btn/premium_16.png",
						ROOT_PATH + "premium_btn/premium_17.png",
						ROOT_PATH + "premium_btn/premium_18.png",
						ROOT_PATH + "premium_btn/premium_19.png",
						ROOT_PATH + "premium_btn/premium_20.png",
					),
				},
			),
		},

		{
			"name" : "ContentButtonUnlock",
			"type" : "button",

			"x" : 13,
			"y" : 37,

			"default_image" : ROOT_PATH + "free_btn_normal.png",
			"over_image" : ROOT_PATH + "free_btn_over.png",
			"down_image" : ROOT_PATH + "free_btn_down.png",

		},

		{"name": "board_01_window", "type": "image", "x": 4, "y": 293, "image" : ROOT_PATH + "tab_1.png",},
		{"name": "board_02_window", "type": "image", "x": 4, "y": 293, "image" : ROOT_PATH + "tab_2.png",},
		{"name": "board_03_window", "type": "image", "x": 4, "y": 293, "image" : ROOT_PATH + "tab_3.png",},

		{"name": "Tab_Button_01", "type": "radio_button", "x": 12 + 5, "y": 299, "width": 45, "height": 23},
		{"name": "Tab_Button_02", "type": "radio_button", "x": 76 + 5, "y": 299, "width": 45, "height": 23},
		{"name": "Tab_Button_03", "type": "radio_button", "x": 405, "y": 299, "width": 45, "height": 23},

	),	
    
}
