# coding: latin_1

import app, localeInfo, grp

BOARD_WIDTH = 470
BOARD_HEIGHT = 405+88
TICKET_PATH = "locale/en/ui/tickets/"

window = {
    "name" : "TicketWindow",
    "style" : ("movable", "float", "animate",),
    
    "x" : 0,
    "y" : 0,
    
    "width" : BOARD_WIDTH,
    "height" : BOARD_HEIGHT,
    
    "children" :
    (
        {
            "name" : "board",
            "type" : "board_with_titlebar",
            "style" : ("attach",),
            
            "x" : 0,
            "y" : 0,
            
            "width" : BOARD_WIDTH,
            "height" : BOARD_HEIGHT,
            
            "title" : localeInfo.TICKET_TITLE,
            
            "children" :
            (
                {    
                    "name" : "btnpage1",
                    "type" : "radio_button",
                    
                    "x" : 10,
                    "y" : 35,
                    
                    "text" : localeInfo.TICKET_MY_TICKETS,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {    
                    "name" : "btnpage2",
                    "type" : "radio_button",
                    
                    "x" : 10+95,
                    "y" : 35,
                    
                    "text" : localeInfo.TICKET_ADMINISTRATION,
                    
                    "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                    "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                    "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                },
                {
                    "name" : "sort_by_text",
                    "type" : "text",
                    "horizontal_align" : "right",
                    "text_horizontal_align" : "right",
                    
                    "x" : 91,
                    "y" : 38,
                    
                    "text" : localeInfo.TICKET_SORT_BY,
                },
                {
                    "name" : "window1",
                    "type" : "window",
                    "horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : 62,
                    
                    "width" : BOARD_WIDTH - 18,
                    "height" : BOARD_HEIGHT - 72,
                    
                    "children" :
                    (
                        {
                            "name" : "my_tickets_title",
                            "type" : "horizontalbar",
                            
                            "x" : 0,
                            "y" : 0,
                            "width" : BOARD_WIDTH - 18,
                            
                            "children" :
                            (
                                {
                                    "name" : "my_tickets_id_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 24,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_ID,
                                },
                                {
                                    "name" : "my_tickets_title_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 140,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_TITLE_T,
                                },
                                {
                                    "name" : "my_tickets_priority_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 248,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_PRIORITY,
                                },
                                {
                                    "name" : "my_tickets_date_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 324,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_DATE,
                                },
                                {
                                    "name" : "my_tickets_state_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 390,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_STATE,
                                },
                            ),
                        },
                        {
                            "name" : "my_tickets_bg_listbox",
                            "type" : "bar",
                            
                            "x" : 0,
                            "y" : 17,
                            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                            
                            "width" : BOARD_WIDTH - 18,
                            "height" : 138+88,
                            
                            "children" :
                            (
                                {
                                    "name" : "my_tickets_listbox",
                                    "type" : "listboxex",
                                    
                                    "x" : 0,
                                    "y" : 3,
                                    
                                    "width" : BOARD_WIDTH - 18,
                                    "height" : 138+88,
                                    "viewcount" : 11,
                                },
                                {
                                    "name" : "my_tickets_scrollbar",
                                    "type" : "scrollbar",
                                    "horizontal_align" : "right",
                                    
                                    "x" : 17,
                                    "y" : 2,
                                    
                                    "size" : 135+88,
                                },
                            ),
                        },
                        {
                            "name" : "create_ticket_title",
                            "type" : "horizontalbar",
                            
                            "x" : 0,
                            "y" : 160+88,
                            "width" : BOARD_WIDTH - 18,
                            
                            "children" :
                            (
                                {
                                    "name" : "create_ticket_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 5,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_CREATE_NEW,
                                },
                            ),
                        },
                        {
                            "name" : "create_ticket_bg",
                            "type" : "bar",
                            
                            "x" : 0,
                            "y" : 177+88,
                            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                            
                            "width" : BOARD_WIDTH - 18,
                            "height" : 156,
                            
                            "children" :
                            (
                                {
                                    "name" : "create_ticket_title_slot",
                                    "type" : "slotbar",
                                    
                                    "x" : 50,
                                    "y" : 12,
                                    
                                    "width" : 360,
                                    "height" : 20,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "create_ticket_title_text",
                                            "type" : "text",
                                            "vertical_align" : "center",
                                            "text_vertical_align" : "center",
                                            
                                            "x" : -40,
                                            "y" : 0,
                                            
                                            "text" : localeInfo.TICKET_TITLE_T+":",
                                        },
                                        {
                                            "name" : "create_ticket_title_value",
                                            "type" : "editline",
                                            
                                            "x" : 3,
                                            "y" : 3,
                                            
                                            "width" : 360,
                                            "height" : 20,
                                            
                                            "input_limit" : 62,
                                        },
                                    ),
                                },
                                {
                                    "name" : "create_ticket_bg_msg",
                                    "type" : "bar",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 42,
                                    "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                                    
                                    "width" : BOARD_WIDTH - 40,
                                    "height" : 105,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "create_ticket_priority_text",
                                            "type" : "text",
                                            "horizontal_align" : "right",
                                            "text_horizontal_align" : "right",
                                            
                                            "x" : 72,
                                            "y" : 6,
                                            
                                            "text" : localeInfo.TICKET_PRIORITY+":",
                                        },
                                        {
                                            "name" : "create_ticket_priority_text",
                                            "type" : "text",
                                            "horizontal_align" : "right",
                                            "text_horizontal_align" : "right",
                                            
                                            "x" : 239,
                                            "y" : 6,
                                            
                                            "text" : localeInfo.TICKET_CATEGORY+":",
                                        },
                                    ),
                                },
                                {
                                    "name" : "create_ticket_bg_msg2",
                                    "type" : "bar",
                                    "horizontal_align" : "center",
                                    
                                    "x" : 0,
                                    "y" : 42+25,
                                    "color" : grp.GenerateColor(0.0, 0.0, 0.0, 1.0),
                                    
                                    "width" : BOARD_WIDTH - 50,
                                    "height" : 75,
                                    
                                    "children" :
                                    (
                                        {
                                            "name" : "create_ticket_msg",
                                            "type" : "editline",
                                            
                                            "x" : 3,
                                            "y" : 3,
                                            
                                            "width" : (BOARD_WIDTH - 50) - 110,
                                            "height" : 75 - 6,
                                            
                                            "input_limit" : 280,
                                            "multi_line" : 1,
                                            "limit_width" : (BOARD_WIDTH - 50) - 110,
                                        },
                                    ),
                                },
                            ),
                        },
                    ),
                },
                {
                    "name" : "window2",
                    "type" : "window",
                    "horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : 62,
                    
                    "width" : BOARD_WIDTH - 18,
                    "height" : BOARD_HEIGHT - 72,
                    
                    "children" :
                    (
                        {
                            "name" : "admin_title",
                            "type" : "horizontalbar",
                            
                            "x" : 0,
                            "y" : 0,
                            "width" : BOARD_WIDTH - 18,
                            
                            "children" :
                            (
                                {
                                    "name" : "admin_id_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 24,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_ID,
                                },
                                {
                                    "name" : "admin_title_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 140,
                                    "y" : 0,
                                    
                                    "text" : "Titulo",
                                },
                                {
                                    "name" : "admin_priority_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 248,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_PRIORITY,
                                },
                                {
                                    "name" : "admin_date_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 324,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_DATE,
                                },
                                {
                                    "name" : "admin_state_text",
                                    "type" : "text",
                                    "vertical_align" : "center",
                                    "text_vertical_align" : "center",
                                    
                                    "x" : 390,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_STATE,
                                },
                            ),
                        },
                        {
                            "name" : "admin_bg_listbox",
                            "type" : "bar",
                            
                            "x" : 0,
                            "y" : 17,
                            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                            
                            "width" : BOARD_WIDTH - 18,
                            "height" : 138+65,
                            
                            "children" :
                            (
                                {
                                    "name" : "admin_listbox",
                                    "type" : "listboxex",
                                    
                                    "x" : 0,
                                    "y" : 3,
                                    
                                    "width" : BOARD_WIDTH - 18,
                                    "height" : 138+65,
                                    "viewcount" : 10,
                                },
                                {
                                    "name" : "admin_scrollbar",
                                    "type" : "scrollbar",
                                    "horizontal_align" : "right",
                                    
                                    "x" : 17,
                                    "y" : 2,
                                    
                                    "size" : 135+65,
                                },
                            ),
                        },
                        {
                            "name" : "search_slot",
                            "type" : "slotbar",
                            "horizontal_align" : "center",
                            
                            "x" : -18,
                            "y" : 17+138+65+5,
                            
                            "width" : 80,
                            "height" : 18,
                            
                            "children" :
                            (
                                {
                                    "name" : "search_value",
                                    "type" : "editline",
                                    
                                    "x" : 3,
                                    "y" : 3,
                                    
                                    "width" : 80,
                                    "height" : 18,
                                    
                                    "input_limit" : 8,
                                },
                                {
                                    "name" : "search_text",
                                    "type" : "text",
                                    "horizontal_align" : "right",
                                    "text_horizontal_align" : "right",
                                    
                                    "x" : 85,
                                    "y" : 3,
                                    
                                    "text" : localeInfo.TICKET_SEARCH,
                                },
                            ),
                        },
                        {
                            "name" : "search_button",
                            "type" : "button",
                            "horizontal_align" : "center",
                    
                            "x" : 72,
                            "y" : 17+138+64+5,
                            
                            "text" : localeInfo.TICKET_SEARCH_BUTTON,
                            
                            "default_image" : "d:/ymir work/ui/public/large_button_01.sub",
                            "over_image" : "d:/ymir work/ui/public/large_button_02.sub",
                            "down_image" : "d:/ymir work/ui/public/large_button_03.sub",
                        },
                        {
                            "name" : "permisions_title",
                            "type" : "horizontalbar",
                            
                            "x" : 0,
                            "y" : 160+88,
                            "width" : BOARD_WIDTH - 18,
                            
                            "children" :
                            (
                                {
                                    "name" : "permisions_name_text",
                                    "type" : "text",
                                    "all_align" : 1,
                                    
                                    "x" : -138,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_NAME,
                                },
                                {
                                    "name" : "permisions_answers_text",
                                    "type" : "text",
                                    "all_align" : 1,
                                    
                                    "x" : -21,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_ANSWER,
                                },
                                {
                                    "name" : "permisions_delete_text",
                                    "type" : "text",
                                    "all_align" : 1,
                                    
                                    "x" : 54,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_DELETE,
                                },
                                {
                                    "name" : "permisions_add_text",
                                    "type" : "text",
                                    "all_align" : 1,
                                    
                                    "x" : 146,
                                    "y" : 0,
                                    
                                    "text" : localeInfo.TICKET_CHANGE_PERMISIONS,
                                },
                            ),
                        },
                        {
                            "name" : "permisions_bg",
                            "type" : "bar",
                            
                            "x" : 0,
                            "y" : 177+88,
                            "color" : grp.GenerateColor(0.0, 0.0, 0.0, 0.5),
                            
                            "width" : BOARD_WIDTH - 18,
                            "height" : 156,
                            
                            "children" :
                            (
                                {
                                    "name" : "permisions_scrollbar",
                                    "type" : "scrollbar",
                                    "horizontal_align" : "right",
                                    
                                    "x" : 17,
                                    "y" : 2,
                                    
                                    "size" : 153,
                                },
                                {
                                    "name" : "permisions_add_member",
                                    "type" : "button",
                                    
                                    "x" : 3,
                                    "y" : 3,
                                    
                                    "tooltip_text" : localeInfo.TICKET_ADD_MEMBER,
                                    
                                    "default_image" : TICKET_PATH + "add_1.tga",
                                    "over_image" : TICKET_PATH + "add_2.tga",
                                    "down_image" : TICKET_PATH + "add_3.tga",
                                },
                            ),
                        },
                    ),
                },
                {
                    "name" : "window3",
                    "type" : "window",
                    "horizontal_align" : "center",
                    
                    "x" : 0,
                    "y" : 62,
                    
                    "width" : BOARD_WIDTH - 18,
                    "height" : BOARD_HEIGHT - 72,
                },
            ),
        },
    ),
}