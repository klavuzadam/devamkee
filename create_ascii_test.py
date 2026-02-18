
import os

orig_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate.lua'
output_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_ascii.lua'

with open(orig_path, 'r', encoding='latin-1') as f:
    lines = f.readlines()

ascii_lines = []
for line in lines:
    # Keep the structure, but replace all string content with "TEST"
    if '"' in line:
        start = line.find('"')
        end = line.rfind('"')
        prefix = line[:start]
        suffix = line[end+1:]
        ascii_lines.append(f'{prefix}"TEST"{suffix}')
    else:
        # For lines without quotes, just keep them but ensure they are clean ASCII
        ascii_lines.append(line.encode('ascii', 'ignore').decode('ascii'))

with open(output_path, 'w', encoding='ascii') as f:
    f.writelines(ascii_lines)

print(f"ASCII version created: {output_path}")
