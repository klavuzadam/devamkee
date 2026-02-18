import os

def convert_to_latin5(file_path):
    # Bu script, UTF-8 kodlamasındaki dosyayı okur 
    # ve Latin5 (cp1254) olarak kaydeder.
    # Metin2 Türkiye lokasyonu (locale/tr) ve Navicat (latin1/latin5) 
    # genellikle bu kodlamayı bekler.
    
    if not os.path.exists(file_path):
        return

    try:
        # Önce UTF-8 olarak oku
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        # Latin5 (cp1254) olarak kaydet
        with open(file_path, 'w', encoding='cp1254', errors='ignore') as f:
            f.write(content)
            
        print(f"Latin5'e (cp1254) dönüştürüldü: {file_path}")
    except Exception as e:
        print(f"Hata ({file_path}): {str(e)}")

if __name__ == "__main__":
    files = [
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\item_names_farklı.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni - Kopya.txt'
    ]
    for f in files:
        convert_to_latin5(f)
