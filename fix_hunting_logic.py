
import os

path = r'C:\Users\Yeni\Desktop\DİL\Server\share\locale\poland\quest\libs\translate\translate_escaped.lua'

with open(path, 'rb') as f:
    content = f.read()

# Original Polish:
# letterName = "Polowanie %s"
# special_letter_reward_exp = "- Doœwiadczenie: %d%% wymaganego doœ. na poziomie %d."
# special_letter_reward_item = "- Przedmiot: %s"

# Current Turkish (incorrect placeholders/newlines):
# letterName = "Avc\253l\253k: %s"  (Includes a colon not in orig)
# special_letter_reward_exp = "- Tecr\252be: %2$d. seviyede gereken TP'nin %%%1$d kadar\253." (Swapped args and extra %)
# special_letter_reward_item = "- E\254ya: %s"

# Fix letterName (remove colon just in case it's parsed)
content = content.replace(b'letterName = "Avc\\253l\\253k: %s"', b'letterName = "Avc\\253l\\253k %s"')

# Fix reward_exp (match orig sequence %d then %d)
# Orig: %d%% ... %d
# New: %d%% ... %d
tr_exp_fixed = b'special_letter_reward_exp = "- Tecr\\252be: %d. seviyede gereken TP\'nin %%%d kadar\\253."'
# Wait, let's look at the wording. 
# "Tecrübe: %d. seviyede gereken TP'nin %%%d kadarı."
# If %1 is percentage and %2 is level:
# "Tecrübe: %d%% (level %d)"
content = content.replace(b'special_letter_reward_exp = "- Tecr\\252be: %2$d. seviyede gereken TP\'nin %%%1$d kadar\\253."', 
                        b'special_letter_reward_exp = "- Tecr\\252be: %d. seviyede gereken TP\'nin %%%d kadar\\253."')

with open(path, 'wb') as f:
    f.write(content)

print("Corrected hunting quest placeholders.")
