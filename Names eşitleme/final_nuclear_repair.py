import os

def FINAL_BYTE_REPAIR(target_dir):
    # Bu script, soru işareti (?) şeklinde kalan karakterleri 
    # bayt seviyesinde gerçek Türkçe karakterlere çevirir.
    # ı: \xfd, İ: \xdd, ş: \xfe, Ş: \xde, ğ: \xf0, Ğ: \xd0, ü: \xfc, Ü: \xdc, ö: \xf6, Ö: \xd6, ç: \xe7, Ç: \xc7
    
    files = ['yeni.txt', 'item_names_farklı.txt']
    
    # Bilinen bozuklukların bayt haritası
    # Bazı sistemler ?'i 0x3F olarak kodlar, bazıları ise hatalı baytları (?) olarak gösterir.
    # Biz hem metin içindeki ? işaretlerini hem de hatalı bayt dizilerini yakalayacağız.
    
    replacements = {
        # Alt?n -> Altın
        b'Alt?n': b'Alt\xfdn',
        b'Avc?': b'Avc\xfd',
        b'Tasmas?': b'Tasmas\xfd',
        b'K?l??': b'K\xfdl\xfd\xe7',
        b'Han?er': b'Han\xe7er',
        b'B??ak': b'B\xfd\xe7ak',
        b'Is?r???': b'Is\xfdr\xfd\xf0\xfd',
        b'?eytan': b'\xfeeytan',
        b'Yumru?u': b'Yumru\xf0\xfd',
        b'B??a??': b'B\xfd\xe7a\xf0\xfd',
        b'?im\xfeek': b'\xfeim\xfeek', # \xfeim\xfeek (şimşek)
        b'?an': b'\xe7an', # çan
        b'Yelpaze': b'Yelpaze', # Garanti
        b'z?rh?': b'z\xfdrh\xfd',
        b'Z?rh': b'Z\xfdrh',
        b'?ld?r?c?': b'\xd6ld\xfcr\xfcc\xfc',
        b'G?m??': b'G\xfcm\xfc\xfe',
        b'D?nya': b'D\xfcnya',
        b'Kurtulu?': b'Kurtulu\xfe',
        b'Bilgelik': b'Bilgelik',
        b'Kemer?': b'Kemeri',
        b'Par?as?': b'Par\xe7as\xfd',
        b'Ejderha': b'Ejderha'
    }

    for filename in files:
        path = os.path.join(target_dir, filename)
        if not os.path.exists(path): continue
        
        with open(path, 'rb') as f:
            data = f.read()
        
        original_data = data
        for bad, good in replacements.items():
            data = data.replace(bad, good)
            
        # Genel ? onarımı (Eğer tekil kalmışsa ve öncesindeki harfe göre tahmin edilebilir)
        # Ama bu riskli olabilir, o yüzden şimdilik sadece kesin kelimelerle gidiyoruz.
        
        with open(path, 'wb') as f:
            f.write(data)
            
        if original_data != data:
            print(f"Onarıldı (Binary): {filename}")

    # Kopya dosyasını tazeleyelim
    yeni_path = os.path.join(target_dir, 'yeni.txt')
    kopya_path = os.path.join(target_dir, 'yeni - Kopya.txt')
    if os.path.exists(yeni_path):
        with open(yeni_path, 'r', encoding='cp1254', errors='ignore') as f_in:
            with open(kopya_path, 'w', encoding='cp1254') as f_out:
                for line in f_in:
                    parts = line.split('\t')
                    if len(parts) > 1:
                        f_out.write(parts[1])
                    elif 'VNUM' in line:
                        f_out.write("LOCALE_NAME\n")

if __name__ == "__main__":
    FINAL_BYTE_REPAIR(r'C:\Users\Yeni\Desktop\DİL\Names eşitleme')
