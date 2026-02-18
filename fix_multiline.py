
import os
import re

path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

with open(path, 'r', encoding='latin-1') as f:
    lines = f.readlines()

# We need to join lines where the quote wasn't closed
# Pattern 1: Lines 253 to 256
# talk1 = "Heh
# ...
# ... paslanmaz, heh.",

def join_lines(lines, start_idx, end_idx):
    # join lines from start_idx to end_idx (0-indexed)
    main_line = lines[start_idx].rstrip()
    for i in range(start_idx + 1, end_idx + 1):
        main_line += " " + lines[i].strip()
    # Replace the range with the single line
    new_lines = lines[:start_idx]
    new_lines.append(main_line + '\n')
    new_lines.extend(lines[end_idx+1:])
    return new_lines

# Fix 1: talk1 (Line 253-256) -> Indices 252 to 255
# Wait, I need to be careful with line numbers as they change after each join.
# I'll do it from bottom to top or just use a more robust way.

# Let's find the content and replace it
content = "".join(lines)

# Fix talk1
content = re.sub(r'talk1\s*=\s*\"Heh\s*\n\s*(.*?)\s*paslanmaz, heh.\",', 
                 r'talk1 = "Heh \1 paslanmaz, heh.",', content, flags=re.DOTALL)

# Fix talk2
content = re.sub(r'talk2\s*=\s*\"Dinle\s*\n\s*(.*?)\s*kazanalım.\",', 
                 r'talk2 = "Dinle \1 kazanalım.",', content, flags=re.DOTALL)

# Fix talk3
content = re.sub(r'talk3\s*=\s*\"Ama bir şeyi unutma\s*\n\s*(.*?)\s*sake koyacağım.\",', 
                 r'talk3 = "Ama bir şeyi unutma \1 sake koyacağım.",', content, flags=re.DOTALL)

# Fix blacksmith_talk4
content = re.sub(r'blacksmith_talk4\s*=\s*\"Dinle, burada güç arayışında olduğunu biliyorum ama Min-Sun\s*\n\s*(.*?)\s*düştüler.\",', 
                 r'blacksmith_talk4 = "Dinle, burada güç arayışında olduğunu biliyorum ama Min-Sun \1 düştüler.",', content, flags=re.DOTALL)

# Also fix the weird spaces I saw in hex (0x85)
content = content.replace('\x85', ' ')

with open(path, 'w', encoding='latin-1') as f:
    f.write(content)

print("Applied joins to multiline strings.")
