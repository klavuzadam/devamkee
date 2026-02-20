
import os

file_path = r"c:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\skilldesc.txt"
temp_path = file_path + ".tmp"

with open(file_path, "rb") as f:
    content = f.read()

# Try common encodings
encodings = ["windows-1254", "utf-8", "ansi"]
decoded_content = None

for enc in encodings:
    try:
        decoded_content = content.decode(enc)
        print(f"Detected encoding: {enc}")
        break
    except UnicodeDecodeError:
        continue

if decoded_content:
    lines = decoded_content.splitlines()
    new_lines = []
    found = False
    for line in lines:
        # Check for Item Production (ID 123)
        if line.startswith("123\tSUPPORT\tE") and "retimi" in line:
            new_lines.append("//" + line)
            found = True
        else:
            new_lines.append(line)
    
    if found:
        with open(temp_path, "w", encoding=enc) as f:
            f.write("\n".join(new_lines) + "\n")
        
        os.remove(file_path)
        os.rename(temp_path, file_path)
        print("Successfully commented out Item Production skill.")
    else:
        print("Skill ID 123 not found in skilldesc.txt.")
else:
    print("Could not decode file with any common encoding.")
