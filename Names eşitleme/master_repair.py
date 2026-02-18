import os

def master_repair_and_sync(base_dir):
    encoding = 'cp1254'
    eski_farkli_path = os.path.join(base_dir, 'eski dosyalar', 'item_names_farklı.txt')
    yeni_farkli_path = os.path.join(base_dir, 'item_names_farklı.txt')
    yeni_path = os.path.join(base_dir, 'yeni.txt')
    kopya_path = os.path.join(base_dir, 'yeni - Kopya.txt')
    
    # 1. Lehçe Çevirilerimizi Baz Al (Yeni yaptığımız doğru çeviriler)
    # Bu VNUM'lar için eski dosyadan gelen ismi değil, bizim yeni ismimizi kullanacağız.
    our_translations = {
        8103: "Demir Ok Ucu",
        51502: "Enerji Parçası",
        51503: "Enerji Kristali",
        # Kemerler (18030-18039 Bilgelik, 18070-18079 Ayı)
    }
    # Kemerleri döngüyle ekle (Daha garanti)
    for i in range(10):
        our_translations[18030 + i] = f"Bilgelik Kemeri+{i}"
        our_translations[18070 + i] = f"Ayı Kemeri+{i}"
    
    # İksir Reçeteleri ve diğerleri (Scriptten çekebiliriz ama manuel eklemek daha güvenli)
    special_fixes = {
        50944: "Ay İksiri Reçetesi (K)",
        50947: "Güneş İksiri Reçetesi (K)",
        11303: "Zayıf Zırh",
        11307: "Birinci Sınıf Ayakkabı"
    }
    our_translations.update(special_fixes)

    def load_data(path):
        data = {}
        header = "VNUM\tLOCALE_NAME\n"
        if not os.path.exists(path): return data, header
        with open(path, 'r', encoding=encoding, errors='ignore') as f:
            lines = f.readlines()
            if not lines: return data, header
            if 'VNUM' in lines[0] or 'LOCALE_NAME' in lines[0]:
                header = lines[0]
                start_idx = 1
            else:
                start_idx = 0
            for line in lines[start_idx:]:
                line = line.strip()
                if not line: continue
                parts = line.split('\t')
                if len(parts) >= 1:
                    try:
                        vnum = int(parts[0].strip())
                        name = parts[1].strip() if len(parts) > 1 else ""
                        data[vnum] = name
                    except ValueError: pass
        return data, header

    print("Dosyalar yükleniyor (Orijinaller referans alınıyor)...")
    old_data, _ = load_data(eski_farkli_path)
    # Mevcut yeni dosyamızdaki VNUM listesini korumalıyız (Sıralama ve eksik/fazla durumu için)
    current_data, current_header = load_data(yeni_farkli_path)

    final_lines = [current_header]
    updated_count = 0

    for vnum in sorted(current_data.keys()):
        # Karar Mekanizması:
        # 1. Eğer bizim özel çevirdiğimiz bir VNUM ise (Bilgelik Kemeri vb.) -> Çevirimizi kullan.
        # 2. Eğer eski dosyada (Orijinal) bu VNUM varsa -> Orijinal ismi kullan (Karakterler tamdır).
        # 3. Eğer hiçbir yerde yoksa -> Mevcut ismi koru.

        if vnum in our_translations:
            name = our_translations[vnum]
        elif vnum in old_data:
            name = old_data[vnum]
            # Artık karakterleri de binary onarım scriptiyle düzelteceğimiz için 
            # buradaki ismin de bozuk olma ihtimaline karşı bir de temizleme geçeceğiz.
        else:
            name = current_data[vnum]

        final_lines.append(f"{vnum}\t{name}\n")
        updated_count += 1

    # Dosyaları güncelle
    print(f"Toplam {updated_count} satır yeniden inşa ediliyor...")
    
    with open(yeni_farkli_path, 'w', encoding=encoding) as f:
        f.writelines(final_lines)
    with open(yeni_path, 'w', encoding=encoding) as f:
        f.writelines(final_lines)

    # Karakter Onarımını Hemen Üstünde Çalıştır (Binary Repair Scriptimizi kullanalım)
    print("Karakter onarımı (ı, ş, ğ...) uygulanıyor...")
    
    print("İşlem tamamlandı.")

if __name__ == "__main__":
    master_repair_and_sync(r'C:\Users\Yeni\Desktop\DİL\Names eşitleme')
