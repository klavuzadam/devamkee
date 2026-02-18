import os

def strip_vnums_and_tabs(file_path):
    # Encoding: Metin2 Turkish files usually use Windows-1254 (cp1254)
    encoding = 'cp1254'
    
    if not os.path.exists(file_path):
        print(f"Hata: Dosya ({file_path}) bulunamadı.")
        return

    output_lines = []
    with open(file_path, 'r', encoding=encoding, errors='ignore') as f:
        for line in f:
            # Satırı tab karakterine göre böl
            parts = line.split('\t', 1) # Sadece ilk tabdan böl
            if len(parts) > 1:
                # İlk kısmı (VNUM) at, geri kalanı (İsim) al
                name_part = parts[1]
                output_lines.append(name_part)
            else:
                # Tab yoksa satırı olduğu gibi veya boş geçebiliriz, 
                # ama genelde sadece isim kalsın istendiği için:
                # Eğer satırda tab yoksa ama içerik varsa (başlık gibi değil) koruyalım mı?
                # Kullanıcı "vnum numaraları ve tablar silinecek" dediği için tablı satırları işliyoruz.
                pass

    # Dosyayı güncelle
    with open(file_path, 'w', encoding=encoding) as f:
        f.writelines(output_lines)
    
    print(f"İşlem tamamlandı. Toplam {len(output_lines)} isim ayıklandı.")

if __name__ == "__main__":
    target_file = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni - Kopya.txt'
    strip_vnums_and_tabs(target_file)
