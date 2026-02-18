# coding: latin_1

import app

AUTOBAN_QUIZ_ANSWER = "ANSWER"
AUTOBAN_QUIZ_REFRESH = "REFRESH"
AUTOBAN_QUIZ_REST_TIME = "REST_TIME"

OPTION_SHADOW = "SHADOW"

CODEPAGE = str(app.GetDefaultCodePage())

#CUBE_TITLE = "Cube Window"

def LoadLocaleFile(srcFileName, localeDict):
    localeDict["CUBE_INFO_TITLE"] = "Recipe"
    localeDict["CUBE_REQUIRE_MATERIAL"] = "Requirements"
    localeDict["CUBE_REQUIRE_MATERIAL_OR"] = "or"

    try:
        lines = open(srcFileName, "r").readlines()
    except IOError:
        import dbg
        dbg.LogBox("LoadUIScriptLocaleError(%(srcFileName)s)" % locals())
        app.Abort()

    for line in lines:
        tokens = line[:-1].split("\t")
        if app.ENABLE_MULTI_TEXTLINE:
            for k in range(1, len(tokens)):
                tokens[k] = tokens[k].replace("\\n", "\n")

        if len(tokens) >= 2:
            localeDict[tokens[0]] = tokens[1]

        else:
            print len(tokens), lines.index(line), line


name = app.GetLocalePath()

LOCALE_UISCRIPT_PATH = "locale/en/ui/"
LOGIN_PATH = "locale/en/ui/login/"
EMPIRE_PATH = "locale/en/ui/empire/"
GUILD_PATH = "locale/en/ui/guild/"
SELECT_PATH = "locale/en/ui/select/"
WINDOWS_PATH = "locale/en/ui/windows/"
MAPNAME_PATH = "%s/ui/mapname/" % (name)

JOBDESC_WARRIOR_PATH = "%s/jobdesc_warrior.txt" % (name)
JOBDESC_ASSASSIN_PATH = "%s/jobdesc_assassin.txt" % (name)
JOBDESC_SURA_PATH = "%s/jobdesc_sura.txt" % (name)
JOBDESC_SHAMAN_PATH = "%s/jobdesc_shaman.txt" % (name)

JOBDESC_WOLFMAN_PATH = "%s/jobdesc_wolfman.txt" % (name)

EMPIREDESC_A = "%s/empiredesc_a.txt" % (name)
EMPIREDESC_B = "%s/empiredesc_b.txt" % (name)
EMPIREDESC_C = "%s/empiredesc_c.txt" % (name)
CARDS_DESC = "%s/mini_game_okey_desc.txt" % (name)

if app.ENABLE_MINI_GAME_CATCH_KING:
    MINIGAME_CATCH_KING_DESC = "%s/catchking_event_desc.txt" % (name)
    MINIGAME_CATCH_KING_SIMPLE_DESC = "%s/catchking_event_simple_desc.txt" % (name)
LOCALE_INTERFACE_FILE_NAME = "%s/locale_interface.txt" % (name)
if app.__BL_BATTLE_ROYALE__:
	BATTLE_ROYALE_EVENT_DESC = "%s/battle_royale_desc.txt" % (name)
LoadLocaleFile(LOCALE_INTERFACE_FILE_NAME, locals())

def Reload():
    LoadLocaleFile("%s/locale_interface.txt" % app.GetLocalePath(), globals())

