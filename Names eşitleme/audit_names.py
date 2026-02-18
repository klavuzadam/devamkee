import os

def byte_audit(base_dir):
    eski_farkli = os.path.join(base_dir, 'eski dosyalar', 'item_names_farklı.txt')
    yeni_farkli = os.path.join(base_dir, 'item_names_farklı.txt')
    
    if not os.path.exists(eski_farkli) or not os.path.exists(yeni_farkli):
        print("Hata: Dosyalar bulunamadı.")
        return

    def load_raw_data(path):
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
                    except ValueError:
                        pass
        return data

    print("Dosyalar bayt bazlı yükleniyor...")
    old_data = load_raw_data(eski_farkli)
    new_data = load_raw_data(yeni_farkli)

    report = []
    
    # Bilinen düzeltmelerimiz (Lehçe -> Türkçe gibi) bu listede "farklılık" olarak görünecek.
    # Biz özellikle Karakter kaybını (length decrease) veya beklenmedik değişimleri arıyoruz.
    
    for vnum in sorted(new_data.keys()):
        if vnum in old_data:
            old_name_bytes = old_data[vnum]
            new_name_bytes = new_data[vnum]
            
            if old_name_bytes != new_name_bytes:
                # İsim değişmiş. 
                # Eğer yeni isim eskisinden kısaysa (karakter silinmişse) kesin bozukluktur.
                # Veya içinde soru işareti varsa.
                
                # Türkçe karakter baytları (cp1254): \xfd (ı), \xfe (ş), \xf0 (ğ), \xfc (ü), \xf6 (ö), \xe7 (ç)
                # Eğer bunlar eskisinde var yenisinde yoksa raporla.
                tr_bytes = [0xfd, 0xfe, 0xf0, 0xfc, 0xf6, 0xe7, 0xdd, 0xde, 0xd0, 0xdc, 0xd6, 0xc7]
                
                lost_tr = False
                for b in tr_bytes:
                    if (bytes([b]) in old_name_bytes) and (bytes([b]) not in new_name_bytes):
                        # Ama belki biz o karakteri değiştirdik (örn: bozuk UTF8 -> Doğru TR byte)
                        # Bu yüzden boyuta da bakalım
                        pass

                # En basit kontrol: Boyut azaldıysa veya karakterler kaybolduysa
                # (Çeviriler genelde boyutu büyütür veya değiştirir ama TR karakter silmez)
                
                # Lehçe çevirilerimizi hariç tutalım (Örn: Pas -> Kemer)
                # Ama 31073 (Altın Avcı...) gibi TR isimli bir şeyde boy kısalması varsa:
                if len(new_name_bytes) < len(old_name_bytes):
                     report.append((vnum, old_name_bytes, new_name_bytes))
                elif b'?' in new_name_bytes:
                     report.append((vnum, old_name_bytes, new_name_bytes))

    # Raporu oluştur
    report_path = os.path.join(base_dir, 'final_discrepancy_report.txt')
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write(f"{'VNUM':<10} | {'ESKI (Raw)':<40} | {'YENI (Raw)':<40}\n")
        f.write("-" * 100 + "\n")
        for vnum, old, new in report:
            f.write(f"{vnum:<10} | {str(old):<40} | {str(new):<40}\n")
            
    print(f"Denetim tamamlandı. Toplam {len(report)} potansiyel sorun bulundu.")
    print(f"Rapor: {report_path}")

if __name__ == "__main__":
    byte_audit(r'C:\Users\Yeni\Desktop\DİL\Names eşitleme')
