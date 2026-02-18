
import re

orig_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate.lua'
tr_source_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate - Kopya.lua'
output_path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_regenerated.lua'

def extract_content(line):
    # Find the content between the first and last double quote
    start = line.find('"')
    end = line.rfind('"')
    if start != -1 and end != -1 and start != end:
        return line[start+1:end]
    return None

with open(orig_path, 'r', encoding='latin-1') as f:
    orig_lines = f.readlines()

with open(tr_source_path, 'r', encoding='latin-1') as f:
    tr_lines = f.readlines()

final_lines = []
for i in range(len(orig_lines)):
    orig_line = orig_lines[i]
    if i < len(tr_lines):
        tr_line = tr_lines[i]
        
        # We only care about lines with quotes
        if '"' in orig_line:
            tr_val = extract_content(tr_line)
            if tr_val is not None:
                first_q = orig_line.find('"')
                last_q = orig_line.rfind('"')
                prefix = orig_line[:first_q]
                suffix = orig_line[last_q+1:]
                # Reconstruct using Polish structure but Turkish value
                final_lines.append(f'{prefix}"{tr_val}"{suffix}')
            else:
                final_lines.append(orig_line)
        else:
            final_lines.append(orig_line)
    else:
        final_lines.append(orig_line)

with open(output_path, 'w', encoding='latin-1') as f:
    f.writelines(final_lines)

print(f"File regenerated successfully: {output_path}")
print(f"Processed {len(final_lines)} lines.")
