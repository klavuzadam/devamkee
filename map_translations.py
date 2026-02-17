# -*- coding: utf-8 -*-
import re
import json
import os

def load_reference(path):
    # Try different encodings for Turkish file
    encodings = ['cp1254', 'utf-8', 'latin-1']
    content = None
    for enc in encodings:
        try:
            with open(path, 'rb') as f:
                content = f.read().decode(enc)
            print(f"Loaded reference with {enc}")
            break
        except Exception:
            continue
    
    if content is None:
        return {}

    # Extract pairs from reference
    pattern = r'\"(.*?)\";'
    all_strings = re.findall(pattern, content, re.DOTALL)
    
    ref_map = {}
    for i in range(0, len(all_strings) - 1, 2):
        key = all_strings[i].strip()
        val = all_strings[i+1].strip()
        ref_map[key] = val
    
    return ref_map

def map_translations():
    ref_path = r'C:\Users\Yeni\Desktop\DİL\localestring çeviri\locale_string_başka_files.txt'
    pairs_path = 'locale_pairs.json'
    
    if not os.path.exists(pairs_path):
        print("locale_pairs.json not found. Extracting first...")
        # (Assuming extract script was already run)
        return

    ref_map = load_reference(ref_path)
    print(f"Reference Map Size: {len(ref_map)} entries.")

    with open(pairs_path, 'r', encoding='utf-8') as f:
        target_pairs = json.load(f)

    match_count = 0
    matched_keys = []
    for p in target_pairs:
        key = p['key'].strip()
        if key in ref_map:
            p['tr'] = ref_map[key]
            match_count += 1
            matched_keys.append(key)
        else:
            p['tr'] = None # Mark for manual translation
    
    print(f"Matched Keys: {matched_keys[:50]}")

    with open(pairs_path, 'w', encoding='utf-8') as f:
        json.dump(target_pairs, f, ensure_ascii=False, indent=2)

    print(f"Mapped {match_count} / {len(target_pairs)} entries.")
    print(f"Remaining: {len(target_pairs) - match_count} entries to translate.")

if __name__ == "__main__":
    map_translations()
