import os
import re

def analyze_root_strict():
    root_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\root"
    output_file = r"C:\Users\Yeni\Desktop\DİL\root_analiz.txt"
    
    # Polish keywords - using regex with word boundaries where appropriate
    # Words ending with characters like 'o' might be partials of longer words (e.g. Pozostalo)
    keywords = [
        r"\bSklep\b", r"\bKup\b", r"\bCena\b", r"\bTak\b", r"\bNie\b", r"\bdni\b", 
        r"\bgodzin\b", r"\bSztuk\b", r"Pozosta", r"Wybierz", r"Zamknij", r"Anuluj", 
        r"Kategorie", r"Wr\xf3", r"Zaloguj", r"Wyloguj", r"Stan Konta", r"Skarbiec", 
        r"Magazyn", r"Gildia", r"Przedmioty", r"si\xea", r"\bza\b", r"Sekund", 
        r"Minut", r"Poziom", r"Wiadomo"
    ]
    
    # Polish specific characters (literal bytes)
    # CP1250: a:b1, c:e6, e:ea, l:b3, n:f1, o:f3, s:b6, z:bc, z:bf
    polish_chars_bytes = [b'\xb1', b'\xe6', b'\xea', b'\xb3', b'\xf1', b'\xf3', b'\xb6', b'\xbc', b'\xbf']
    
    with open(output_file, "w", encoding="utf-8") as out:
        out.write("--- ROOT DIZINI DETAYLI LEHCE ANALIZ RAPORU (HASSAS TARAMA) ---\n")
        out.write(f"Dizin: {root_path}\n")
        out.write("Not: Sadece gercek Lehce kelimeler ve karakterler raporlanmistir.\n\n")
        
        files = [f for f in os.listdir(root_path) if f.lower().endswith(".py")]
        
        total_found = 0
        
        for filename in files:
            path = os.path.join(root_path, filename)
            found_header = False
            
            try:
                with open(path, "rb") as f:
                    content = f.read()
                
                lines = content.splitlines()
                
                for i, line_bytes in enumerate(lines):
                    line_no = i + 1
                    found_matches = []
                    
                    # 1. Check for Polish Bytes/Characters
                    char_found = []
                    for pb in polish_chars_bytes:
                        if pb in line_bytes:
                            char_found.append(pb.hex())
                    
                    if char_found:
                        found_matches.append(f"Lehce Karakterler: {', '.join(char_found)}")
                    
                    # 2. Check for Keywords using Regex
                    # We need to decode the line as much as possible for regex to work with \b
                    # or stay in bytes. Bytes regex is safer.
                    for kw_pattern in keywords:
                        # Convert pattern to bytes
                        pattern_bytes = kw_pattern.encode("cp1250")
                        # Use ignore case
                        if re.search(pattern_bytes, line_bytes, re.IGNORECASE):
                            # Verification: If it matched "Tak", ensure it's not "Task" or "Take"
                            # The \b in regex handles this, but let's be sure.
                            found_matches.append(f"Kelime: {kw_pattern}")

                    if found_matches:
                        if not found_header:
                            out.write(f"\nDOSYA: {filename}\n")
                            out.write("-" * (len(filename) + 7) + "\n")
                            found_header = True
                        
                        try:
                            # Try to show the line in Turkish encoding
                            display_line = line_bytes.decode("cp1254", errors="replace").strip()
                        except:
                            display_line = repr(line_bytes)
                            
                        out.write(f"Satir {line_no}: {display_line}\n")
                        out.write(f"   -> [Tespit: {'; '.join(found_matches)}]\n")
                        total_found += 1
                
                if found_header:
                    out.write("----------------------------------\n")
            
            except Exception as e:
                out.write(f"HATA: {filename} okunurken sorun olustu: {str(e)}\n")

        out.write(f"\nTOPLAM BULUNAN SATIR: {total_found}\n")

    print(f"Hassas analiz tamamlandi: {output_file}")

if __name__ == "__main__":
    analyze_root_strict()
