import os

def force_fix_potion_dialog():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\potionrechargedialog.py"
    if not os.path.exists(path):
        print("File not found")
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    # Target: "text": "Eliksir Alan\xfd",
    # (Checking for both literal and byte versions)
    
    old_literal = b'Eliksir Alan\\xfd'
    old_byte = b'Eliksir Alan\xfd'
    new_byte = b'\xddksir Yuvas\xfd'
    
    fixed_content = content
    if old_literal in content:
        fixed_content = fixed_content.replace(old_literal, new_byte)
        print("Fixed literal match")
        
    if old_byte in fixed_content:
        fixed_content = fixed_content.replace(old_byte, new_byte)
        print("Fixed byte match")
        
    # Also fix anything that says "Eliksir" and make it "İksir"
    if b'Eliksir' in fixed_content:
        fixed_content = fixed_content.replace(b'Eliksir', b'\xddksir')
        print("Fixed standalone Eliksir match")

    if fixed_content != content:
        with open(path, "wb") as f:
            f.write(fixed_content)
        print("Successfully updated potionrechargedialog.py")
    else:
        print("No changes needed or matching text not found")

if __name__ == "__main__":
    force_fix_potion_dialog()
