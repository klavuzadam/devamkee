import os

def final_attack_cleanup():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    if not os.path.exists(path):
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    lines = content.split(b'\r\n')
    fixed_lines = []
    fixed_count = 0
    
    # Target patterns
    # ı = \xfd, r = \x72, ı = \xfd
    # 'saldırıı' -> 'saldırı'
    # 'Saldırıı' -> 'Saldırı'
    
    for line in lines:
        if b'\t' in line:
            parts = line.split(b'\t')
            key = parts[0]
            value = b'\t'.join(parts[1:])
            
            initial_value = value
            
            # Clean double 'ı' in 'saldırı'
            if b'sald\xfdr\xfdr\xfd' in value:
                value = value.replace(b'sald\xfdr\xfdr\xfd', b'sald\xfdr\xfd')
            if b'Sald\xfdr\xfdr\xfd' in value:
                value = value.replace(b'Sald\xfdr\xfdr\xfd', b'Sald\xfdr\xfd')
            
            # Specific skill fixes found in list
            if key == b'SKILL_ROOT':
                value = b'K\xf6k Sald\xfdr\xfd'
            if key == b'SKILL_ATTACK_UP':
                value = b'Y\xfcksek Sald\xfdr\xfd'
            if key == b'SKILL_FAST_ATTACK':
                value = b'H\xfdzl\xfd Sald\xfdr\xfd'
                
            # Generic capitalization fix for any 'saldırı' or 'saldırısı' at the start of a value
            # or after a space if it looks like a title
            if value.startswith(b'sald\xfdr\xfd'):
                value = b'Sald\xfdr\xfd' + value[len(b'sald\xfdr\xfd'):]
            
            # Specific typo 'saldırıırdıktan'
            if b'sald\xfdr\xfdf\xfdrd\xfdktan' in value: # Wait, how was it in the list?
                # Line 71: SKILL_ASSASSIN_BLOODTHIRST_DESC ... saldırıırdıktan ...
                # Let's use a safe replace
                value = value.replace(b'sald\xfdr\xfd\xfdrd\xfdktan', b'sald\xfdr\xfdktan')
                
            # Additional common titles
            titles = [
                (b'ejderha tanr\xfd sald\xfdr\xfds\xfd', b'Ejderha Tanr\xfd Sald\xfdr\xfds\xfd'),
                (b'Ejderha Tanr\xfd sald\xfdr\xfds\xfd', b'Ejderha Tanr\xfd Sald\xfdr\xfds\xfd'),
                (b'oto sald\xfdr\xfd', b'Otomatik Sald\xfdr\xfd'),
                (b'Oto sald\xfdr\xfd', b'Otomatik Sald\xfdr\xfd'),
            ]
            for old, new in titles:
                value = value.replace(old, new)

            if value != initial_value:
                fixed_count += 1
                line = key + b'\t' + value
            
        fixed_lines.append(line)
        
    new_content = b'\r\n'.join(fixed_lines)
    
    with open(path, "wb") as f:
        f.write(new_content)
    print(f"Final touches fixed {fixed_count} lines.")

if __name__ == "__main__":
    final_attack_cleanup()
