
path_orig = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate.lua'
path_reg = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_regenerated.lua'

orig_text = open(path_orig, 'r', encoding='latin-1').read()
reg_text = open(path_reg, 'r', encoding='latin-1').read()

print(f"Orig quotes: {orig_text.count('\"')}")
print(f"Reg quotes : {reg_text.count('\"')}")
print(f"Orig { '{' }: {orig_text.count('{')}")
print(f"Reg { '{' } : {reg_text.count('{')}")
print(f"Orig { '[' }: {orig_text.count('[')}")
print(f"Reg { '[' } : {reg_text.count('[')}")
