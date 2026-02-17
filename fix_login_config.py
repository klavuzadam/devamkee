import os

def fix_login_and_config():
    # List of (File, Replacements)
    # CP1254: ı = \xfd, İ = \xdd, ü = \xfc, ö = \xf6, ş = \xfe, ç = \xe7, ğ = \xf0
    
    fixes = [
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\loginwindow.py", [
            (b'"Ustawienia"', b'"Ayarlar"'),
            (b'"Zasady"', b'"Kurallar"'),
        ]),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\configwindow.py", [
            (b'"Muzyka"', b'"M\xfczik"'),
            (b'"Inne"', b'"Di\xf0er"'),
            (b'"Skala (1.0)"', b'#\xd6l\xe7ek (1.0)#'.replace(b'#', b'"')), # Avoiding direct quote confusion in my head
        ]),
    ]
    
    # Wait, "Skala (1.0)" I will replace with "Ölçek (1.0)"
    # ö = \xf6, l = \x6c, ç = \xe7, e = \x65, k = \x6b
    olcek = b'\xd6l\xe7ek (1.0)' # Actually \xd6 is Ö in Latin-5 (Turkish)? 
    # Let's check: Ö = \xd6, ö = \xf6, Ç = \xc7, ç = \xe7
    # "Ölçek" -> \xd6, l, \xe7, e, k
    
    fixes[1][1][2] = (b'"Skala (1.0)"', b'"\xd6l\xe7ek (1.0)"')
    
    for file_path, replacements in fixes:
        if not os.path.exists(file_path):
            print(f"File not found: {file_path}")
            continue
            
        with open(file_path, "rb") as f:
            content = f.read()
            
        new_content = content
        for old, new in replacements:
            if old in new_content:
                new_content = new_content.replace(old, new)
                print(f"Fixed {old.decode('utf-8')} in {os.path.basename(file_path)}")
        
        if new_content != content:
            with open(file_path, "wb") as f:
                f.write(new_content)
            print(f"Successfully updated {os.path.basename(file_path)}")
        else:
            print(f"No changes needed for {os.path.basename(file_path)}")

if __name__ == "__main__":
    fix_login_and_config()
