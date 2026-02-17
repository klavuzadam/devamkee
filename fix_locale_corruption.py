import os

def fix_corrupted_locale():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    if not os.path.exists(path):
        print("File not found.")
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    # The corruption seems to be 'te' (lower) followed by hex 0xfe (ş in CP1254)
    # replacing it back to 'TE' (upper)
    # But only if it's likely a Key (before the first tab or in a word that looks like a tag)
    
    lines = content.split(b'\r\n')
    fixed_lines = []
    
    for line in lines:
        if b'\t' in line:
            parts = line.split(b'\t')
            key = parts[0]
            # If key contains the corruption 'te\xfe', fix it to 'TE'
            if b'te\xfe' in key:
                key = key.replace(b'te\xfe', b'TE')
                print(f"Fixed Key: {key.decode('cp1254', errors='ignore')}")
            
            # For values, we should be careful. 
            # But 'te\xfe' followed by a capital letter or at the end of a word-like segment in value 
            # might also be corruption of tags like %s or similar, but here it looks like mostly Keys were affected.
            # However, 'Ateş' (fire) is usually 'A-t-e-ş'. 
            # My 'te\xfe' search in values showed things like 'teşekkürler' which are VALID.
            # So I will ONLY fix Keys for now unless I find a corrupted value.
            
            line = b'\t'.join([key] + parts[1:])
            
        fixed_lines.append(line)
        
    new_content = b'\r\n'.join(fixed_lines)
    
    if new_content != content:
        with open(path, "wb") as f:
            f.write(new_content)
        print("Successfully fixed corrupted keys in locale_game.txt")
    else:
        print("No corruptions found or already fixed.")

if __name__ == "__main__":
    fix_corrupted_locale()
