import os

def list_attack_strings():
    path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\locale_game.txt"
    if not os.path.exists(path):
        print("File not found")
        return
        
    with open(path, "rb") as f:
        content = f.read()
    
    lines = content.split(b'\r\n')
    results = []
    
    for line in lines:
        if b'sald\xfdr\xfd' in line.lower():
            results.append(line.decode('cp1254', errors='ignore'))
            
    # Save to a temporary file to show the user
    output_path = r"C:\Users\Yeni\Desktop\DİL\saldırı_listesi.txt"
    with open(output_path, "w", encoding="utf-8") as f:
        for res in results:
            f.write(res + "\n")
    
    print(f"Total lines found: {len(results)}. List saved to {output_path}")

if __name__ == "__main__":
    list_attack_strings()
