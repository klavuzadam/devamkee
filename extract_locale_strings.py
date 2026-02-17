# -*- coding: utf-8 -*-
import re
import json
import os

def extract_pairs():
    path = r'C:\Users\Yeni\Desktop\DİL\localestring çeviri\locale_string.txt'
    if not os.path.exists(path):
        print(f"File not found: {path}")
        return

    try:
        with open(path, 'rb') as f:
            content = f.read().decode('cp1250', errors='ignore')
    except Exception as e:
        print(f"Error reading file: {e}")
        return

    # Metin2 locale_string.txt format is usually pairs of strings in quotes:
    # "English Key";
    # "Polish translation";
    
    # We use a pattern that finds all strings inside double quotes that end with a semicolon.
    # Note: Some strings might have escaped quotes, but usually they don't in Metin2.
    pattern = r'\"(.*?)\";'
    all_strings = re.findall(pattern, content, re.DOTALL)

    pairs = []
    for i in range(0, len(all_strings) - 1, 2):
        # We take strings in pairs. 
        # i is the key (English), i+1 is the translation (Polish)
        pairs.append({
            "id": i // 2,
            "key": all_strings[i],
            "pl": all_strings[i+1]
        })

    # Save to a temporary json file for processing
    output_path = 'locale_pairs.json'
    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(pairs, f, ensure_ascii=False, indent=2)

    print(f"Extracted {len(pairs)} pairs to {output_path}")

if __name__ == "__main__":
    extract_pairs()
