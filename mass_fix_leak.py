import os

def mass_fix():
    base_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack"
    
    # Using raw bytes to avoid encoding issues in the script itself
    # ı = 0xfd, ü = 0xfc, ş = 0xfe, ö = 0xf6, ç = 0xe7, ğ = 0xf0
    # I = 0xdd, Ü = 0xdc, Ş = 0xde, Ö = 0xd6, Ç = 0xc7, Ğ = 0xd0
    
    replacements = [
        (b" Sztuk", b" Adet"),
        (b" Szt.", b" Adet"),
        (b" Cena:", b" Fiyat:"),
        (b" Cena :", b" Fiyat :"),
        (b" dni", b" g\xfcn"),
        (b"(dni)", b"(g\xfcn)"),
        (b" godzin", b" saat"),
        (b" Kup ", b" Sat\xfdn Al "),
        (b" Sklep", b" Market"),
        (b" Sklepu", b" Marketi"),
    ]
    
    for root, dirs, files in os.walk(base_path):
        for file in files:
            if any(file.lower().endswith(ext) for ext in [".py", ".txt", ".xml", ".lua", ".pyc"]):
                # Skip .pyc as they are binary and might lead to corruption if replaced blindly
                if file.lower().endswith(".pyc"):
                    continue
                    
                path = os.path.join(root, file)
                try:
                    with open(path, "rb") as f:
                        data = f.read()
                    
                    original_data = data
                    for old, new in replacements:
                        data = data.replace(old, new)
                    
                    if data != original_data:
                        with open(path, "wb") as f:
                            f.write(data)
                        print(f"Fixed: {path}")
                except Exception as e:
                    print(f"Error on {path}: {e}")

if __name__ == "__main__":
    mass_fix()
