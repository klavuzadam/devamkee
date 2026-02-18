import os

def brute_force_repair(target_dir):
    encoding = 'cp1254'
    
    # VNUM bazlı kesin isimler (Soru işaretli kalanlar için)
    vnum_fixes = {
        51502: "Enerji Parçası",
        51503: "Enerji Kristali",
        18030: "Bilgelik Kemeri+0",
        18031: "Bilgelik Kemeri+1",
        18032: "Bilgelik Kemeri+2",
        18033: "Bilgelik Kemeri+3",
        18034: "Bilgelik Kemeri+4",
        18035: "Bilgelik Kemeri+5",
        18036: "Bilgelik Kemeri+6",
        18037: "Bilgelik Kemeri+7",
        18038: "Bilgelik Kemeri+8",
        18039: "Bilgelik Kemeri+9",
        18070: "Ayı Kemeri+0",
        18071: "Ayı Kemeri+1",
        18072: "Ayı Kemeri+2",
        18073: "Ayı Kemeri+3",
        18074: "Ayı Kemeri+4",
        18075: "Ayı Kemeri+5",
        18076: "Ayı Kemeri+6",
        18077: "Ayı Kemeri+7",
        18078: "Ayı Kemeri+8",
        18079: "Ayı Kemeri+9",
        50944: "Ay İksiri Reçetesi (K)",
        50947: "Güneş İksiri Reçetesi (K)",
        11303: "Zayıf Zırh",
        11307: "Birinci Sınıf Ayakkabı"
        # Gerektiğinde buraya ekleme yapılabilir
    }

    files = ['yeni.txt', 'item_names_farklı.txt']
    for filename in files:
        path = os.path.join(target_dir, filename)
        if not os.path.exists(path): continue
        
        with open(path, 'r', encoding=encoding, errors='ignore') as f:
            lines = f.readlines()
        
        new_lines = []
        for line in lines:
            parts = line.split('\t')
            if len(parts) > 0:
                try:
                    vnum = int(parts[0].strip())
                    if vnum in vnum_fixes:
                        # VNUM bazlı tamir yap
                        line = f"{vnum}\t{vnum_fixes[vnum]}\n"
                except ValueError:
                    pass
            new_lines.append(line)
            
        with open(path, 'w', encoding=encoding) as f:
            f.writelines(new_lines)

    # yeni - Kopya.txt için özel (Sadece isimler olduğu için tab içermez, sıralama bazlı gidilebilir 
    # veya yeni.txt'den tekrar oluşturulabilir - En güvenlisi tekrar oluşturmak)
    yeni_path = os.path.join(target_dir, 'yeni.txt')
    kopya_path = os.path.join(target_dir, 'yeni - Kopya.txt')
    
    if os.path.exists(yeni_path):
        with open(yeni_path, 'r', encoding=encoding) as f_in:
            with open(kopya_path, 'w', encoding=encoding) as f_out:
                for line in f_in:
                    parts = line.split('\t')
                    if len(parts) > 1:
                        f_out.write(parts[1])
                    elif 'VNUM' in line:
                        f_out.write("LOCALE_NAME\n")

    print("VNUM bazlı kesin onarım ve Kopya dosyası güncellemesi tamamlandı.")

if __name__ == "__main__":
    dir_path = r'C:\Users\Yeni\Desktop\DİL\Names eşitleme'
    brute_force_repair(dir_path)
