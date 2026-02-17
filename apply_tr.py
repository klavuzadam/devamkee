# -*- coding: utf-8 -*-
import json
import sys
import os

def apply_translations(translation_dict):
    path = 'locale_pairs.json'
    try:
        with open(path, 'r', encoding='utf-8') as f:
            pairs = json.load(f)
    except Exception as e:
        print(f"Error loading {path}: {e}")
        return

    updated_count = 0
    for p in pairs:
        pid = str(p['id'])
        if pid in translation_dict:
            p['tr'] = translation_dict[pid]
            updated_count += 1

    with open(path, 'w', encoding='utf-8') as f:
        json.dump(pairs, f, ensure_ascii=False, indent=2)
    
    print(f"Updated {updated_count} pairs in {path}")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        arg = sys.argv[1]
        if os.path.exists(arg):
            with open(arg, 'r', encoding='utf-8') as f:
                tr_data = json.load(f)
        else:
            tr_data = json.loads(arg)
        apply_translations(tr_data)
