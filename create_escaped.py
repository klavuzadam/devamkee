
import os

source_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_regenerated.lua'
output_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_escaped.lua'

# We will read as bytes and escape everything > 127
with open(source_path, 'rb') as f:
    data = f.read()

def escape_non_ascii(data):
    result = []
    for b in data:
        if b > 127:
            # Lua escape format: \ddd
            result.append(f'\\{b:03d}'.encode('ascii'))
        else:
            result.append(bytes([b]))
    return b"".join(result)

escaped_data = escape_non_ascii(data)

with open(output_path, 'wb') as f:
    f.write(escaped_data)

print(f"Escaped version created: {output_path}")
