# coding: latin_1

import grp

BOARD_WIDTH = 448
BOARD_HEIGHT = 70

window = {
    "name" : "TicketManageTop",
    "style" : ("attach",),
    
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
                    "name" : "state_bar",
                    "type" : "bar",
                    
                    "x" : 5,
                    "y" : 5,
                    
                    "width" : 75,
                    "height" : 22,
                    
                    "color" : grp.GenerateColor(0.0, 1.0, 0.0, 0.4),
                    
                    "children" :
                    (
                        {
                            "name" : "state_value",
                            "type" : "text",
                            "all_align" : 1,
                            "fontname" : "Tahoma:14",
                            "outline" : 1,
                            
                            "x" : 0,
                            "y" : -1,
                            
                            "text" : "Solucionado",
                        },
                        {
                            "name" : "title_value",
                            "type" : "text",
                            "vertical_align" : "center",
                            "text_vertical_align" : "center",
                            "fontname" : "Tahoma:14",
                            "outline" : 1,
                            
                            "x" : 86,
                            "y" : 0,
                            
                            "text" : "Titulo test bla bla bla bla bla",
                        },
                    ),
                },    
                {
                    "name" : "h_line",
                    "type" : "line",
                    "horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : 31,
                    
                    "width" : BOARD_WIDTH - 8,
                    "height" : 0,
                    
                    "color" : 0xff777777,
                },
                {
                    "name" : "ticket_id_text",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : -180,
                    "y" : 35,
                    
                    "text" : "ID de Ticket",
                },
                {
                    "name" : "ticket_id_value",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : -179,
                    "y" : 51,
                    
                    "text" : "AAAA0001",
                },
                {
                    "name" : "v_line_1",
                    "type" : "line",
                    
                    "x" : 95,
                    "y" : 31+5,
                    
                    "width" : 0,
                    "height" : 30,
                    
                    "color" : 0xff777777,
                },
                {
                    "name" : "ticket_priority_text",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : -83,
                    "y" : 35,
                    
                    "text" : "Prioridad",
                },
                {
                    "name" : "ticket_priority_value",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : -82,
                    "y" : 51,
                    
                    "text" : "Alta",
                },
                {
                    "name" : "v_line_2",
                    "type" : "line",
                    
                    "x" : 95*2,
                    "y" : 31+5,
                    
                    "width" : 0,
                    "height" : 30,
                    
                    "color" : 0xff777777,
                },
                {
                    "name" : "ticket_date_text",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : 13,
                    "y" : 35,
                    
                    "text" : "Fecha",
                },
                {
                    "name" : "ticket_date_value",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : 14,
                    "y" : 51,
                    
                    "text" : "03/03/2016",
                },
                {
                    "name" : "v_line_3",
                    "type" : "line",
                    
                    "x" : 95*3,
                    "y" : 31+5,
                    
                    "width" : 0,
                    "height" : 30,
                    
                    "color" : 0xff777777,
                },
                {
                    "name" : "ticket_answers_text",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : 145,
                    "y" : 35,
                    
                    "text" : "Respuestas",
                },
                {
                    "name" : "ticket_answers_value",
                    "type" : "text",
                    "horizontal_align" : "center",
                    "text_horizontal_align" : "center",
                    
                    "x" : 146,
                    "y" : 51,
                    
                    "text" : "100",
                },
            ),
        },
    ),
}