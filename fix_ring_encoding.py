# -*- coding: utf-8 -*-
import os
import subprocess

# 1. Definir le contenu de la quest (utilisant des TABULATIONS au lieu des espaces)
content = u"""quest ring_warp begin
	state start begin
		when 40002.use begin
			say_title("Işınlanma Yüzüğü")
			say("Nereye gitmek istiyorsun?")
			local s = select("Köyler", "Orman Bölgeleri", "Örümcek Zindanı", "Sürgün Mağarası", "Karanlık Ejderha Kayası", "Diğer Bölgeler", "Kapat")

			if s == 1 then
				say_title("Işınlanma Yüzüğü")
				local ss = select("Kırmızı Bayrak (1.Köy)", "Kırmızı Bayrak (2.Köy)", "Sarı Bayrak (1.Köy)", "Sarı Bayrak (2.Köy)", "Mavi Bayrak (1.Köy)", "Mavi Bayrak (2.Köy)", "Vazgeç")
				if ss == 1 then
					pc.warp(474300,954800)
				elseif ss == 2 then
					pc.warp(360800,877600)
				elseif ss == 3 then
					pc.warp(63800,166400)
				elseif ss == 4 then
					pc.warp(138500,234900)
				elseif ss == 5 then
					pc.warp(959800,268400)
				elseif ss == 6 then
					pc.warp(873100,242600)
				end
			elseif s == 2 then
				say_title("Işınlanma Yüzüğü")
				local sss = select("Hayalet Orman", "Kızıl Orman Başlangıç", "Kızıl Orman Sonu", "Vazgeç")
				if sss == 1 then
					pc.warp(288700,5700)
				elseif sss == 2 then
					pc.warp(1119900,70800)
				elseif sss == 3 then
					pc.warp(1118100,8600)
				end
			elseif s == 3 then
				say_title("Işınlanma Yüzüğü")
				local zindan = select("Örümcek Zindanı 1. Kat Başı", "Örümcek Zindanı 2. Kat Sonu", "Vazgeç")
				if zindan == 1 then
					pc.warp(59800,497300)
				elseif zindan == 2 then
					pc.warp(704100,521900)
				end
			elseif s == 4 then
				say_title("Işınlanma Yüzüğü")
				say("Sürgün Mağarası girişine Işınlanıyorsun...")
				wait()
				pc.warp(284400,810700)
			elseif s == 5 then
				say_title("Işınlanma Yüzüğü")
				say("Karanlık Ejderha Kayasına Işınlanıyorsun...")
				wait()
				pc.warp(1104300,1788500)
			elseif s == 6 then
				say_title("Işınlanma Yüzüğü")
				local sssssss = select("Seungryong Vadisi", "Sohan Dağı", "Yongbi Çölü", "Doyyumhwan", "Devler Diyarı", "Şeytan Kulesi", "Şeytan Mezarlığı", "Kırmızı Ejderha Kalesi", "Nemere'nin Gözetleme Kulesi", "Vazgeç")
				if sssssss == 1 then
					pc.warp(336000,755600)
				elseif sssssss == 2 then
					pc.warp(436400,215600)
				elseif sssssss == 3 then
					pc.warp(296800,547400)
				elseif sssssss == 4 then
					pc.warp(600800,687400)
				elseif sssssss == 5 then
					pc.warp(829900,763300)
				elseif sssssss == 6 then
					pc.warp(590500,110500)
				elseif sssssss == 7 then
					pc.warp(591100,99300)
				elseif sssssss == 8 then
					pc.warp(614739,706929)
				elseif sssssss == 9 then
					pc.warp(432792,165998)
				end
			end
		end
	end
end"""

# 2. Sauvegarder en UTF-8 pour que qc.exe compile sans erreur (au root)
root_path = r"C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest"
quest_file = os.path.join(root_path, "ring_warp.quest")
official_path = os.path.join(root_path, "quest", "item", "ring_warp.quest")

# Ensure official directory exists
if not os.path.exists(os.path.dirname(official_path)):
    os.makedirs(os.path.dirname(official_path))

# Write to root (for compiler)
with open(quest_file, 'wb') as f:
    f.write(content.replace('\n', '\r\n').encode('utf-8'))
print(f"Quest file created for compiler: {quest_file}")

# Write to official path (for the user) - with actual TAB characters
with open(official_path, 'wb') as f:
    f.write(content.replace('\n', '\r\n').encode('utf-8'))
print(f"Quest file restored to official path with TABS: {official_path}")

# 3. Compiler avec qc.exe
os.chdir(root_path)
print("Running qc.exe...")
subprocess.call([r".\qc.exe", "ring_warp.quest"], shell=True)

# 4. Rechercher les fichiers générés dans object/
obj_path = os.path.join(root_path, "object", "40002", "use")
if not os.path.exists(obj_path):
    print(f"Error: Object path {obj_path} not found!")
    exit(1)

# Map des caractères turcs UTF-8 -> CP1254
charset_map = {
    u'İ': (u'İ'.encode('utf-8'), u'İ'.encode('cp1254')),
    u'ı': (u'ı'.encode('utf-8'), u'ı'.encode('cp1254')),
    u'Ş': (u'Ş'.encode('utf-8'), u'Ş'.encode('cp1254')),
    u'ş': (u'ş'.encode('utf-8'), u'ş'.encode('cp1254')),
    u'Ğ': (u'Ğ'.encode('utf-8'), u'Ğ'.encode('cp1254')),
    u'ğ': (u'ğ'.encode('utf-8'), u'ğ'.encode('cp1254')),
    u'Ö': (u'Ö'.encode('utf-8'), u'Ö'.encode('cp1254')),
    u'ö': (u'ö'.encode('utf-8'), u'ö'.encode('cp1254')),
    u'Ü': (u'Ü'.encode('utf-8'), u'Ü'.encode('cp1254')),
    u'ü': (u'ü'.encode('utf-8'), u'ü'.encode('cp1254')),
    u'Ç': (u'Ç'.encode('utf-8'), u'Ç'.encode('cp1254')),
    u'ç': (u'ç'.encode('utf-8'), u'ç'.encode('cp1254')),
}

# 5. Converter les fichiers .start, .when, .arg
for filename in os.listdir(obj_path):
    if filename.endswith(".start") or filename.endswith(".when") or filename.endswith(".arg"):
        file_path = os.path.join(obj_path, filename)
        print(f"Fixing encoding for: {filename}")
        
        with open(file_path, 'rb') as f:
            data = f.read()
            
        new_data = data
        for char, (utf8_bytes, cp1254_bytes) in charset_map.items():
            new_data = new_data.replace(utf8_bytes, cp1254_bytes)
            
        with open(file_path, 'wb') as f:
            f.write(new_data)
            
print("All done! Quest compiled and encoding fixed in binary objects (TABS used).")
