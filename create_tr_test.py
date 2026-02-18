
import os

source_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_regenerated.lua'
output_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_tr_test.lua'

with open(source_path, 'r', encoding='latin-1') as f:
    lines = f.readlines()

test_lines = []
for i, line in enumerate(lines):
    # Keep the first 50 lines as they are (with Turkish characters)
    if i < 50:
        test_lines.append(line)
    else:
        # For the rest, use the ASCII "TEST" format
        if '"' in line:
            start = line.find('"')
            end = line.rfind('"')
            prefix = line[:start]
            suffix = line[end+1:]
            test_lines.append(f'{prefix}"TEST"{suffix}')
        else:
            test_lines.append(line)

with open(output_path, 'w', encoding='latin-1') as f:
    f.writelines(test_lines)

print(f"Turkish test version created: {output_path}")
