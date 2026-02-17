import os

def fix_comprehensive_locale():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    if not os.path.exists(path):
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    # 1. Fix remaining corrupted tags in Keys (Key is uppercase, usually no 'ş')
    # Use a more strict replace for keys
    lines = content.split(b'\r\n')
    fixed_lines = []
    
    for line in lines:
        if b'\t' in line:
            parts = line.split(b'\t')
            key = parts[0]
            value = b'\t'.join(parts[1:])
            
            # Key fixes
            if b'te\xfe' in key:
                key = key.replace(b'te\xfe', b'TE')
            
            # Value fixes
            # Pattern: 'te\xfe\xfe' -> 'te\xfe' (ateşş -> ateş)
            if b'te\xfe\xfe' in value:
                value = value.replace(b'te\xfe\xfe', b'te\xfe')
            
            # Pattern: 'zAte\xfen' (corrupted zaten) -> 'zaten'
            if b'zAte\xfen' in value:
                value = value.replace(b'zAte\xfen', b'zaten')
            
            # Pattern: 'ZAte\xfen' -> 'Zaten'
            if b'ZAte\xfen' in value:
                value = value.replace(b'ZAte\xfen', b'Zaten')
                
            # Pattern: 'KAte\xfegori' -> 'Kategori'
            if b'KAte\xfegori' in value:
                value = value.replace(b'KAte\xfegori', b'Kategori')
            
            line = key + b'\t' + value
            
        fixed_lines.append(line)
        
    new_content = b'\r\n'.join(fixed_lines)
    
    if new_content != content:
        with open(path, "wb") as f:
            f.write(new_content)
        print("Successfully applied comprehensive fix to locale_game.txt")
    else:
        print("No more corruptions found.")

if __name__ == "__main__":
    fix_comprehensive_locale()
