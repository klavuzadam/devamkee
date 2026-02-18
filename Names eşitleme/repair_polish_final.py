import os

def final_polish_fix(file_path):
    # Bu script, Lehçe karakterlerin (ł, ą, ś vb.) soru işaretine dönüşmesiyle 
    # oluşan "Enerji Par?as?" gibi bozulmaları kesin olarak düzeltir.
    encoding = 'cp1254'
    
    if not os.path.exists(file_path):
        return

    # Önce dosyayı latin1 olarak okuyalım ki tüm baytlar gelsin
    with open(file_path, 'r', encoding='latin1', errors='ignore') as f:
        content = f.read()

    # Bilinen bozuk kalıpların onarılması
    # ? karakterleri bazen \xfd (ı), \xfe (ş) vb. olabilir ama direkt '?' olarak görünüyorsa:
    fixes = {
        'Enerji Par?as?': 'Enerji Parçası',
        'Enerji Kristali': 'Enerji Kristali',
        'Kryszta? Energii': 'Enerji Kristali',
        'Ay ?ksiri': 'Ay İksiri',
        'G?ne?': 'Güneş',
        'I??k': 'Işık',
        'Sand???': 'Sandığı',
        'Ta??': 'Taşı',
        'Receptura': 'Reçete',
        'Eliksir': 'İksir',
        'Od?amek': 'Parçası',
        'Kronika Rzemie?lników': 'Zanaatkar Günlüğü',
        'Rzemie?lników': 'Zanaatkar',
        'Wytrzyma?o??': 'Dayanıklılık',
        'Ci?gliwo??': 'Süreklilik',
        'M?dro?ci': 'Bilgelik',
        'Nied?wiedzi': 'Ayı',
        'Ksi??yca': 'Ay',
        'S?o?ca': 'Güneş',
        'Trwa?o?ci': 'Dayanıklılık',
        'Pot?gi': 'Güç',
        '?uski': 'Pulu',
        'S?aba': 'Zayıf'
    }

    original_content = content
    for bad, good in fixes.items():
        content = content.replace(bad, good)

    # Hala soru işareti kaldıysa (Lehçe özel harfi ł gibi olanlar için) 
    # Ama dosya genelindeki diğer Türkçe karakterleri bozmamalıyız.
    
    # Yaz (cp1254 / Latin5)
    with open(file_path, 'w', encoding=encoding, errors='ignore') as f:
        f.write(content)
    
    if original_content != content:
        print(f"Onarıldı: {file_path}")

if __name__ == "__main__":
    files = [
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\item_names_farklı.txt',
        r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni - Kopya.txt'
    ]
    for f in files:
        final_polish_fix(f)
