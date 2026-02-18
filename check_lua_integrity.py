
import re
import os

path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

with open(path, 'r', encoding='latin-1') as f:
    content = f.read()

# Assignments like key = value where value is not starting with ", {, digit, true, false, nil
# Improved regex to handle spaces before the value
matches = re.finditer(r'^\s*([\w\.]+)\s*=\s*(?!(?:\s*\"|\s*\{|\s*true|\s*false|\s*nil|\s*\d))(.+)$', content, re.MULTILINE)
found = False
for m in matches:
    print(f'Suspect Assignment at: {m.group(0).strip().encode("ascii", "replace").decode("ascii")}')
    found = True

if not found:
    print("No suspect unquoted assignments found.")

# Check for any non-ASCII characters OUTSIDE of strings
lines = content.splitlines()
for i, line in enumerate(lines):
    # Strip comments
    line_no_comment = re.sub(r'--.*$', '', line)
    # Find parts OUTSIDE of strings
    # This is a bit tricky with nested quotes, but translate.lua is simple
    parts = re.split(r'\".*?\"', line_no_comment)
    for part in parts:
        if any(ord(c) > 127 for c in part):
            print(f"Non-ASCII outside string at line {i+1}: {line.strip().encode('ascii', 'replace').decode('ascii')}")
