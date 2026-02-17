# -*- coding: utf-8 -*-
import os

def patch_client_source():
    file_path = r"C:\Users\Yeni\Desktop\DİL\Client Source\Source Client\UserInterface\PythonNetworkStreamPhaseGameActor.cpp"
    
    if not os.path.exists(file_path):
        print(f"Error: File not found at {file_path}")
        return

    print(f"Reading {file_path}...")
    with open(file_path, 'r', encoding='cp1254', errors='ignore') as f: # Source files usually ANSI/CP1254 in TR files
        lines = f.readlines()

    # Locate the RecvCharacterAdditionalInfo function or the specific block
    # We know it's around line 162 based on previous `view_file`
    
    target_line_index = -1
    for i, line in enumerate(lines):
        if "if(kNetActorData.m_dwVID == chrInfoPacket.dwVID)" in line:
            # Check context to be sure it's the one in RecvCharacterAdditionalInfo
            # The one we want is followed by `{` and then `kNetActorData.m_stName = ...`
            if i + 2 < len(lines) and "kNetActorData.m_stName" in lines[i+2] and "chrInfoPacket.name" in lines[i+2]:
                target_line_index = i
                break
    
    if target_line_index == -1:
        print("Error: Could not find the target code block.")
        return

    print(f"Found target block at line {target_line_index + 1}")
    
    # We want to replace the line:
    # kNetActorData.m_stName		= chrInfoPacket.name;
    # with our logic.
    
    # The line after `if` and `{`
    replace_index = target_line_index + 2
    original_line = lines[replace_index]
    print(f"Original line: {original_line.strip()}")
    
    # Indentation guess (usually 2 tabs)
    indent = "\t\t"
    
    new_code = [
        f"{indent}if (kNetActorData.m_bType == CActorInstance::TYPE_NPC)\n",
        f"{indent}{{\n",
        f"{indent}\tconst char* c_szName;\n",
        f"{indent}\tif (CPythonNonPlayer::Instance().GetName(kNetActorData.m_dwRace, &c_szName))\n",
        f"{indent}\t\tkNetActorData.m_stName = c_szName;\n",
        f"{indent}\telse\n",
        f"{indent}\t\tkNetActorData.m_stName = chrInfoPacket.name;\n",
        f"{indent}}}\n",
        f"{indent}else\n",
        f"{indent}{{\n",
        f"{indent}\tkNetActorData.m_stName = chrInfoPacket.name;\n",
        f"{indent}}}\n"
    ]
    
    # Replace the single line with multiple lines
    lines[replace_index] = "".join(new_code)
    
    print("Applying patch...")
    with open(file_path, 'w', encoding='cp1254') as f:
        f.writelines(lines)
    print("Done.")

if __name__ == "__main__":
    patch_client_source()
