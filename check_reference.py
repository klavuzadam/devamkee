# -*- coding: utf-8 -*-
import re
import os

def check_reference():
    path = r'C:\Users\Yeni\Desktop\DİL\localestring çeviri\locale_string_başka_files.txt'
    if not os.path.exists(path): return

    with open(path, 'rb') as f:
        content = f.read().decode('cp1254', errors='ignore')
    
    pattern = r'\"(.*?)\";'
    all_strings = re.findall(pattern, content, re.DOTALL)
    
    print("FIRST 20 PAIRS IN REFERENCE:")
    for i in range(0, min(40, len(all_strings)), 2):
        if i+1 < len(all_strings):
            print(f"[{i//2}] KEY: {repr(all_strings[i])}")
            print(f"[{i//2}] VAL: {repr(all_strings[i+1])}")
            print("-" * 10)

if __name__ == "__main__":
    check_reference()
