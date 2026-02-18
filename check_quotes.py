
import re
import os

path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

with open(path, 'r', encoding='latin-1') as f:
    content = f.read()

# 1. Total balance
q_count = content.count('"')
print(f"Total double quotes: {q_count}")
if q_count % 2 != 0:
    print("WARNING: Total quote count is ODD!")

# 2. Line by line check
lines = content.splitlines()
errors = []
for i, line in enumerate(lines):
    # Ignore comments
    line_clean = re.sub(r'--.*$', '', line)
    cnt = line_clean.count('"')
    if cnt % 2 != 0:
        errors.append((i + 1, line.strip()))

if errors:
    print(f"Found {len(errors)} lines with odd number of quotes:")
    for l_num, text in errors:
        # Safe print for encoding
        safe_text = text.encode('ascii', 'replace').decode('ascii')
        print(f"Line {l_num}: {safe_text}")
else:
    print("No lines with unbalanced quotes found.")

# 3. Check for strings spanning multiple lines
multi_line_matches = re.finditer(r'\"([^\"\n]*\n+[^\"\n]*)\"', content)
found_multi = False
for m in multi_line_matches:
    found_multi = True
    start_pos = m.start()
    line_num = content.count('\n', 0, start_pos) + 1
    # Capture a bit of context
    context = content[max(0, start_pos-50):min(len(content), m.end()+50)]
    safe_context = context.encode('ascii', 'replace').decode('ascii').replace('\n', '\\n')
    print(f"LITERAL NEWLINE IN STRING at line {line_num}:")
    print(f"  Context: ...{safe_context}...")

if not found_multi:
    print("No literal newlines in strings found.")
