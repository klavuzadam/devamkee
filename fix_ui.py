import os

def fix_files():
    # Fix characterwindow.py
    path1 = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\characterwindow.py"
    if os.path.exists(path1):
        with open(path1, "rb") as f:
            data = f.read()
        
        # Replace common corruptions of "Özellikler"
        # 1. UTF-8 Ö: \xc3\x96
        # 2. Corrupted UTF-8 Ö read as replacement char + -: \xef\xbf\xbd-
        # 3. Just "-zellikler"
        data = data.replace(b"\xc3\x96zellikler", "Özellikler".encode("cp1254"))
        data = data.replace(b"\xef\xbf\xbd-zellikler", "Özellikler".encode("cp1254"))
        data = data.replace(b"-zellikler", "Özellikler".encode("cp1254"))
        
        # Ensure the whole file is CP1254 by trying to decode from UTF-8 and re-encode
        # This is risky if file is mixed, so we just do targeted replaces for now.
        
        with open(path1, "wb") as f:
            f.write(data)
        print(f"Fixed {path1}")

    # Fix itemshopwindow.py
    path2 = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\itemshopwindow.py"
    if os.path.exists(path2):
        with open(path2, "rb") as f:
            data = f.read()
        
        # Translations
        replacements = [
            (b"Sklep z przedmiotami", "Nesne Market".encode("cp1254")),
            (b"Stan Konta", "Hesap Durumu".encode("cp1254")),
            (b"\"text\" : \"SM\"", "\"text\" : \"EP\"".encode("cp1254")),
            (b"\"text\" : \"SZ\"", "\"text\" : \"KP\"".encode("cp1254")),
            (b"Kategorie", "Kategoriler".encode("cp1254")),
            (b"Potwierdzenie zakupu", "Satın Alma Onayı".encode("cp1254")),
            (b"Kup 1", "Satın Al (1)".encode("cp1254")),
            (b"Kup 5", "Satın Al (5)".encode("cp1254")),
            (b"Kup 10", "Satın Al (10)".encode("cp1254"))
        ]
        
        for old, new in replacements:
            data = data.replace(old, new)
            
        with open(path2, "wb") as f:
            f.write(data)
        print(f"Fixed {path2}")

if __name__ == "__main__":
    fix_files()
