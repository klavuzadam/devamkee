import os
import re

def fix_bonus_categories_regex():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    if not os.path.exists(path):
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    # ı = \xfd, r = \x72
    # Targets: 
    # sald\xfdr\xfd\xfdr\xfd
    # sald\xfdr\xfdr\xfd
    # sald\xfdr\xfdr\xfds\xfd (saldırırısı)
    
    lines = content.split(b'\r\n')
    fixed_lines = []
    fixed_count = 0
    
    # Regex for corrupted forms of 'saldırı' or 'Saldırı'
    # Match [sS]ald followed by 3 or more chars from {\xfd, \x72}
    # and not continuing with other common letters if possible.
    # But simpler: just replace known bad patterns.
    
    BAD_PATTERNS = [
        (b'Sald\xfdr\xfd\xfdr\xfd', b'Sald\xfdr\xfd'), # Saldıırırı -> Saldırı
        (b'sald\xfdr\xfd\xfdr\xfd', b'sald\xfdr\xfd'), # saldıırırı -> saldırı
        (b'Sald\xfdr\xfdr\xfd', b'Sald\xfdr\xfd'),   # Saldırırı -> Saldırı
        (b'sald\xfdr\xfdr\xfd', b'sald\xfdr\xfd'),   # saldırırı -> saldırı
        (b'sald\xfdr\xfdr\xfds\xfd', b'sald\xfdr\xfds\xfd'), # saldırırısı -> saldırısı
        (b'Sald\xfdr\xfdr\xfds\xfd', b'Sald\xfdr\xfds\xfd'), # Saldırırısı -> Saldırısı
    ]
    
    for line in lines:
        if b'\t' in line:
            parts = line.split(b'\t')
            key = parts[0]
            value = b'\t'.join(parts[1:])
            
            initial_value = value
            
            # 1. Force fix for ATTRIBUTE_LIST categories
            if key.startswith(b'ATTRIBUTE_LIST_OFFENSIVE'):
                # First cleanup corruptions
                for bad, good in BAD_PATTERNS:
                    value = value.replace(bad, good)
                
                # Then ensure starts with 'Saldırı'
                if value.lower().startswith(b'sald\xfdr\xfd'):
                    # Preserve original case of 'Saldırı'
                    value = b'Sald\xfdr\xfd' + value[len(b'sald\xfdr\xfd'):]
            else:
                # General fix for corruptions in other values
                for bad, good in BAD_PATTERNS:
                    value = value.replace(bad, good)
            
            if value != initial_value:
                fixed_count += 1
                line = key + b'\t' + value
            
        fixed_lines.append(line)
        
    new_content = b'\r\n'.join(fixed_lines)
    
    if new_content != content:
        with open(path, "wb") as f:
            f.write(new_content)
        print(f"Successfully fixed {fixed_count} lines in locale_game.txt")
    else:
        print("No corruptions found or already fixed.")

if __name__ == "__main__":
    fix_bonus_categories_regex()
