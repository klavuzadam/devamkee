import os

def fix_final_touches():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    if not os.path.exists(path):
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    # Specific targeted fixes for remains
    replacements = [
        (b'zAte\xfen', b'zaten'),
        (b'ZAte\xfen', b'Zaten'),
        (b'saAte\xfe', b'saate'),
        (b'KAte\xfegori', b'Kategori'),
    ]
    
    new_content = content
    for old, new in replacements:
        if old in new_content:
            new_content = new_content.replace(old, new)
            print(f"Fixed string: {old.decode('cp1254', errors='ignore')} -> {new.decode('cp1254', errors='ignore')}")
            
    if new_content != content:
        with open(path, "wb") as f:
            f.write(new_content)
        print("Final touches applied to locale_game.txt")
    else:
        print("No more remains found.")

if __name__ == "__main__":
    fix_final_touches()
