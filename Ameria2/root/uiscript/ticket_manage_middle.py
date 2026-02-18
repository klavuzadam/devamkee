# coding: latin_1

import grp

BOARD_WIDTH = 448
BOARD_HEIGHT = 153+88

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
                    "name" : "creator_date_value",
                    "type" : "text",
                    
                    "x" : 6,
                    "y" : 6,
                    "outline" : 1,
                    
                    "text" : "por User en 04/03/216 11:18",
                },
                {
                    "name" : "h_line",
                    "type" : "line",
                    
                    "x" : 6,
                    "y" : 23,
                    
                    "width" : 0,
                    "height" : 0,
                    
                    "color" : 0xff777777,
                },
                {
                    "name" : "msg",
                    "type" : "editline",
                    
                    "x" : 6,
                    "y" : 25,
                    
                    "text" : "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo",
                    
                    "width" : BOARD_WIDTH - 14,
                    "height" : 180,
                    
                    "input_limit" : 300,
                    "multi_line" : 1,
                    "limit_width" : BOARD_WIDTH - 14,
                },
                {
                    "name" : "scrollbar",
                    "type" : "scrollbar",
                    "horizontal_align" : "right",
                    
                    "x" : 19,
                    "y" : 81,
                    
                    "size" : 150,
                },
            ),
        },
    ),
}