
path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_regenerated.lua'
content = open(path, 'r', encoding='latin-1').read()

curly_open = content.count('{')
curly_close = content.count('}')
square_open = content.count('[')
square_close = content.count(']')
quotes = content.count('"')

print(f"Curly: {curly_open} / {curly_close}")
print(f"Square: {square_open} / {square_close}")
print(f"Quotes: {quotes}")

if curly_open == curly_close and square_open == square_close and quotes % 2 == 0:
    print("VERIFICATION_SUCCESS: All brackets and quotes are balanced.")
else:
    print("VERIFICATION_FAILURE: Imbalance detected!")
