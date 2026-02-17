import os

def apply_translations():
    # List of (Path, OldBytes, NewBytes)
    # Using hex codes for Turkish characters in cp1254:
    # i -> \xfd (dotted i is \x69, but in CP1254 \xfd is dotted i? No. 
    # In CP1254:
    # i = \x69 (lower i)
    # I = \x49 (upper I)
    # ş = \xfe
    # Ş = \xde
    # ğ = \xf0
    # Ğ = \xd0
    # ü = \xfc
    # Ü = \xdc
    # ö = \xf6
    # Ö = \xd6
    # ç = \xe7
    # Ç = \xc7
    # ı = \xfd
    # İ = \xdd
    
    translations = [
        # root/uiscript
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\itemshopitem.py", b"Zaczarowanie Przedmiotu (Paczka 4x)", b"Efsun Nesnesi (4'l\xfcc Paket)"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\privateshopbuilder.py", b"Podaj nazwe sklepu...", b"Pazar ismini gir..."),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiscript\dialog_busy_action.py", b"Zbieranie...", b"Toplan\xfdyor..."),
        
        # root
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uiaffectbar.py", b"Nieaktywny", b"Devre d\xfd\xfe\xfd"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uibusyaction.py", b"Zbieranie", b"Toplan\xfdyor"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uibusyaction.py", b"Przerwano", b"\xddptal edildi"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uibusyaction.py", b"Niepowodzenie", b"Ba\xfear\xfds\xfdz"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uicraft.py", b"Niehandlowalne", b"Ticaret yap\xfdlmaz"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uicaptcha.py", b"Niepoprawny kod", b"Ge\xe7ersiz kod"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\game.py", b"[Sklep]", b"[Nesne Market]"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\root\uireport.py", b"Wybierz", b"Se\xe7"),
        
        # locale
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\common\locale_game_ikashop.txt", b"Sklep zosta\xb3 zamkni\xe9ty!", b"Pazar kapat\xfdld\xfd!"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\mob_names.txt", b"20628\t\xaddnie\xbfny Kwiat", b"20628\tKar \xc7i\xe7e\xf0i"),
        (r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game_marty.txt", b"DAY\tg", b"DAY\tg\xfcn"),
    ]
    
    for path, old_bytes, new_bytes in translations:
        if not os.path.exists(path):
            continue
            
        try:
            with open(path, "rb") as f:
                content = f.read()
            
            if old_bytes in content:
                new_content = content.replace(old_bytes, new_bytes)
                with open(path, "wb") as f:
                    f.write(new_content)
                print(f"Fixed: {os.path.basename(path)}")
        except Exception as e:
            print(f"Error fixing {os.path.basename(path)}: {str(e)}")

if __name__ == "__main__":
    apply_translations()
