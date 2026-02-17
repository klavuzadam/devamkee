# -*- coding: utf-8 -*-
import os

def load_names(path):
    names = {}
    try:
        with open(path, 'r', encoding='cp1254', errors='ignore') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue
                parts = line.split('\t')
                if len(parts) >= 2:
                    try:
                        vnum = int(parts[0])
                        name = parts[1]
                        names[vnum] = name
                    except ValueError:
                        continue
    except Exception as e:
        print(f"Error loading {path}: {e}")
    return names

def sync_files():
    source_path = 'item_names.txt'
    target_path = 'item_names_old.txt'
    output_path = 'item_names_old_updated.txt'
    
    if not os.path.exists(source_path) or not os.path.exists(target_path):
        print("Source or target file not found.")
        return

    # Load source names
    source_names = load_names(source_path)
    print(f"Loaded {len(source_names)} names from {source_path}")

    updated_count = 0
    total_lines = 0

    try:
        with open(target_path, 'r', encoding='cp1254', errors='ignore') as f_in, \
             open(output_path, 'w', encoding='cp1254', errors='replace') as f_out:
            
            for line in f_in:
                total_lines += 1
                stripped_line = line.strip()
                if not stripped_line:
                    f_out.write(line)
                    continue
                
                parts = stripped_line.split('\t')
                if len(parts) >= 2:
                    try:
                        vnum = int(parts[0])
                        if vnum in source_names:
                            new_name = source_names[vnum]
                            if new_name != parts[1]:
                                # Reconstruct line with new name, preserving original format if possible
                                # Assuming VNUM\tNAME format
                                f_out.write(f"{vnum}\t{new_name}\n")
                                updated_count += 1
                            else:
                                f_out.write(line)
                        else:
                            # VNUM not in source, keep original line
                            f_out.write(line)
                    except ValueError:
                        # Parsing error, keep line
                        f_out.write(line)
                else:
                    # Line format matches roughly but maybe not VNUM\tNAME, keep it
                    f_out.write(line)
        
        print(f"Processed {total_lines} lines.")
        print(f"Updated {updated_count} names.")
        print(f"Updated file saved to: {output_path}")

    except Exception as e:
        print(f"Error processing files: {e}")

if __name__ == "__main__":
    sync_files()
