
import re
import os

orig_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate.lua'
tr_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

def load_file(path):
    with open(path, 'r', encoding='latin-1') as f:
        return f.readlines()

orig_lines = load_file(orig_path)
tr_lines = load_file(tr_path)

print(f"Original lines: {len(orig_lines)}")
print(f"Translated lines: {len(tr_lines)}")

# 1. Structural comparison - ignore the string content, check the code structure
def get_signature(line):
    line = line.strip()
    if not line or line.startswith('--'):
        return None
    # Replace the string value with a placeholder
    # This regex looks for: key = "value" or [key] = "value"
    sig = re.sub(r'\".*\"', '"VALUE"', line)
    return sig

errors = []
max_len = max(len(orig_lines), len(tr_lines))

for i in range(max_len):
    l1 = orig_lines[i] if i < len(orig_lines) else ""
    l2 = tr_lines[i] if i < len(tr_lines) else ""
    
    s1 = get_signature(l1)
    s2 = get_signature(l2)
    
    if s1 != s2:
        # Ignore minor whitespace differences in the signature itself
        if s1 and s2 and s1.replace(" ", "") == s2.replace(" ", ""):
            continue
            
        # Specific check for missing commas
        if s1 and s2 and s1.endswith(',') and not s2.endswith(','):
            errors.append(f"Line {i+1}: Missing comma? \n  Orig: {l1.strip()}\n  Tr  : {l2.strip()}")
        elif s1 != s2:
             # Only report if it's not a comment or empty line difference
             if s1 or s2:
                errors.append(f"Line {i+1}: Structural mismatch\n  Orig: {l1.strip()}\n  Tr  : {l2.strip()}")

if errors:
    print(f"Found {len(errors)} structural/syntax differences:")
    for err in errors[:50]:
        # Encode to clean ascii/latin-1 for console output if needed
        print(err.encode('ascii', 'replace').decode('ascii'))
    if len(errors) > 50:
        print("...")
else:
    print("No structural differences found.")

# 2. Check for unbalanced markers across the whole file
tr_content = "".join(tr_lines)
for marker in ['{', '}', '[', ']']:
    c = tr_content.count(marker)
    print(f"Marker '{marker}' count: {c}")
    
# Check for unclosed quotes
quote_errors = []
for i, line in enumerate(tr_lines):
    if line.count('"') % 2 != 0:
        quote_errors.append(i+1)
if quote_errors:
    print(f"Unclosed quotes found on lines: {quote_errors}")
else:
    print("No unclosed quotes found.")
