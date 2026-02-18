
import os

path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

if not os.path.exists(path):
    print(f"File not found: {path}")
    exit(1)

with open(path, 'r', encoding='latin-1') as f:
    lines = f.readlines()

changes = 0
# Line numbers 53 and 57 (0-indexed: 52 and 56)
for idx in [52, 56]:
    if idx < len(lines):
        line = lines[idx].strip()
        if line.startswith('[') and '=' in line and not line.endswith(','):
            lines[idx] = lines[idx].rstrip() + ',\n'
            changes += 1
            print(f"Fixed line {idx+1}")

if changes > 0:
    with open(path, 'w', encoding='latin-1') as f:
        f.writelines(lines)
    print(f"Applied {changes} fixes to {path}")
else:
    print("No changes applied. Check if the file content matches expectations.")
