import os

def replace_in_file(filepath, old_bytes, new_bytes):
    if not os.path.exists(filepath):
        print(f"File not found: {filepath}")
        return
    with open(filepath, 'rb') as f:
        content = f.read()
    
    if old_bytes in content:
        new_content = content.replace(old_bytes, new_bytes)
        with open(filepath, 'wb') as f:
            f.write(new_content)
        print(f"Updated: {os.path.basename(filepath)}")
    else:
        print(f"Target not found in: {os.path.basename(filepath)}")

base_dir = r"C:\Users\Yeni\Desktop\DİL\Client Source\Source Client\UserInterface"

# PythonApplication.cpp
replace_in_file(
    os.path.join(base_dir, "PythonApplication.cpp"),
    b'if (currentLanguage != "pl")',
    b'if (currentLanguage != "tr")'
)

# PythonSystem.cpp
replace_in_file(
    os.path.join(base_dir, "PythonSystem.cpp"),
    b'm_Config.language = std::string("pl");',
    b'm_Config.language = std::string("tr");'
)

# PythonNetworkStreamPhaseGame.cpp
replace_in_file(
    os.path.join(base_dir, "PythonNetworkStreamPhaseGame.cpp"),
    b'discordPresence.buttonLabel = "Dolacz do gry!";',
    b'discordPresence.buttonLabel = "Oyuna Katil!";'
)
replace_in_file(
    os.path.join(base_dir, "PythonNetworkStreamPhaseGame.cpp"),
    b'discordPresence.buttonURL = "https://mt2009.pl/";',
    b'discordPresence.buttonURL = "https://mt2009.tr/";'
)
