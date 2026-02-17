import os

def fix_uicraft_manual():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uicraft.py"
    if not os.path.exists(path):
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    # ı = \xfd, ğ = \xf0, ü = \xfc, ş = \xfe, ö = \xf6, ç = \xe7
    # CP1254 encoding bytes
    
    replacements = [
        (b'"Bronie"', b'"Silahlar"'),
        (b'"Pancerze"', b'"Z\xfdrhlar"'),
        (b'"Inne"', b'"Di\xf0er"'),
        (b'"Zwoje i Marmury"', b'"Par\xfe\xf6menler ve K\xfcreler"'),
        (b'"Wymiany"', b'"Takaslar"'),
        (b'# zmianka', b'# efsun de\xf0i\xfetirme'),
        (b'MessengerWindow.__LoadWindow.__Bind', b'CraftingWindow.__LoadWindow.__Bind'),
    ]
    
    new_content = content
    for old, new in replacements:
        if old in new_content:
            new_content = new_content.replace(old, new)
            print(f"Fixed: {old.decode('utf-8', errors='ignore')} -> {new.decode('cp1254', errors='ignore')}")
            
    if new_content != content:
        with open(path, "wb") as f:
            f.write(new_content)
        print("Successfully cleaned uicraft.py manually.")
    else:
        print("No matches found for manual cleanup in uicraft.py.")

if __name__ == "__main__":
    fix_uicraft_manual()
