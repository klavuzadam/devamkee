import os
import re

def super_sync(target_dir):
    encoding = 'cp1254'
    farkli_path = os.path.join(target_dir, 'item_names_farklı.txt')
    yeni_path = os.path.join(target_dir, 'yeni.txt')
    
    def load_data(path):
        data = {}
        header = "VNUM\tLOCALE_NAME\n"
        if not os.path.exists(path):
            return data, header
        with open(path, 'r', encoding=encoding, errors='ignore') as f:
            lines = f.readlines()
            if not lines:
                return data, header
            if 'VNUM' in lines[0] or 'LOCALE_NAME' in lines[0]:
                header = lines[0]
                start_idx = 1
            else:
                start_idx = 0
            for line in lines[start_idx:]:
                line = line.strip()
                if not line: continue
                parts = line.split('\t')
                if parts:
                    try:
                        vnum = int(parts[0].strip())
                        name = parts[1].strip() if len(parts) > 1 else ""
                        data[vnum] = name
                    except ValueError:
                        pass
        return data, header

    # 1. Dosyaları yükle
    print("Dosyalar yükleniyor...")
    farkli_data, farkli_header = load_data(farkli_path)
    yeni_data, yeni_header = load_data(yeni_path)
    
    # 2. VNUM listesini yeni.txt referansından al (Kullanıcın isteği: yeni'de olmayanları sil)
    ref_vnums = sorted(yeni_data.keys())
    
    # 3. Senkronize edilmiş liste oluştur
    final_list = []
    updated_names_count = 0
    
    for vnum in ref_vnums:
        # İsim önceliği: item_names_farklı.txt (Kullanıcının isteği: oradaki isimler doğru)
        # İstisna: Eğer vnum farkli'da yoksa yeni'dekinden al
        correct_name = farkli_data.get(vnum)
        old_name = yeni_data.get(vnum)
        
        name_to_use = ""
        if vnum == 8103:
            name_to_use = "Demir Ok Ucu"
        elif correct_name:
            name_to_use = correct_name
            if correct_name != old_name:
                updated_names_count += 1
        else:
            name_to_use = old_name
            
        final_list.append(f"{vnum}\t{name_to_use}\n")

    # 4. Kaydet
    output_header = farkli_header # Genelde aynıdır ama farkli'nınkini koruyalım
    
    # item_names_farklı.txt güncelle
    with open(farkli_path, 'w', encoding=encoding) as f:
        f.write(output_header)
        f.writelines(final_list)
        
    # yeni.txt güncelle
    with open(yeni_path, 'w', encoding=encoding) as f:
        f.write(output_header)
        f.writelines(final_list)

    print(f"İşlem Tamamlandı!")
    print(f"Referans VNUM Sayısı: {len(ref_vnums)}")
    print(f"Düzeltilen İsim Sayısı: {updated_names_count}")
    print(f"Artı seviyeleri (+0, +1 vb.) ve tüm karakterler korunarak dosyalar eşitlendi.")

if __name__ == "__main__":
    dir_path = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme'
    super_sync(dir_path)
