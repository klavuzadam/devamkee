import os

def final_nuclear_fix(target_dir):
    encoding = 'cp1254'
    files = ['yeni.txt', 'item_names_farklı.txt']
    
    # 51502 \t Enerji Parçası \r\n (Latin5 baytları)
    # ç: \xe7, ı: \xfd
    energy_fix = b"51502\tEnerji Par\xe7as\xfd\r\n"
    
    for filename in files:
        path = os.path.join(target_dir, filename)
        if not os.path.exists(path): continue
        
        with open(path, 'rb') as f:
            data = f.read()

        # Doğrudan bayt değiştirmeyi dene (VNUM'lu haliyle)
        # Önce dosyada nasıl durduğuna bakalım (PowerShell ? olarak gösterse de gerçekte başka karakter dizisi olabilir)
        # Ama en güvenli arama VNUM bazlı satır bulup değiştirmek.
        
        lines = data.splitlines(keepends=True)
        new_lines = []
        for line in lines:
            if line.startswith(b"51502\t"):
                new_lines.append(energy_fix)
            else:
                # Satırda soru işareti baytı (0x3F) ve Enerji/Parça kelimeleri varsa temizle
                if b"Enerji Par" in line and b"?" in line:
                    line = line.replace(b"Enerji Par?as?", b"Enerji Par\xe7as\xfd")
                    line = line.replace(b"Enerji Par?as", b"Enerji Par\xe7as\xfd")
                    # Eğer bayt 0x3F (?) değil de başka bir şeyse:
                    line = line.replace(b"Enerji Par", b"Enerji Par\xe7as\xfd").strip() + b"\r\n"
                new_lines.append(line)

        with open(path, 'wb') as f:
            f.writelines(new_lines)

    # Kopya dosyasını tekrar oluştur
    yeni_path = os.path.join(target_dir, 'yeni.txt')
    kopya_path = os.path.join(target_dir, 'yeni - Kopya.txt')
    if os.path.exists(yeni_path):
        with open(yeni_path, 'r', encoding=encoding, errors='ignore') as f_in:
            with open(kopya_path, 'w', encoding=encoding) as f_out:
                for line in f_in:
                    parts = line.split('\t')
                    if len(parts) > 1:
                        f_out.write(parts[1])
                    elif 'VNUM' in line:
                        f_out.write("LOCALE_NAME\n")

    print("Kesin onarım tamamlandı.")

if __name__ == "__main__":
    dir_path = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme'
    final_nuclear_fix(dir_path)
