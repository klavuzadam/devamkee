
import re
import sys

def get_structure(path):
    try:
        with open(path, 'r', encoding='latin-1') as f:
            content = f.read()
    except Exception as e:
        return f"Error reading {path}: {e}"

    # Replace all double-quoted strings with a placeholder to compare structure
    # This regex handles escaped quotes as well
    structure = re.sub(r'\"(.*?)(?<!\\)\"', '\"VALUE\"', content)
    return structure

orig_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate.lua'
tr_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'

orig_struct = get_structure(orig_path)
tr_struct = get_structure(tr_path)

if orig_struct == tr_struct:
    print("STRUCTURE_MATCH: The structures are identical.")
else:
    print("STRUCTURE_MISMATCH: The structures are different.")
    
    orig_lines = orig_struct.splitlines()
    tr_lines = tr_struct.splitlines()
    
    max_len = max(len(orig_lines), len(tr_lines))
    diff_count = 0
    for i in range(max_len):
        l1 = orig_lines[i] if i < len(orig_lines) else "[END OF FILE]"
        l2 = tr_lines[i] if i < len(tr_lines) else "[END OF FILE]"
        
        # Strip trailing potential whitespace
        if l1.rstrip() != l2.rstrip():
            print(f"Difference at line {i+1}:")
            print(f"  Original   : [{l1}]")
            print(f"  Translated : [{l2}]")
            diff_count += 1
            if diff_count > 100:
                print("... too many differences, stopping.")
                break
    if diff_count == 0:
        print("No structural mismatch found after cleaning.")
