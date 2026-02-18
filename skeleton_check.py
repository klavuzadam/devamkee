
import re

def get_skeleton(path):
    with open(path, 'r', encoding='latin-1') as f:
        lines = f.readlines()
    
    skeleton = []
    for line in lines:
        stripped = line.strip()
        if not stripped or stripped.startswith('--'):
            continue
        # Remove string content
        line_no_strings = re.sub(r'\".*\"', '""', stripped)
        # Remove trailing comments
        line_clean = re.sub(r'--.*$', '', line_no_strings).strip()
        if line_clean:
            skeleton.append(line_clean)
    return skeleton

orig_skel = get_skeleton(r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate.lua')
tr_skel = get_skeleton(r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua')

max_len = max(len(orig_skel), len(tr_skel))
diffs = []
for i in range(max_len):
    s1 = orig_skel[i] if i < len(orig_skel) else "[EOF]"
    s2 = tr_skel[i] if i < len(tr_skel) else "[EOF]"
    if s1 != s2:
        diffs.append(f"Diff at entry {i+1}:")
        diffs.append(f"  Orig: {s1}")
        diffs.append(f"  Tr  : {s2}")
        if len(diffs) > 40: break

if diffs:
    print("\n".join(diffs))
else:
    print("Skeletons are identical.")
