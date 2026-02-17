
# -*- coding: utf-8 -*-
import os

files = [
    r"Client\pack\locale\locale\tr\locale_game.txt",
    r"Client\pack\locale\locale\tr\itemdesc.txt"
]
polish_bytes = b"\xb3\xbf\x9c\xe6\xf1\xf3\xb9\xea"

results = []
for f_path in files:
    if not os.path.exists(f_path):
        continue
    with open(f_path, 'rb') as f:
        for line in f:
            if any(b in line for b in polish_bytes):
                parts = line.split(b"\t")
                if len(parts) >= 2:
                    results.append(parts[1].decode('cp1254', 'ignore').strip())

with open("polish_to_translate.txt", "w", encoding="utf-8") as out:
    for s in sorted(list(set(results))):
        out.write(s + "\n")
print(f"Extracted {len(set(results))} unique Polish strings.")
