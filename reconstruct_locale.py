# -*- coding: utf-8 -*-
import json
import os

def reconstruct():
    json_path = 'locale_pairs.json'
    output_path = r'C:\Users\Yeni\Desktop\DİL\localestring çeviri\locale_string_tr.txt'
    
    if not os.path.exists(json_path):
        print(f"Error: {json_path} not found.")
        return

    try:
        with open(json_path, 'r', encoding='utf-8') as f:
            pairs = json.load(f)
    except Exception as e:
        print(f"Error loading JSON: {e}")
        return

    print(f"Loaded {len(pairs)} pairs. Starting reconstruction...")
    
    try:
        # Using cp1254 for Turkish characters (Game requirement)
        with open(output_path, 'w', encoding='cp1254', errors='replace') as f:
            for p in pairs:
                key = p['key']
                # tr might be missing if mapping failed, fallback to pl but shouldn't happen now
                tr = p.get('tr', p['pl']) 
                
                # Metin2 locale_string.txt format:
                # "KEY";
                # "VALUE";
                # (empty line)
                f.write(f'"{key}";\n')
                f.write(f'"{tr}";\n\n')
        
        print(f"Successfully reconstructed file to: {output_path}")
        print(f"Total lines written: {len(pairs) * 2}")
    except Exception as e:
        print(f"Error writing file: {e}")

if __name__ == "__main__":
    reconstruct()
