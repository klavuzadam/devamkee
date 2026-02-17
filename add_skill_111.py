# -*- coding: utf-8 -*-

def add_skill_111():
    path = r"Client\pack\locale\locale\tr\skilldesc.txt"
    try:
        with open(path, 'r', encoding='cp1254', errors='ignore') as f:
            lines = [line.rstrip('\r\n') for line in f]
    except FileNotFoundError:
        print("File not found.")
        return

    # Skill 111: Attack Up (Yüksek Saldırı)
    # Polish/Mixed Attributes -> Turkish:
    # "Si³a Ataku" -> "Saldırı Gücü"
    # "Magiczne Obra¿enia (ogólne)" -> "Büyülü Hasar"
    # "Magiczne Obra¿enia w potwory" -> "Canavar Büyülü Hasar Artışı"
    # "Premia: Magiczne obra¿enia w potwory" -> "Canavarlara Karşı Büyülü Hasar Bonusu"
    # Desc: "Zwiêksza Si³ê Ataku" -> "Saldırı Gücünü Artırır"
    # "Mo¿liwoœæ u¿ycia..." -> "Grup Üyesinde Kullanılabilir"
    # "Premia magicznego..." -> "Kendine Kullanıldığında Büyü Bonusu"
    
    skill_111 = "111	SHAMAN	Yüksek Saldırı	Güç Artışı	Tanrısal Güç	Saldırı gücünü artırır.	Saldırı Gücünü Artırır	Grup Üyesinde Kullanılabilir	Kendine Kullanıldığında Büyü Bonusu Vurur		CAN_USE_FOR_ME|NEED_TARGET|ONLY_FOR_ALLIANCE		jeungryeok	21	4			Saldırı Gücü: +%.0f	10+(iq*0.65 +20)*k	 	Büyülü Hasar (Genel) +%.0f%%	8*k		Canavar Büyülü Hasar Artışı +%.0f%%	(iq*0.3*(2*k+0.35)/(k+2))-8*k		Bonus: Canavarlara Büyülü Hasar +%.0f	(65+iq*4+(minmwep+maxmwep)/2*2.5)*k+750*lv/(lv+60)-30"
    
    final_lines = []
    inserted = False
    
    # Check if 111 exists
    for line in lines:
        if line.startswith('111\t'):
            print("Skill 111 already exists. Skipping.")
            return

    for line in lines:
        parts = line.split('\t')
        if parts[0].isdigit():
            current_id = int(parts[0])
            if current_id > 111 and not inserted:
                print("Inserting skill 111 before ID", current_id)
                final_lines.append(skill_111)
                inserted = True
        
        final_lines.append(line)

    if not inserted:
        final_lines.append(skill_111)

    with open(path, 'w', encoding='cp1254', newline='\r\n') as f:
        for line in final_lines:
            f.write(line + "\r\n")
            
    print("Done.")

if __name__ == "__main__":
    add_skill_111()
