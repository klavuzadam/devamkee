# coding: latin_1

import localeInfo
import player

EMOTION_CLAP = 1
EMOTION_CONGRATULATION = 2
EMOTION_FORGIVE = 3
EMOTION_ANGRY = 4
EMOTION_ATTRACTIVE = 5
EMOTION_SAD = 6
EMOTION_SHY = 7
EMOTION_CHEERUP = 8
EMOTION_BANTER = 9
EMOTION_JOY = 10
EMOTION_CHEERS_1 = 11
EMOTION_CHEERS_2 = 12
EMOTION_DANCE_1 = 13
EMOTION_DANCE_2 = 14
EMOTION_DANCE_3 = 15
EMOTION_DANCE_4 = 16
EMOTION_DANCE_5 = 17
EMOTION_DANCE_6 = 18
EMOTION_KISS = 51
EMOTION_FRENCH_KISS = 52
EMOTION_SLAP = 53

EMOTION_DICT = {
    EMOTION_CLAP : {"name": localeInfo.EMOTION_CLAP, "command":"/clap"},
    EMOTION_DANCE_1 : {"name": localeInfo.EMOTION_DANCE_1, "command":"/dance1"},
    EMOTION_DANCE_2 : {"name": localeInfo.EMOTION_DANCE_2, "command":"/dance2"},
    EMOTION_DANCE_3 : {"name": localeInfo.EMOTION_DANCE_3, "command":"/dance3"},
    EMOTION_DANCE_4 : {"name": localeInfo.EMOTION_DANCE_4, "command":"/dance4"},
    EMOTION_DANCE_5 : {"name": localeInfo.EMOTION_DANCE_5, "command":"/dance5"},
    EMOTION_DANCE_6 : {"name": localeInfo.EMOTION_DANCE_6, "command":"/dance6"},
    EMOTION_CONGRATULATION : {"name": localeInfo.EMOTION_CONGRATULATION, "command":"/congratulation"},
    EMOTION_FORGIVE : {"name": localeInfo.EMOTION_FORGIVE, "command":"/forgive"},
    EMOTION_ANGRY : {"name": localeInfo.EMOTION_ANGRY, "command":"/angry"},
    EMOTION_ATTRACTIVE : {"name": localeInfo.EMOTION_ATTRACTIVE, "command":"/attractive"},
    EMOTION_SAD : {"name": localeInfo.EMOTION_SAD, "command":"/sad"},
    EMOTION_SHY : {"name": localeInfo.EMOTION_SHY, "command":"/shy"},
    EMOTION_CHEERUP : {"name": localeInfo.EMOTION_CHEERUP, "command":"/cheerup"},
    EMOTION_BANTER : {"name": localeInfo.EMOTION_BANTER, "command":"/banter"},
    EMOTION_JOY : {"name": localeInfo.EMOTION_JOY, "command":"/joy"},
    EMOTION_CHEERS_1 : {"name": localeInfo.EMOTION_CHEERS_1, "command":"/cheer1"},
    EMOTION_CHEERS_2 : {"name": localeInfo.EMOTION_CHEERS_2, "command":"/cheer2"},
    EMOTION_KISS : {"name": localeInfo.EMOTION_CLAP_KISS, "command":"/kiss"},
    EMOTION_FRENCH_KISS : {"name": localeInfo.EMOTION_FRENCH_KISS, "command":"/french_kiss"},
    EMOTION_SLAP : {"name": localeInfo.EMOTION_SLAP, "command":"/slap"},
}

ICON_DICT = {
    EMOTION_CLAP : "d:/ymir work/ui/game/windows/emotion_clap.sub",
    EMOTION_CHEERS_1 : "d:/ymir work/ui/game/windows/emotion_cheers_1.sub",
    EMOTION_CHEERS_2 : "d:/ymir work/ui/game/windows/emotion_cheers_2.sub",
    EMOTION_DANCE_1 : "icon/action/dance1.tga",
    EMOTION_DANCE_2 : "icon/action/dance2.tga",
    EMOTION_CONGRATULATION : "icon/action/congratulation.tga",
    EMOTION_FORGIVE : "icon/action/forgive.tga",
    EMOTION_ANGRY : "icon/action/angry.tga",
    EMOTION_ATTRACTIVE : "icon/action/attractive.tga",
    EMOTION_SAD : "icon/action/sad.tga",
    EMOTION_SHY : "icon/action/shy.tga",
    EMOTION_CHEERUP : "icon/action/cheerup.tga",
    EMOTION_BANTER : "icon/action/banter.tga",
    EMOTION_JOY : "icon/action/joy.tga",
    EMOTION_DANCE_1 : "icon/action/dance1.tga",
    EMOTION_DANCE_2 : "icon/action/dance2.tga",
    EMOTION_DANCE_3 : "icon/action/dance3.tga",
    EMOTION_DANCE_4 : "icon/action/dance4.tga",
    EMOTION_DANCE_5 : "icon/action/dance5.tga",
    EMOTION_DANCE_6 : "icon/action/dance6.tga",
    EMOTION_KISS : "d:/ymir work/ui/game/windows/emotion_kiss.sub",
    EMOTION_FRENCH_KISS : "d:/ymir work/ui/game/windows/emotion_french_kiss.sub",
    EMOTION_SLAP : "d:/ymir work/ui/game/windows/emotion_slap.sub",
}

def RegisterEmotionIcons():
    for key, val in ICON_DICT.items():
        player.RegisterEmotionIcon(key, val)
