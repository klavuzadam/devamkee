# coding: latin_1

import player
import chat

def CheckBlockLevel():
    import chr
    if not chr.IsGameMaster():
        chat.AppendChat(chat.CHAT_TYPE_INFO, "Test 1")
    else:
        chat.AppendChat(chat.CHAT_TYPE_INFO, "Test 2")

    level = player.GetStatus(player.LEVEL)
    if level < 30:
        return False

    return True

def GetTextBlockLevel():
    chat.AppendChat(chat.CHAT_TYPE_INFO, "Tu nivel no es suficiente para realizar esta acción, necesitas ser level 30.")
