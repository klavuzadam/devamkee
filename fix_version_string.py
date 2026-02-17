import os

def fix_version_string():
    files_to_fix = [
        r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\intrologin.py",
        r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uisystem.py",
        r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\loginwindow.py",
        r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\systemdialog.py"
    ]
    
    fixed_count = 0
    for file_path in files_to_fix:
        if not os.path.exists(file_path):
            print(f"File not found: {file_path}")
            continue
            
        with open(file_path, "rb") as f:
            content = f.read()
            
        if b"Wersja" in content:
            new_content = content.replace(b"Wersja", b"Versiyon")
            with open(file_path, "wb") as f:
                f.write(new_content)
            print(f"Fixed: {os.path.basename(file_path)}")
            fixed_count += 1
        else:
            print(f"No match in: {os.path.basename(file_path)}")
            
    print(f"Total files fixed: {fixed_count}")

if __name__ == "__main__":
    fix_version_string()
