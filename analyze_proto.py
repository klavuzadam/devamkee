
import struct
import os

def analyze_proto(path):
    if not os.path.exists(path):
        print(f"File not found: {path}")
        return
    
    print(f"Analyzing {path}...")
    try:
        with open(path, "rb") as f:
            data = f.read(20) # Read first 5 DWORDS
            if len(data) < 20:
                print("  File too small")
                return
            
            fourcc, version, stride, elements, data_size = struct.unpack("<IIIII", data)
            
            # Convert fourcc to string
            fourcc_str = "".join([chr((fourcc >> (i * 8)) & 0xFF) if 32 <= (fourcc >> (i * 8)) & 0xFF <= 126 else '?' for i in range(4)])
            
            print(f"  FourCC: {fourcc_str} (0x{fourcc:08X})")
            print(f"  Version: {version}")
            print(f"  Stride: {stride}")
            print(f"  Elements: {elements}")
            print(f"  Encrypted Data Size: {data_size}")
            
            total_real_size = stride * elements
            print(f"  Computed Real Size: {total_real_size}")
            
    except Exception as e:
        print(f"  Error: {e}")

base_dir = r"C:\Users\Yeni\Desktop\DİL\Client"
analyze_proto(os.path.join(base_dir, "pack", "item_proto"))
analyze_proto(os.path.join(base_dir, "pack", "gamedata", "gamedata", "item_proto"))
analyze_proto(os.path.join(base_dir, "pack", "gamedata", "item_proto"))
