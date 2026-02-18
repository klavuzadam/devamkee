import os

def sync_names_from_source(target_path, source_path):
    # Encoding: Metin2 Turkish files usually use Windows-1254 (cp1254)
    encoding = 'cp1254'
    
    if not os.path.exists(source_path):
        print(f"Hata: Kaynak dosya ({source_path}) bulunamadı.")
        return
    
    if not os.path.exists(target_path):
        print(f"Hata: Hedef dosya ({target_path}) bulunamadı.")
        return

    def get_records(path):
        records = {}
        header = None
        with open(path, 'r', encoding=encoding, errors='ignore') as f:
            lines = f.readlines()
            if not lines:
                return records, header
            
            if 'VNUM' in lines[0] or 'LOCALE_NAME' in lines[0]:
                header = lines[0]
                start_idx = 1
            else:
                start_idx = 0
                
            for line in lines[start_idx:]:
                line = line.strip()
                if not line:
                    continue
                parts = line.split('\t')
                if parts:
                    try:
                        vnum = int(parts[0].strip())
                        name = parts[1].strip() if len(parts) > 1 else ""
                        records[vnum] = name
                    except ValueError:
                        pass
        return records, header

    print(f"Kaynak dosya ({source_path}) okunuyor (Doğru isimler)...")
    source_records, source_header = get_records(source_path)
    
    print(f"Hedef dosya ({target_path}) okunuyor (Düzeltilecek dosya)...")
    target_records, target_header = get_records(target_path)

    # Hedef dosyadaki tüm VNUM'ları baz alarak isimleri kaynağa göre güncelle
    updated_count = 0
    final_vnums = sorted(target_records.keys())
    
    output_lines = [target_header if target_header else "VNUM\tLOCALE_NAME\n"]
    for vnum in final_vnums:
        # Kaynak dosyada (farklı.txt) isim varsa onu al, yoksa hedeftekini koru
        correct_name = source_records.get(vnum)
        old_name = target_records.get(vnum)
        
        if correct_name is not None and correct_name != old_name:
            updated_count += 1
            name_to_use = correct_name
        else:
            name_to_use = old_name
            
        output_lines.append(f"{vnum}\t{name_to_use}\n")

    # Hedef dosyayı (yeni.txt) güncelle
    with open(target_path, 'w', encoding=encoding) as f:
        f.writelines(output_lines)
    
    print(f"İşlem tamamlandı.")
    print(f"Toplam {updated_count} adet satırdaki isim kayması/hatası düzeltildi.")
    print(f"Hedef dosya ({target_path}) güncellendi.")

if __name__ == "__main__":
    # Kullanıcı item_names_farklı için "Doğru", yeni için "Hatalı/Kaymış" demişti.
    correct_source = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\item_names_farklı.txt'
    damaged_target = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni.txt'
    sync_names_from_source(damaged_target, correct_source)
