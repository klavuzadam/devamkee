import os

def generate_broken_report(base_dir):
    encoding = 'cp1254'
    eski_path = os.path.join(base_dir, 'eski dosyalar', 'item_names_farklı.txt')
    yeni_path = os.path.join(base_dir, 'item_names_farklı.txt')
    
    def load_raw(path):
        data = {}
        with open(path, 'rb') as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith(b'VNUM'): continue
                parts = line.split(b'\t')
                if len(parts) >= 1:
                    try:
                        vnum = int(parts[0].strip())
                        name = parts[1].strip() if len(parts) > 1 else b""
                        data[vnum] = name
                    except ValueError: pass
        return data

    old_data = load_raw(eski_path)
    new_data = load_raw(yeni_path)
    
    report = []
    
    # Lehçe çeviride kullandığımız anahtar kelimeler (hariç tutmak için)
    ignore_turkish = ['Bilgelik Kemeri', 'Ayı Kemeri', 'Enerji Parçası', 'Enerji Kristali', 'İksir Reçetesi', 'Demir Ok Ucu']

    for vnum in sorted(new_data.keys()):
        if vnum in old_data:
            old_raw = old_data[vnum]
            new_raw = new_data[vnum]
            
            # Eski ismide düzgün karakterler (\xfd vb) var ama yenisinde yoksa
            # VEYA karakter eksilmişse
            if old_raw != new_raw:
                try:
                    # Eski ismi düzgünce decode edelim (cp1254)
                    old_name = old_raw.decode('cp1254', errors='ignore')
                    new_name = new_raw.decode('cp1254', errors='ignore')
                    
                    # Eğer yeni isim eskisinden kısaysa ve bir Lehçe çeviri değilse
                    if len(new_name) < len(old_name):
                        # Bizim çevirilerimizi hariç tut
                        is_translation = any(t in new_name for t in ignore_turkish)
                        if not is_translation:
                            report.append(f"VNUM: {vnum} | Doğru: {old_name} | Hatalı: {new_name}")
                except:
                    pass

    # Raporu yaz
    report_file = os.path.join(base_dir, 'broken_items_report.txt')
    with open(report_file, 'w', encoding='utf-8') as f:
        f.write("ONAYIN İÇİN BOZUK İSİMLER LİSTESİ\n")
        f.write("================================\n")
        for item in report:
            f.write(item + "\n")
            
    print(f"Rapor hazırlandı: {len(report)} adet sorunlu isim bulundu.")

if __name__ == "__main__":
    generate_broken_report(r'C:\Users\Yeni\Desktop\DİL\Names eşitleme')
