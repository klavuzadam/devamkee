import os

def fix_root():
    root_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root"
    
    fixes = [
        (os.path.join(root_path, "uicaptcha.py"), b"Pozostaly czas:", "Kalan s\xfcre:".encode("cp1254")),
        (os.path.join(root_path, "uireport.py"), b'"Wybierz",', '"Se\xe7",'.encode("cp1254")),
        (os.path.join(root_path, "uiinventory.py"), b"# nie handel", b"# ticaret degil"),
        # UIScripts
        (os.path.join(root_path, "uiscript", "captchadialog.py"), b'"text": "Pozostalo: 45s"', '"text": "Kalan: 45s"'.encode("cp1254")),
        (os.path.join(root_path, "uiscript", "captchadialog.py"), b'"text": "Pozostalo prob: 3"', '"text": "Kalan deneme: 3"'.encode("cp1254")),
        (os.path.join(root_path, "uiscript", "gamemaster_captchadialog.py"), b'"text": "Pozostalo czasu: 30s"', '"text": "Kalan s\xfcre: 30s"'.encode("cp1254")),
        (os.path.join(root_path, "uiscript", "privateshopmanage.py"), b'"text" : "Pozostalo 23:59:59"', '"text" : "Kalan 23:59:59"'.encode("cp1254")),
        (os.path.join(root_path, "uiscript", "shopdialog.py"), b'"text": "Pozostalo 23:59:59"', '"text": "Kalan 23:59:59"'.encode("cp1254")),
        (os.path.join(root_path, "uiscript", "itemshopitem.py"), b"Koniec za", b"Bitis"),
    ]
    
    for path, old, new in fixes:
        if os.path.exists(path):
            with open(path, "rb") as f:
                data = f.read()
            
            if old in data:
                data = data.replace(old, new)
                with open(path, "wb") as f:
                    f.write(data)
                print(f"Fixed: {path}")
            else:
                print(f"Not found in {path}: {old}")

if __name__ == "__main__":
    fix_root()
