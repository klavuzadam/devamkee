import os

def final_polish_repair(file_path):
    # Bu script, çeviri sonrası kalan veya soru işaretine dönüşen 
    # karakter bozulmalarını (Enerji Par?as? gibi) düzeltir.
    
    # Bayt tabanlı değişimler (cp1254 baytları)
    # \xfd: ı, \xfe: ş, \xf0: ğ, \xfc: ü, \xf6: ö, \xe7: ç
    
    replacements = {
        # Soru işareti şeklinde kalmış olanları VNUM/İçerik bazlı tamir edelim
        b'Enerji Par?as?': b'Enerji Par\xe7as\xfd',
        b'Enerji Kristali': b'Enerji Kristali', # Zaten düzgün olabilir ama garanti olsun
        b'Ay \xddksiri': b'Ay \xddksiri', # İksiri baş harfi İ (\xdd)
        b'G\xfcne\xba': b'G\xfcne\xfe', # Güneş (ş: \xfe)
        b'?erisi': b'\xferisi', # şerisi
    }

    if not os.path.exists(file_path):
        return

    with open(file_path, 'rb') as f:
        data = f.read()

    original_data = data
    
    # Bilinen bozuklukları bayt bayt değiştir
    for bad, good in replacements.items():
        data = data.replace(bad, good)
        
    # Ayrıca metin içinde kalan diğer "çöl, güneş, reçete" gibi kelimelerdeki 
    # olası ? karakterlerini (eğer Latin5 seti dışındaysa) düzeltelim
    # Ama daha güvenlisi, spesifik kelime bazlı gitmek.
    
    # Yaz
    with open(file_path, 'wb') as f:
        f.write(data)
    
    if original_data != data:
        print(f"Final onarım yapıldı: {file_path}")

if __name__ == "__main__":
    files = [
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\item_names_farklı.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni - Kopya.txt'
    ]
    for f in files:
        final_polish_repair(f)
