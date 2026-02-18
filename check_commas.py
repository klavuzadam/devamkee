
import re
import os

path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

if not os.path.exists(path):
    print(f"File not found: {path}")
    exit(1)

with open(path, 'r', encoding='latin-1') as f:
    lines = f.readlines()

errors = []
for i, line in enumerate(lines):
    stripped = line.strip()
    
    # 1. Check for missing comma in [number] = "text"
    if re.search(r'\[\d+\]\s*=\s*\".*\"$', stripped):
        # Look ahead for next key
        found_next = False
        for next_i in range(i + 1, len(lines)):
            next_line = lines[next_i].strip()
            if not next_line or next_line.startswith('--'):
                continue
            if re.search(r'\[\d+\]\s*=', next_line):
                found_next = True
            break
        
        if found_next:
            errors.append((i + 1, line.rstrip()))

    # 2. Check for missing comma in key = "text"
    if re.search(r'^\w+\s*=\s*\".*\"$', stripped):
        found_next = False
        for next_i in range(i + 1, len(lines)):
            next_line = lines[next_i].strip()
            if not next_line or next_line.startswith('--'):
                continue
            if re.search(r'^\w+\s*=', next_line) or next_line.startswith('{'):
                found_next = True
            break
        
        if found_next:
             errors.append((i + 1, line.rstrip()))

if errors:
    print(f"Found {len(errors)} potential syntax errors (missing commas):")
    for line_num, content in errors:
        print(f"Line {line_num}: {content}")
else:
    print("No missing commas found matching the cross-line criteria.")
