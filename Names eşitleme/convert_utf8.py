import os

def convert_to_utf8_nobom(file_path):
    # Bu script, Latin5 (cp1254) kodlamasındaki dosyayı okur 
    # ve UTF-8 (BOM'suz) olarak kaydeder.
    # Metin2 dump_proto ve oyun motoru genellikle UTF-8 bekler.
    
    if not os.path.exists(file_path):
        return

    try:
        # Önce Latin5 olarak oku
        with open(file_path, 'r', encoding='cp1254', errors='ignore') as f:
            content = f.read()
            
        # UTF-8 (BOM'suz) olarak kaydet
        with open(file_path, 'w', encoding='utf-8', newline='') as f:
            f.write(content)
            
        print(f"UTF-8'e dönüştürüldü: {file_path}")
    except Exception as e:
        print(f"Hata ({file_path}): {str(e)}")

if __name__ == "__main__":
    files = [
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\item_names_farklı.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni - Kopya.txt'
    ]
    for f in files:
        convert_to_utf8_nobom(f)
