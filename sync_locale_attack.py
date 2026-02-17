import os

def sync_and_fix_attack_strings():
    main_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    marty_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game_marty.txt"
    
    if not os.path.exists(main_path) or not os.path.exists(marty_path):
        print("Required files not found")
        return
        
    # 1. Load Marty Locale into a dict
    marty_dict = {}
    with open(marty_path, "rb") as f:
        for line in f:
            line = line.strip(b'\r\n')
            if b'\t' in line:
                parts = line.split(b'\t')
                key = parts[0]
                value = b'\t'.join(parts[1:])
                marty_dict[key] = value
    
    # 2. Fix the main locale
    with open(main_path, "rb") as f:
        content = f.read()
    
    lines = content.split(b'\r\n')
    fixed_lines = []
    fixed_count = 0
    
    # CP1254: ı = \xfd, Büyü = B\xfcy\xfc
    
    for line in lines:
        if b'\t' in line:
            parts = line.split(b'\t')
            key = parts[0]
            value = b'\t'.join(parts[1:])
            
            initial_value = value
            
            # Key checks for synchronization from original reference
            if key in marty_dict:
                marty_val = marty_dict[key]
                if b'sald\xfdr\xfd' in initial_value.lower():
                    value = marty_val
            
            # Special manual fixes for common mistakes or remains
            if b'sald\xfdr\xfdonu' in value: # Düğün Salonu fix
                value = value.replace(b'sald\xfdr\xfdonu', b'Salonu')
            
            if b'sald\xfdr\xfdr\xfdramazs\xfdn' in value: # saldıramazsın fix
                value = value.replace(b'sald\xfdr\xfdr\xfdramazs\xfdn', b'sald\xfdiramazs\xfdn')

            # Force Capitalization for stats and tooltips starting with 'saldırı'
            if value.startswith(b'sald\xfdr\xfd'):
                value = b'Sald\xfdr\xfd' + value[len(b'sald\xfdr\xfd'):]
            
            # Mid-sentence property names (Party bonuses etc)
            replacements = [
                (b'b\xfcy\xfc sald\xfdr\xfd', b'B\xfcy\xfc Sald\xfdr\xfd'),
                (b'B\xfcy\xfc sald\xfdr\xfd', b'B\xfcy\xfc Sald\xfdr\xfd'),
                (b'fiziksel sald\xfdr\xfd', b'Fiziksel Sald\xfdr\xfd'),
                (b'Fiziksel sald\xfdr\xfd', b'Fiziksel Sald\xfdr\xfd'),
                (b'sald\xfdr\xfd De\xf0eri', b'Sald\xfdr\xfd De\xf0eri'),
                (b'sald\xfdr\xfd H\xfd\x7afd', b'Sald\xfdr\xfd H\xfd\x7afd'),
                (b'sald\xfdr\xfd G\xfc\xe7', b'Sald\xfdr\xfd G\xfc\xe7'),
            ]
            for old, new in replacements:
                value = value.replace(old, new)

            if value != initial_value:
                fixed_count += 1
                line = key + b'\t' + value
            
        fixed_lines.append(line)
        
    new_content = b'\r\n'.join(fixed_lines)
    
    with open(main_path, "wb") as f:
        f.write(new_content)
    print(f"Fixed {fixed_count} lines in locale_game.txt")

if __name__ == "__main__":
    sync_and_fix_attack_strings()
