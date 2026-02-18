import os
import re

def detect_polish_refined(file_path):
    encoding = 'cp1254'
    
    # Lehçe kelime kökleri
    polish_roots = [
        r'\bMiecz\b', r'\bZbroja\b', r'\bFryzura\b', r'\bSkrzynia\b', 
        r'\bMikstura\b', r'\bKamie\w+', r'\bDuchowy\b', r'\bMagiczny\b', 
        r'\bZloty\b', r'\bSrebrny\b', r'\bBialy\b', r'\bNiebieski\b', 
        r'\bCzerwony\b', r'\bZielony\b', r'\bCzarny\b', r'\bFioletowy\b', 
        r'\bZolty\b', r'\bHelm\b', r'\bTarcza\b', r'\bBransoleta\b', 
        r'\bNaszyjnik\b', r'\bKolczyki\b', r'\bButy\b', r'\bWachlarz\b', 
        r'\bDzwon\b', r'\bLuk\b', r'\bSztylet\b', r'\bOstrze\b', r'\bPas \b',
        r'M\w+dro\w+ci', r'Pierwszorz\w+dne', r'S\w+aba'
    ]
    
    detected = []
    if not os.path.exists(file_path):
        return []

    with open(file_path, 'r', encoding=encoding, errors='ignore') as f:
        for line in f:
            line_clean = line.strip()
            if not line_clean: continue
            
            # Soru işareti içerenler (Muhtemelen bozuk Lehçe karakterler)
            if '?' in line_clean and not any(turkish in line_clean for turkish in ['ı','ş','ğ','ü','ö','ç']):
                 # Eğer soru işareti varsa ve bariz Türkçe karakter yoksa Lehçe olma ihtimali yüksektir.
                 detected.append(line_clean)
                 continue

            # Regex tabanlı kontrol
            for root in polish_roots:
                if re.search(root, line_clean, re.IGNORECASE):
                    detected.append(line_clean)
                    break
                    
    return detected

if __name__ == "__main__":
    file_path = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme\yeni.txt'
    results = detect_polish_refined(file_path)
    
    print(f"Tespit edilen net Lehçe satır sayısı: {len(results)}")
    for r in results:
        print(r)
