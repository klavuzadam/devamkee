import os
import re

def analyze_root_recursive():
    root_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root"
    output_file = r"C:\Users\Yeni\Desktop\DİL\root_analiz_recursive.txt"
    
    # Polish keywords - using regex with word boundaries
    keywords = [
        r"\bSklep\b", r"\bKup\b", r"\bCena\b", r"\bTak\b", r"\bNie\b", r"\bdni\b", 
        r"\bgodzin\b", r"\bSztuk\b", r"Pozosta", r"Wybierz", r"Zamknij", r"Anuluj", 
        r"Kategorie", r"Wr\xf3", r"Zaloguj", r"Wyloguj", r"Stan Konta", r"Skarbiec", 
        r"Magazyn", r"Gildia", r"Przedmioty", r"si\xea", r"\bza\b", r"Sekund", 
        r"Minut", r"Poziom", r"Wiadomo"
    ]
    
    # Polish specific characters (literal bytes)
    polish_chars_bytes = [b'\xb1', b'\xe6', b'\xea', b'\xb3', b'\xf1', b'\xf3', b'\xb6', b'\xbc', b'\xbf']
    
    with open(output_file, "w", encoding="utf-8") as out:
        out.write("--- ROOT DIZINI RECURSIVE LEHCE ANALIZ RAPORU ---\n")
        out.write(f"Dizin: {root_path}\n\n")
        
        total_found = 0
        
        for root, dirs, files in os.walk(root_path):
            for file in files:
                if file.lower().endswith(".py"):
                    path = os.path.join(root, file)
                    rel_path = os.path.relpath(path, root_path)
                    found_header = False
                    
                    try:
                        with open(path, "rb") as f:
                            content = f.read()
                        
                        lines = content.splitlines()
                        
                        for i, line_bytes in enumerate(lines):
                            line_no = i + 1
                            found_matches = []
                            
                            # 1. Check for Keywords using Regex
                            for kw_pattern in keywords:
                                pattern_bytes = kw_pattern.encode("cp1250")
                                if re.search(pattern_bytes, line_bytes, re.IGNORECASE):
                                    # Filter false positives for 'Minute' if keywords has 'Minut'
                                    if kw_pattern == r"Minut" and b"minute" in line_bytes.lower():
                                        continue
                                    found_matches.append(kw_pattern)

                            # 2. Check for Polish Bytes/Characters
                            char_found = []
                            for pb in polish_chars_bytes:
                                if pb in line_bytes:
                                    char_found.append(pb.hex())
                            
                            if char_found:
                                found_matches.append(f"Chars({', '.join(char_found)})")

                            if found_matches:
                                if not found_header:
                                    out.write(f"\nDOSYA: {rel_path}\n")
                                    out.write("-" * (len(rel_path) + 7) + "\n")
                                    found_header = True
                                
                                try:
                                    display_line = line_bytes.decode("cp1254", errors="replace").strip()
                                except:
                                    display_line = repr(line_bytes)
                                    
                                out.write(f"Satir {line_no}: {display_line} [Tespit: {', '.join(found_matches)}]\n")
                                total_found += 1
                        
                        if found_header:
                            out.write("----------------------------------\n")
                    
                    except Exception as e:
                        out.write(f"HATA: {rel_path} okunurken sorun olustu: {str(e)}\n")

        out.write(f"\nTOPLAM BULUNAN SATIR: {total_found}\n")

    print(f"Recursive analiz tamamlandi: {output_file}")

if __name__ == "__main__":
    analyze_root_recursive()
