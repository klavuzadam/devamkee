import os

def fix_itemshop():
    # Fix uiitemshop.py
    path1 = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiitemshop.py"
    if os.path.exists(path1):
        with open(path1, "rb") as f:
            data = f.read()
        
        replacements = [
            (b'return "SM"', b'return "EP"'),
            (b'return "SZ"', b'return "KP"'),
            (b'30 dni', '30 gün'.encode('cp1254')),
            (b'7 dni', '7 gün'.encode('cp1254')),
            (b'Paczka 25', 'Paket 25'.encode('cp1254')),
            (b'Sztuk: %d', 'Adet: %d'.encode('cp1254')),
        ]
        
        for old, new in replacements:
            data = data.replace(old, new)
            
        with open(path1, "wb") as f:
            f.write(data)
        print(f"Fixed {path1}")

    # Fix itemshop_shopwindow.py
    path2 = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\itemshop_shopwindow.py"
    if os.path.exists(path2):
        with open(path2, "rb") as f:
            data = f.read()
            
        # Polish "się" can be \xca or \xc4\x99 depending on encoding it was read from
        data = data.replace(b"Zaczyna si\xca t za %s  ", "Başlıyor: %s  ".encode("cp1254"))
        data = data.replace(b"Zaczyna si\xca za %s  ", "Başlıyor: %s  ".encode("cp1254"))
        data = data.replace(b"Koniec za %s  ", "Bitiş: %s  ".encode("cp1254"))
        data = data.replace(b"Sztuk: %d | ", "Adet: %d | ".encode("cp1254"))
        data = data.replace(b"Cena: ", "Fiyat: ".encode("cp1254"))
        data = data.replace(b"Koniec promocji za %s | ", "İndirim bitişi: %s | ".encode("cp1254"))
        
        # Fallback for dynamic strings or missed ones
        data = data.replace(b"Sztuk:", "Adet:".encode("cp1254"))
        data = data.replace(b"Cena", "Fiyat".encode("cp1254"))
        data = data.replace(b"Koniec", "Bitiş".encode("cp1254"))
        data = data.replace(b"Zaczyna", "Başlıyor".encode("cp1254"))
            
        with open(path2, "wb") as f:
            f.write(data)
        print(f"Fixed {path2}")

if __name__ == "__main__":
    fix_itemshop()
