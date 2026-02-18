import os
import re

def translate_polish_terms(target_dir):
    encoding = 'cp1254'
    
    # Çeviri Sözlüğü (En spesifik olandan en genele doğru sıralandı)
    translations = {
        'Pas M?dro?ci': 'Bilgelik Kemeri',
        'Pas Nied?wiedzi': 'Ayı Kemeri',
        'Od?amek Energii': 'Enerji Parçası',
        'Kryszta? Energii': 'Enerji Kristali',
        'Receptura Eliksir Ksi??yca (M)': 'Ay İksiri Reçetesi (K)',
        'Receptura Eliksir Ksi??yca (?)': 'Ay İksiri Reçetesi (O)',
        'Receptura Eliksir Ksi??yca (D)': 'Ay İksiri Reçetesi (B)',
        'Receptura Eliksir S?o?ca (M)': 'Güneş İksiri Reçetesi (K)',
        'Receptura Eliksir S?o?ca (?)': 'Güneş İksiri Reçetesi (O)',
        'Receptura Eliksir S?o?ca (D)': 'Güneş İksiri Reçetesi (B)',
        'Receptura Br?zowy Wywar': 'Kahverengi İksir Reçetesi',
        'Receptura Be?owy Wywar': 'Bej İksir Reçetesi',
        'Mikstura Trwa?o?ci (D)': 'Dayanıklılık İksiri (B)',
        'Mikstura Regeneracji P?': 'HP Yenileme İksiri',
        'Mikstura Regeneracji PE': 'SP Yenileme İksiri',
        'Mikstura Pot?gi (M)': 'Güç İksiri (K)',
        'Mikstura Pot?gi (?)': 'Güç İksiri (O)',
        'Mikstura Pot?gi (D)': 'Güç İksiri (B)',
        'Mikstura ?uski Smoka (M)': 'Ejderha Pulu İksiri (K)',
        'Mikstura Pogromcy (D)': 'Fatih İksiri (B)',
        'Mikstura Determinacji': 'Kararlılık İksiri',
        'Mikstura Stagnacji': 'Duraklatma İksiri',
        'Mikstura Szybkiej Analizy': 'Hızlı Analiz İksiri',
        'Be?owy Sok': 'Bej Suyu',
        'Be?owa Woda': 'Bej Suyu', # Bazı dosyalarda farklı geçebilir
        'Be?owa Rosa': 'Bej Şebnemi',
        'Smocza Skrzynia+ (kol.)': 'Ejderha Sandığı+ (renk.)',
        'Smocza Skrzynia (kol.)': 'Ejderha Sandığı (renk.)',
        'Smocza Skrzynia+': 'Ejderha Sandığı+',
        'Smocza Skrzynia': 'Ejderha Sandığı',
        'Szlach. Smocza Skrzynia': 'Asil Ejderha Sandığı',
        'Kamie? Czerw. P?omienia': 'Kırmızı Alev Taşı',
        'Kamie? Tygrysa': 'Kaplan Taşı',
        'List Go?czy': 'Aranıyor İlanı',
        'Kronika Rzemie?lnikÃ³w': 'Zanaatkar Günlüğü',
        'Korze? Czerwonego Lasu': 'Kızıl Orman Kökü',
        'Cor Draconis (roz?arzone)': 'Cor Draconis (İşlenmiş)',
        'Cor Draconis+ (roz?arz.)': 'Cor Draconis+ (İşlenmiş)',
        'Cor Diamas (roz?arzone)': 'Cor Diamas (İşlenmiş)',
        'Cor Rubinum (roz?arzone)': 'Cor Rubinum (İşlenmiş)',
        'Cor Jadeit (roz?arzone)': 'Cor Jadeit (İşlenmiş)',
        'Cor Saphirum (roz?arzone)': 'Cor Saphirum (İşlenmiş)',
        'Cor Granatum (roz?arzone)': 'Cor Granatum (İşlenmiş)',
        'Cor Atrum (roz?arzone)': 'Cor Atrum (İşlenmiş)',
        'Cor Diamas+ (roz?arzone)': 'Cor Diamas+ (İşlenmiş)',
        'Cor Rubinum+ (roz?arzone)': 'Cor Rubinum+ (İşlenmiş)',
        'Cor Jadeit+ (roz?arzone)': 'Cor Jadeit+ (İşlenmiş)',
        'Cor Saphirum+ (roz?arz.)': 'Cor Saphirum+ (İşlenmiş)',
        'Cor Granatum+ (roz?arz.)': 'Cor Granatum+ (İşlenmiş)',
        'Cor Atrum+ (roz?arzone)': 'Cor Atrum+ (İşlenmiş)',
        'Ci?gliwo??': 'Süreklilik',
        'Dobra Wytrzyma?o??': 'Yüksek Dayanıklılık',
        'S?aba Zbroja': 'Zayıf Zırh',
        '?o?nierze': 'Askerler',
        'Pierwszorz?dne Buty': 'Birinci Sınıf Ayakkabı',
        'Wytrzyma?o?? Wroga': 'Düşman Dayanıklılığı',
        'Inna Wroga Wytrzyma?o??': 'Diğer Düşman Dayanıklılığı'
    }

    files_to_update = [
        'item_names_farklı.txt',
        'yeni.txt',
        'yeni - Kopya.txt'
    ]

    for filename in files_to_update:
        file_path = os.path.join(target_dir, filename)
        if not os.path.exists(file_path):
            continue

        print(f"İşleniyor: {filename}")
        
        # Dosyayı oku (cp1254)
        with open(file_path, 'r', encoding=encoding, errors='ignore') as f:
            lines = f.readlines()

        new_lines = []
        for line in lines:
            # Satırdaki Lehçe terimleri çevir
            for polish, turkish in translations.items():
                # Metnin içinde varsa değiştir
                if polish in line:
                    line = line.replace(polish, turkish)
            
            new_lines.append(line)

        # Dosyaya yaz
        with open(file_path, 'w', encoding=encoding) as f:
            f.writelines(new_lines)

    print("Çeviri işlemi tüm dosyalarda başarıyla tamamlandı.")

if __name__ == "__main__":
    dir_path = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme'
    translate_polish_terms(dir_path)
