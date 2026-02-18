# coding: latin_1

import grp

BOARD_WIDTH = 410
BOARD_HEIGHT = 73

window = {
    "name" : "TicketManageAnswer",
    
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
                    "name" : "creator_date_value",
                    "type" : "text",
                    
                    "x" : 6,
                    "y" : 2,
                    "outline" : 1,
                    
                    "text" : "por User en 04/03/216 11:18",
                },
                {
                    "name" : "h_line",
                    "type" : "line",
                    
                    "x" : 5,
                    "y" : 19,
                    
                    "width" : 0,
                    "height" : 0,
                    
                    "color" : 0xff777777,
                },
                {
                    "name" : "msg",
                    "type" : "editline",
                    
                    "x" : 6,
                    "y" : 21,
                    
                    "text" : "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
                    
                    "width" : BOARD_WIDTH - 12,
                    "height" : 180,
                    
                    "input_limit" : BOARD_WIDTH - 12,
                    "multi_line" : 1,
                    "limit_width" : BOARD_WIDTH - 12,
                },
            ),
        },
    ),
}