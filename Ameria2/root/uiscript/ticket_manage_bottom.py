# coding: latin_1

import grp, app

BOARD_WIDTH = 448
BOARD_HEIGHT = 102
TICKET_PATH = "locale/en/ui/tickets/"

window = {
    "name" : "TicketManageMiddle",
    
    "x" : 0,
    "y" : 0,
    
    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGHT,
    
    "children" :
    (
        {
            "name" : "board",
            "type" : "bar",
            
            "x" : 0,
            "y" : 0,
            
            "width" : BOARD_WIDTH,
            "height" : BOARD_HEIGHT,
            
            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.6),
            
            "children" :
            (
                {
                    "name" : "msg_bar",
                    "type" : "bar",
                    "horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : 5,
                    
                    "width" : BOARD_WIDTH - 10,
                    "height" : BOARD_HEIGHT - 32,
                    
                    "color" : grp.GenerateColor(0.0, 0.0, 0.0, 1.0),
                    
                    "children" :
                    (
                        {
                            "name" : "msg",
                            "type" : "editline",
                            
                            "x" : 3,
                            "y" : 3,
                            
                            "width" : BOARD_WIDTH - 10,
                            "height" : BOARD_HEIGHT - 6,
                            
                            "input_limit" : 300,
                            "multi_line" : 1,
                            "limit_width" : BOARD_WIDTH - 10,
                        },
                    ),
                },
                {
                    "name" : "send_button",
                    "type" : "button",
                    
                    "x" : BOARD_WIDTH - 95,
                    "y" : BOARD_HEIGHT - 24,
                    
                    "text" : "Responder",
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "state_1_button",
                    "type" : "radio_button",
                    
                    "x" : 5,
                    "y" : BOARD_HEIGHT - 22,
                    
                    "default_image" : TICKET_PATH + "icon_exp_empty.tga",
                    "over_image" : TICKET_PATH + "icon_exp_full.tga",
                    "down_image" : TICKET_PATH + "icon_exp_full.tga",
                    
                    "children" :
                    (
                        {
                            "name" : "state_1_text",
                            "type" : "text",
                            
                            "x" : 20,
                            "y" : 2,
                            
                            "text" : "En proceso",
                        },
                    ),
                },
                {
                    "name" : "state_2_button",
                    "type" : "radio_button",
                    
                    "x" : 85,
                    "y" : BOARD_HEIGHT - 22,
                    
                    "default_image" : TICKET_PATH + "icon_exp_empty.tga",
                    "over_image" : TICKET_PATH + "icon_exp_full.tga",
                    "down_image" : TICKET_PATH + "icon_exp_full.tga",
                    
                    "children" :
                    (
                        {
                            "name" : "state_2_text",
                            "type" : "text",
                            
                            "x" : 20,
                            "y" : 2,
                            
                            "text" : "Cerrado",
                        },
                    ),
                },
                {
                    "name" : "state_3_button",
                    "type" : "radio_button",
                    
                    "x" : 85+66,
                    "y" : BOARD_HEIGHT - 22,
                    
                    "default_image" : TICKET_PATH + "icon_exp_empty.tga",
                    "over_image" : TICKET_PATH + "icon_exp_full.tga",
                    "down_image" : TICKET_PATH + "icon_exp_full.tga",
                    
                    "children" :
                    (
                        {
                            "name" : "state_3_text",
                            "type" : "text",
                            
                            "x" : 20,
                            "y" : 2,
                            
                            "text" : "Solucionado",
                        },
                    ),
                },
            ),
        },
    ),
}