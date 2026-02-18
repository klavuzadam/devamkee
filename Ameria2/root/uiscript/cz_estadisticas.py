# coding: latin_1

import uiScriptLocale

ROOT = "combat_zone_estadisticas/"

window = {
    "name" : "CombatZoneEstadisticas",
    
    "x" : SCREEN_WIDTH - 136 - 150 - 120 + 70,
    "y" : 15,
    
    "width" : 156,
    "height" : 77+25,
    
    "children" :
    (
        {
            "name" : "estadisticas_windows",
            "type" : "image",
            
            "x" : 0,
            "y" : 0,
            
            "image": ROOT+"Base.tga",
            
            "children":
            (
                
                {"name" : "puntos_title","type": "text","x" : 28,"y" :7,"text": "Puntos:",},
                {"name" : "bonus_title","type": "text","x" : 28,"y" :7+23,"text": "Bonus:",},
                {"name" : "tiempo_title","type": "text","x" : 28,"y" :7+(23*2),"text": "Tiempo:",},


                {"name" : "puntos","type": "text","x" : 121,"y" :7,"text": "222222220","text_horizontal_align":"center",},
                {"name" : "bonus","type": "text","x" : 121,"y" :7+23,"text": "222222220","text_horizontal_align":"center",},
                {"name" : "tiempo","type": "text","x" : 121,"y" :7+(23*2),"text": "222222220","text_horizontal_align":"center",},

            ),
        },    

        {
            "name" : "exit_button",
            "type" : "button",
                    
            "x" : 5,
            "y" : 80,
                    
            "default_image" : ROOT + "salir_1.tga",
            "over_image" : ROOT + "salir_2.tga",
            "down_image" : ROOT + "salir_3.tga",

            "text": "Abandonar Total War",
        },    
    ),    
}
