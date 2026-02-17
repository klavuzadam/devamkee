import os

def fix_login_interface():
    # File 1: locale/tr/locale_interface.txt
    file1 = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_interface.txt"
    # File 2: locale/common/locale_interface_ex.txt
    file2 = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\common\locale_interface_ex.txt"
    
    # CP1254: ğ = \xf0, ı = \xfd, ü = \xfc
    
    # Replacement for File 1
    if os.path.exists(file1):
        with open(file1, "rb") as f:
            content = f.read()
            
        # CREDENTIALS_LOAD	Y?kle -> CREDENTIALS_LOAD	Bağlan
        # Note: Y?kle is likely Y\xfckle or Y... depending on original file encoding.
        # I will match the key.
        lines = content.split(b"\n")
        new_lines = []
        for line in lines:
            if b"CREDENTIALS_LOAD" in line:
                new_lines.append(b"CREDENTIALS_LOAD\tBa\xf0lan")
            elif b"BUTTON_HOMEPAGE" in line:
                # "Beni oku" is fine, but maybe user wants "Web Sitesi"? 
                # Keeping as is for now unless requested.
                new_lines.append(line)
            else:
                new_lines.append(line)
        
        new_content = b"\n".join(new_lines)
        if new_content != content:
            with open(file1, "wb") as f:
                f.write(new_content)
            print(f"Updated {os.path.basename(file1)}")
            
    # Replacement for File 2
    if os.path.exists(file2):
        with open(file2, "rb") as f:
            content = f.read()
            
        fixes = [
            (b"SAVE_ACCOUNT_CONNECT2\t|cffFF0000|hF{}|r Connect {}", b"SAVE_ACCOUNT_CONNECT2\t|cffFF0000|hF{}|r Ba\xf0lan {}"),
            (b"SAVE_ACCOUNT_TITLE\tSave Account Board", b"SAVE_ACCOUNT_TITLE\tKay\xfdtl\xfd Hesaplar"),
            (b"SAVE_ACCOUNT_SAVE\tSave Account", b"SAVE_ACCOUNT_SAVE\tHesab\xfd Kaydet"),
            (b"SAVE_ACCOUNT_CONNECT\tConnect #{}", b"SAVE_ACCOUNT_CONNECT\tBa\xf0lan #{}"),
            (b"SAVE_ACCOUNT_REMOVE\tRemove", b"SAVE_ACCOUNT_REMOVE\tSil"),
        ]
        
        new_content = content
        for old, new in fixes:
            if old in new_content:
                new_content = new_content.replace(old, new)
                print(f"Fixed {old.split(b'\t')[0].decode()} in {os.path.basename(file2)}")
            elif old.replace(b"\t", b" ") in new_content: # Try space instead of tab
                 new_content = new_content.replace(old.replace(b"\t", b" "), new)
                 print(f"Fixed {old.split(b'\t')[0].decode()} (space) in {os.path.basename(file2)}")

        if new_content != content:
            with open(file2, "wb") as f:
                f.write(new_content)
            print(f"Updated {os.path.basename(file2)}")

if __name__ == "__main__":
    fix_login_interface()
