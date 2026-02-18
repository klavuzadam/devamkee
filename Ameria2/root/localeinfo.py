# coding: latin_1

import app

def ShopSecondsToDHM(time):
	if time < 60:
		if IsARABIC():
			return "%.2f%s" % (time, IKASHOP_SECOND)
		else:
			return "0" + IKASHOP_MINUTE

	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	day = int(int((time / 60) / 60) / 24)

	text = ""

	if day > 0:
		text += str(day) + IKASHOP_DAY
		text += " "

	if hour > 0:
		text += str(hour) + IKASHOP_HOUR
		text += " "

	if minute > 0:
		text += str(minute) + IKASHOP_MINUTE

	return text

def NumberToWithItemString(n,c) :
	if n <= 0 :
		return "0 %s" % (c)
	return "%s x | %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), c)

def NumberToString(n):
	if n < 0:
		return "-" + NumberToString(-n)

	ret = ""
	while n >= 1000:
		r = n % 1000
		ret = (".%03d"%r) + ret
		n //= 1000

	return str(n) + ret

def DottedNumber(n) :
	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ])) 

def SecondToDHMSNew(time):
	if time < 60:
		return "0" + MINUTE

	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	day = int(int((time / 60) / 60) / 24)
	text = ""

	if day > 0:
		text += str(day) + DAY
		text += " "

	if hour > 0:
		text += str(hour) + HOUR
		text += " "

	if minute > 0:
		text += str(minute) + MINUTE

	if second > 0:
		text += str(second) + "s"

	return text

def SecondToMS(time):
	if time < 60:
		return "%d%s" % (time, SECOND)

	second = int(time % 60)
	minute = int((time / 60) % 60)

	text = ""

	if minute > 0:
		text += str(minute) + MINUTE
		if minute > 0:
			text += " "

	if second > 0:
		text += str(second) + SECOND

	return text

MAP_TRENT02 = "MAP_TRENT02" # ÀÓ½Ã
MAP_WL = "MAP_WL" # ÀÓ½Ã
MAP_NUSLUCK = "MAP_NUSLUCK" # ÀÓ½Ã
MAP_TREE2 = "MAP_TREE2"

BLEND_POTION_NO_TIME = "BLEND_POTION_NO_TIME"
BLEND_POTION_NO_INFO = "BLEND_POTION_NO_INFO"

APP_TITLE = "Ameria.to # Battle Royale"

GUILD_HEADQUARTER = "Main Building"
GUILD_FACILITY = "Facility"
GUILD_OBJECT = "Object"
GUILD_MEMBER_COUNT_INFINITY = "INFINITY"

LOGIN_FAILURE_WEB_BLOCK = "BLOCK_LOGIN(WEB)"
LOGIN_FAILURE_BLOCK_LOGIN = "BLOCK_LOGIN"
CHANNEL_NOTIFY_FULL = "CHANNEL_NOTIFY_FULL"

GUILD_BUILDING_LIST_TXT = app.GetLocalePath() + "/GuildBuildingList.txt"

GUILD_MARK_MIN_LEVEL = "3"
GUILD_MARK_NOT_ENOUGH_LEVEL = "±æµå·¹º§ 3ÀÌ»ó ºÎÅÍ °¡´ÉÇÕ´Ï´Ù."

ERROR_MARK_UPLOAD_NEED_RECONNECT = "UploadMark: Reconnect to game"
ERROR_MARK_CHECK_NEED_RECONNECT = "CheckMark: Reconnect to game"

VIRTUAL_KEY_ALPHABET_LOWERS  = r"[1234567890]/qwertyuiop\=asdfghjkl;`'zxcvbnm.,"
VIRTUAL_KEY_ALPHABET_UPPERS  = r'{1234567890}?QWERTYUIOP|+ASDFGHJKL:~"ZXCVBNM<>'
VIRTUAL_KEY_SYMBOLS = '!@#$%^&*()_+|{}:"<>?~'
VIRTUAL_KEY_NUMBERS = "1234567890-=\[];',./`"
VIRTUAL_KEY_SYMBOLS_BR = '!@#$%^&*()_+|{}:"<>?~áàãâéèêíìóòôõúùç'

__IS_ENGLISH = "ENGLISH" == app.GetLocaleServiceName()
__IS_HONGKONG = "HONGKONG" == app.GetLocaleServiceName()
__IS_NEWCIBN = "locale/newcibn" == app.GetLocalePath()
__IS_EUROPE = "EUROPE" == app.GetLocaleServiceName()
__IS_CANADA = "locale/ca" == app.GetLocalePath()
__IS_BRAZIL = "locale/br" == app.GetLocalePath()
__IS_SINGAPORE = "locale/sg" == app.GetLocalePath()
__IS_VIETNAM = "locale/vn" == app.GetLocalePath()
__IS_ARABIC = "locale/ae" == app.GetLocalePath()
__IS_CIBN10 = "locale/cibn10" == app.GetLocalePath()
__IS_WE_KOREA = "locale/we_korea" == app.GetLocalePath()
__IS_TAIWAN = "locale/taiwan" == app.GetLocalePath()
__IS_JAPAN = "locale/japan" == app.GetLocalePath()
LOGIN_FAILURE_WRONG_SOCIALID = "ASDF"
LOGIN_FAILURE_SHUTDOWN_TIME = "ASDF"

if __IS_CANADA:
	__IS_EUROPE = True



def IsYMIR():
	return "locale/ymir" == app.GetLocalePath()

def IsJAPAN():
	return "locale/japan" == app.GetLocalePath()

def IsENGLISH():
	global __IS_ENGLISH
	return __IS_ENGLISH

def IsHONGKONG():
	global __IS_HONGKONG
	return __IS_HONGKONG

def IsTAIWAN():
	return "locale/taiwan" == app.GetLocalePath()

def IsNEWCIBN():
	return "locale/newcibn" == app.GetLocalePath()

def IsCIBN10():
	global __IS_CIBN10
	return __IS_CIBN10

def IsEUROPE():
	global __IS_EUROPE
	return __IS_EUROPE

def IsCANADA():
	global __IS_CANADA
	return __IS_CANADA

def IsBRAZIL():
	global __IS_BRAZIL
	return __IS_BRAZIL

def IsVIETNAM():
	global __IS_VIETNAM
	return __IS_VIETNAM

def IsSINGAPORE():
	global __IS_SINGAPORE
	return __IS_SINGAPORE

def IsARABIC():
	global __IS_ARABIC
	return __IS_ARABIC

def IsWE_KOREA():
	return "locale/we_korea" == app.GetLocalePath()

# SUPPORT_NEW_KOREA_SERVER
def LoadLocaleData():
	if IsYMIR():
		import net
		SERVER = "Äèµµ ¼­¹ö"
		if SERVER == net.GetServerInfo()[:len(SERVER)]:
			app.SetCHEONMA(0)
			app.LoadLocaleData("locale/we_korea")
		else:
			app.SetCHEONMA(1)
			app.LoadLocaleData("locale/ymir")
	else:
		app.LoadLocaleData(app.GetLocalePath())

def IsCHEONMA():
	return IsYMIR()		# ÀÌÁ¦ YMIR ·ÎÄÉÀÏÀº ¹«Á¶°Ç Ãµ¸¶¼­¹öÀÓ. Ãµ¸¶¼­¹ö°¡ ¹®À» ´Ý±â Àü±îÁö º¯ÇÒ ÀÏ ¾øÀ½.

# END_OF_SUPPORT_NEW_KOREA_SERVER

def mapping(**kwargs): return kwargs

def SNA(text):
	def f(x):
		return text
	return f

def SA(text):
	def f(x):
		return text % x
	return f
import app
if app.ENABLE_BIOLOG_SYSTEM:
	def FormatTime(time):
		m, s = divmod(time, 60)
		h, m = divmod(m, 60)
		return "%02d:%02d:%02d" % (h, m, s)	
## PET_SKILL_INFO FNCS BEGIN
def SAN(text):
	def f(x):
		return text % x
	return f

def SAA(text):
	def f(x):
		return text % x
	return f
## PET_SKILL_INFO FNCS END

def LoadLocaleFile(srcFileName, localeDict):

	funcDict = {"SA":SA, "SNA":SNA, "SAA":SAA, "SAN":SAN}

	lineIndex = 1

	try:
		lines = open(srcFileName, "r").readlines()
	except IOError:
		import dbg
		dbg.LogBox("LoadLocaleError(%(srcFileName)s)" % locals())
		app.Abort()

	for line in lines:
		try:
			tokens = line[:-1].split("\t")
			if app.ENABLE_MULTI_TEXTLINE:
				for k in range(1, len(tokens)):
					tokens[k] = tokens[k].replace("\\n", "\n")
			if len(tokens) == 2:
				localeDict[tokens[0]] = tokens[1]
			elif len(tokens) >= 3:
				type = tokens[2].strip()
				if type:
					localeDict[tokens[0]] = funcDict[type](tokens[1])
				else:
					localeDict[tokens[0]] = tokens[1]
			else:
				raise RuntimeError, "Unknown TokenSize"

			lineIndex += 1
		except:
			import dbg
			dbg.LogBox("%s: line(%d): %s" % (srcFileName, lineIndex, line), "Error")
			raise

all = ["locale","error"]

FN_GM_MARK = "locale/en/effect/gm.mse"
LOCALE_FILE_NAME = "%s/locale_game.txt" % app.GetLocalePath()


LoadLocaleFile(LOCALE_FILE_NAME, locals())
if app.ENABLE_MULTILANGUAGE_SYSTEM:
	LC={}
	QUEST={}
	def LoadLocaleStringFile(srcFileName):
		lineIndex = 1
		try:
			lines = open(srcFileName, "r").readlines()
		except IOError:
			import dbg
			dbg.LogBox("LoadLocaleError(%s)" % srcFileName)
			app.Abort()
		for line in lines:
			if line[0] == "#":
				continue
			try:
				tokens = line.split("\t")
				LC[int(tokens[0])]=tokens[1]
				lineIndex+=1
			except:
				import dbg
				dbg.LogBox("%s: line(%d): %s" % (srcFileName, lineIndex, line), "Error")
				raise
	def LoadLocaleQuestFile(srcFileName):
		lineIndex = 1
		try:
			lines = open(srcFileName, "r").readlines()
		except IOError:
			import dbg
			dbg.LogBox("LoadLocaleError(%s)" % srcFileName)
			app.Abort()

		for line in lines:
			if line[0] == "#":
				continue
			try:
				tokens = line.split("\t")
				QUEST[str(tokens[0])]=tokens[1]
				lineIndex+=1
			except:
				import dbg
				dbg.LogBox("%s: line(%d): %s" % (srcFileName, lineIndex, line), "Error")
				raise
	LoadLocaleStringFile("%s/locale_string.txt" % (app.GetLocalePath()))
	LoadLocaleQuestFile("%s/locale_quest.txt" % (app.GetLocalePath()))

def Reload():
	LoadLocaleFile("%s/locale_game.txt" % app.GetLocalePath(), globals())

########################################################################################################
## NOTE : ¾ÆÀÌÅÛÀ» ¹ö¸±¶§ "¹«¾ùÀ»/¸¦ ¹ö¸®½Ã°Ú½À´Ï±î?" ¹®ÀÚ¿­ÀÇ Á¶»ç ¼±ÅÃÀ» À§ÇÑ ÄÚµå
dictSingleWord = {
	"m":1, "n":1, "r":1, "M":1, "N":1, "R":1, "l":1, "L":1, "1":1, "3":1, "6":1, "7":1, "8":1, "0":1,
}

dictDoubleWord = {
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?v":1, "??":1, "??":1, "??":1, "?R":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "?x":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?m":1, "??":1, "??":1, "??":1, "?O":1, "??":1, "??":1, "??":1, "??":1, "?l":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?u":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "?X":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?o":1, "??":1, "??":1, "??":1, "?y":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?p":1,
	"¢®I":1, "¨Ïo":1, "¡§u":1, "??":1, "??":1, "?c":1, "??":1, "??":1, "??":1, "??":1, "¢®¨¡":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?R":1, "??":1, "??":1, "?n":1,
	"??":1, "i":1, "o":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?":1, "a":1, "??":1, "u":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?M":1,
	"?":1, "?":1, "¡§¡§":1, "??":1, "??":1, "??":1, "??":1, "?":1, "?":1, "?":1, "¨Ï|":1, "?m":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1,
	"?":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?":1, "?":1, "?":1, "??":1, "?O":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?":1, "??":1, "?":1, "?":1,
	"??":1, "??":1, "??":1, "??":1, "??":1, "?":1, "?":1, "?":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?":1, "??":1, "?":1, "?R":1, "?":1, "?":1, "?c":1,
	"??":1, "??":1, "??":1, "??":1, "?":1, "?":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "??":1, "?":1, "?":1, "??":1, "??":1, "?":1, "??":1, "??":1,
}


locale = mapping(
)


def GetAuxiliaryWordType(text):

	textLength = len(text)

	if textLength > 1:

		singleWord = text[-1]

		if (singleWord >= '0' and singleWord <= '9') or\
			(singleWord >= 'a' and singleWord <= 'z') or\
			(singleWord >= 'A' and singleWord <= 'Z'):
			if not dictSingleWord.has_key(singleWord):
				return 1

		elif dictDoubleWord.has_key(text[-2:]):
			return 1

	return 0



def CutMoneyString(sourceText, startIndex, endIndex, insertingText, backText):

	sourceLength = len(sourceText)

	if sourceLength < startIndex:
		return backText

	text = sourceText[max(0, sourceLength-endIndex):sourceLength-startIndex]

	if not text:
		return backText

	if long(text) <= 0:
		return backText

	text = str(int(text))

	if backText:
		backText = " " + backText

	return text + insertingText + backText

def SecondToDHM(time):
	if time < 60:
		if IsARABIC():
			return "%.2f %s" % (time, SECOND)
		else:
			return "0" + MINUTE

	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	day = int(int((time / 60) / 60) / 24)

	text = ""

	if day > 0:
		text += str(day) + DAY
		text += " "

	if hour > 0:
		text += str(hour) + HOUR
		text += " "

	if minute > 0:
		text += str(minute) + MINUTE

	return text


def SecondOfflineToDHM(time):
	if time < 60:
		if IsARABIC():
			return "%.2f %s" % (time, "S")
		else:
			return "0 M"

	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	day = int(int((time / 60) / 60) / 24)

	text = ""

	if day > 0:
		text += str(day) + "D"
		text += " "

	if hour > 0:
		text += str(hour) + "H"
		text += " "

	if minute > 0:
		text += str(minute) + "M"

	return text

def SecondToHM(time):

	if time < 60:
		if IsARABIC():
			return "%.2f %s" % (time, SECOND)
		else:
			return "0" + MINUTE

	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60)

	text = ""

	if hour > 0:
		text += str(hour) + HOUR
		if hour > 0:
			text += " "

	if minute > 0:
		text += str(minute) + MINUTE

	return text


def GetAlignmentTitleName(alignment):
	if alignment >= 12000:
		return TITLE_NAME_LIST[0]
	elif alignment >= 8000:
		return TITLE_NAME_LIST[1]
	elif alignment >= 4000:
		return TITLE_NAME_LIST[2]
	elif alignment >= 1000:
		return TITLE_NAME_LIST[3]
	elif alignment >= 0:
		return TITLE_NAME_LIST[4]
	elif alignment > -4000:
		return TITLE_NAME_LIST[5]
	elif alignment > -8000:
		return TITLE_NAME_LIST[6]
	elif alignment > -12000:
		return TITLE_NAME_LIST[7]

	return TITLE_NAME_LIST[8]


OPTION_PVPMODE_MESSAGE_DICT = {
	0 : PVP_MODE_NORMAL,
	1 : PVP_MODE_REVENGE,
	2 : PVP_MODE_KILL,
	3 : PVP_MODE_PROTECT,
	4 : PVP_MODE_GUILD,
	5 : PVP_MODE_FEAR,
}

error = mapping(
	CREATE_WINDOW = GAME_INIT_ERROR_MAIN_WINDOW,
	CREATE_CURSOR = GAME_INIT_ERROR_CURSOR,
	CREATE_NETWORK = GAME_INIT_ERROR_NETWORK,
	CREATE_ITEM_PROTO = GAME_INIT_ERROR_ITEM_PROTO,
	CREATE_MOB_PROTO = GAME_INIT_ERROR_MOB_PROTO,
	CREATE_NO_DIRECTX = GAME_INIT_ERROR_DIRECTX,
	CREATE_DEVICE = GAME_INIT_ERROR_GRAPHICS_NOT_EXIST,
	CREATE_NO_APPROPRIATE_DEVICE = GAME_INIT_ERROR_GRAPHICS_BAD_PERFORMANCE,
	CREATE_FORMAT = GAME_INIT_ERROR_GRAPHICS_NOT_SUPPORT_32BIT,
	NO_ERROR = ""
)


GUILDWAR_NORMAL_DESCLIST = [GUILD_WAR_USE_NORMAL_MAP, GUILD_WAR_LIMIT_30MIN, GUILD_WAR_WIN_CHECK_SCORE]
GUILDWAR_WARP_DESCLIST = [GUILD_WAR_USE_BATTLE_MAP, GUILD_WAR_WIN_WIPE_OUT_GUILD, GUILD_WAR_REWARD_POTION]
GUILDWAR_CTF_DESCLIST = [GUILD_WAR_USE_BATTLE_MAP, GUILD_WAR_WIN_TAKE_AWAY_FLAG1, GUILD_WAR_WIN_TAKE_AWAY_FLAG2, GUILD_WAR_REWARD_POTION]

MINIMAP_ZONE_NAME_DICT = {
	"metin2_map_a1" : MAP_A1,
	"map_a2" : MAP_A2,
	"metin2_map_a3" : MAP_A3,
	"metin2_map_b1" : MAP_B1,
	"map_b2" : MAP_B2,
	"metin2_map_b3" : MAP_B3,
	"metin2_map_c1" : MAP_C1,
	"map_c2" : MAP_C2,
	"metin2_map_c3" : MAP_C3,
	"map_n_snowm_01" : MAP_SNOW,
	"metin2_map_n_flame_01" : MAP_FLAME,
	"metin2_map_n_desert_01" : MAP_DESERT,
	"metin2_map_milgyo" : MAP_TEMPLE,
	"metin2_map_spiderdungeon" : MAP_SPIDER,
	"metin2_map_deviltower1" : MAP_SKELTOWER,
	"metin2_map_guild_01" : MAP_AG,
	"metin2_map_guild_02" : MAP_BG,
	"metin2_map_guild_03" : MAP_CG,
	"metin2_map_trent" : MAP_TREE,
	"metin2_map_trent02" : MAP_TREE2,
	"season1/metin2_map_WL_01" : MAP_WL,
	"season1/metin2_map_nusluck01" : MAP_NUSLUCK,
	"Metin2_map_CapeDragonHead" : MAP_CAPE,
	"metin2_map_Mt_Thunder" : MAP_THUNDER,
	"metin2_map_dawnmistwood" : MAP_DAWN,
	"metin2_map_BayBlackSand" : MAP_BAY,
	"metin2_map_snakevalley" : MAP_DESERT,
	"metin2_map_dawnmist_dungeon_01" : MAP_DAWN,
}
if app.__BL_BATTLE_ROYALE__:
	MINIMAP_ZONE_NAME_DICT["metin2_map_battleroyale"] = MAP_BATTLE_ROYALE

JOBINFO_TITLE = [
	[JOB_WARRIOR0, JOB_WARRIOR1, JOB_WARRIOR2,],
	[JOB_ASSASSIN0, JOB_ASSASSIN1, JOB_ASSASSIN2,],
	[JOB_SURA0, JOB_SURA1, JOB_SURA2,],
	[JOB_SHAMAN0, JOB_SHAMAN1, JOB_SHAMAN2,],
]
if app.ENABLE_WOLFMAN_CHARACTER:
	JOBINFO_TITLE += [[JOB_WOLFMAN0,JOB_WOLFMAN1,JOB_WOLFMAN2,],]

WHISPER_ERROR = {
	1 : CANNOT_WHISPER_NOT_LOGON,
	2 : CANNOT_WHISPER_DEST_REFUSE,
	3 : CANNOT_WHISPER_SELF_REFUSE,
}

if app.__AUTO_QUQUE_ATTACK__:
	METIN_STONE_QUEUE_MSG_DICT = {
		"REMOVED" : METIN_STONE_QUEUE_MSG_REMOVED,
		"MAX" : METIN_STONE_QUEUE_MSG_MAX,
		"ADDED" : METIN_STONE_QUEUE_MSG_ADDED,
	}

NOTIFY_MESSAGE = {
	"CANNOT_EQUIP_SHOP" : CANNOT_EQUIP_IN_SHOP,
	"CANNOT_EQUIP_EXCHANGE" : CANNOT_EQUIP_IN_EXCHANGE,
}

ATTACK_ERROR_TAIL_DICT = {
	"IN_SAFE" : CANNOT_ATTACK_SELF_IN_SAFE,
	"DEST_IN_SAFE" : CANNOT_ATTACK_DEST_IN_SAFE,
}

SHOT_ERROR_TAIL_DICT = {
	"EMPTY_ARROW" : CANNOT_SHOOT_EMPTY_ARROW,
	"IN_SAFE" : CANNOT_SHOOT_SELF_IN_SAFE,
	"DEST_IN_SAFE" : CANNOT_SHOOT_DEST_IN_SAFE,
}

USE_SKILL_ERROR_TAIL_DICT = {
	"IN_SAFE" : CANNOT_SKILL_SELF_IN_SAFE,
	"NEED_TARGET" : CANNOT_SKILL_NEED_TARGET,
	"NEED_EMPTY_BOTTLE" : CANNOT_SKILL_NEED_EMPTY_BOTTLE,
	"NEED_POISON_BOTTLE" : CANNOT_SKILL_NEED_POISON_BOTTLE,
	"REMOVE_FISHING_ROD" : CANNOT_SKILL_REMOVE_FISHING_ROD,
	"NOT_YET_LEARN" : CANNOT_SKILL_NOT_YET_LEARN,
	"NOT_MATCHABLE_WEAPON" : CANNOT_SKILL_NOT_MATCHABLE_WEAPON,
	"WAIT_COOLTIME" : CANNOT_SKILL_WAIT_COOLTIME,
	"NOT_ENOUGH_HP" : CANNOT_SKILL_NOT_ENOUGH_HP,
	"NOT_ENOUGH_SP" : CANNOT_SKILL_NOT_ENOUGH_SP,
	"CANNOT_USE_SELF" : CANNOT_SKILL_USE_SELF,
	"ONLY_FOR_ALLIANCE" : CANNOT_SKILL_ONLY_FOR_ALLIANCE,
	"CANNOT_ATTACK_ENEMY_IN_SAFE_AREA" : CANNOT_SKILL_DEST_IN_SAFE,
	"CANNOT_APPROACH" : CANNOT_SKILL_APPROACH,
	"CANNOT_ATTACK" : CANNOT_SKILL_ATTACK,
	"ONLY_FOR_CORPSE" : CANNOT_SKILL_ONLY_FOR_CORPSE,
	"EQUIP_FISHING_ROD" : CANNOT_SKILL_EQUIP_FISHING_ROD,
	"NOT_HORSE_SKILL" : CANNOT_SKILL_NOT_HORSE_SKILL,
	"HAVE_TO_RIDE" : CANNOT_SKILL_HAVE_TO_RIDE,
}

LEVEL_LIST=["", HORSE_LEVEL1, HORSE_LEVEL2, HORSE_LEVEL3]

HEALTH_LIST=[
	HORSE_HEALTH0,
	HORSE_HEALTH1,
	HORSE_HEALTH2,
	HORSE_HEALTH3,
]


USE_SKILL_ERROR_CHAT_DICT = {
	"NEED_EMPTY_BOTTLE" : SKILL_NEED_EMPTY_BOTTLE,
	"NEED_POISON_BOTTLE" : SKILL_NEED_POISON_BOTTLE,
	"ONLY_FOR_GUILD_WAR" : SKILL_ONLY_FOR_GUILD_WAR,
}

SHOP_ERROR_DICT = {
	"NOT_ENOUGH_MONEY" : SHOP_NOT_ENOUGH_MONEY,
	"SOLDOUT" : SHOP_SOLDOUT,
	"INVENTORY_FULL" : SHOP_INVENTORY_FULL,
	"INVALID_POS" : SHOP_INVALID_POS,
	"NOT_ENOUGH_MONEY_EX" : SHOP_NOT_ENOUGH_MONEY_EX,
	"NOT_ENOUGH_LUCKY" : SHOP_NOT_ENOUGH_LUCKY,
}
if (app.WJ_COMBAT_ZONE):
	SHOP_ERROR_DICT.update({
		"NOT_ENOUGH_POINTS" : COMBAT_ZONE_SHOP_NOT_ENOUGH_BATTLE_POINT,
		"MAX_LIMIT_POINTS" : COMBAT_ZONE_SHOP_EXCEED_LIMIT_TODAY,
		"OVERFLOW_LIMIT_POINTS" : COMBAT_ZONE_SHOP_OVERFLOW_LIMIT_TODAY
		}
	)

STAT_MINUS_DESCRIPTION = {
	"HTH-" : STAT_MINUS_CON,
	"INT-" : STAT_MINUS_INT,
	"STR-" : STAT_MINUS_STR,
	"DEX-" : STAT_MINUS_DEX,
}

MODE_NAME_LIST = ( PVP_OPTION_NORMAL, PVP_OPTION_REVENGE, PVP_OPTION_KILL, PVP_OPTION_PROTECT, )
TITLE_NAME_LIST = ( PVP_LEVEL0, PVP_LEVEL1, PVP_LEVEL2, PVP_LEVEL3, PVP_LEVEL4, PVP_LEVEL5, PVP_LEVEL6, PVP_LEVEL7, PVP_LEVEL8, )

def GetLetterImageName():
	return "season1/icon/scroll_close.tga"
def GetLetterOpenImageName():
	return "season1/icon/scroll_open.tga"
def GetLetterCloseImageName():
	return "season1/icon/scroll_close.tga"
	
if (app.WJ_COMBAT_ZONE):
	def NumberToCombatZoneCoinString(n) :
		if n <= 0 :
			return "0 %s" % (MONETARY_COMBAT_ZONE)
		return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_COMBAT_ZONE)


def DO_YOU_SELL_ITEM(sellItemName, sellItemCount, sellItemPrice):
	if sellItemCount > 1 :
		return DO_YOU_SELL_ITEM2 % (sellItemName, sellItemCount, NumberToMoneyString(sellItemPrice) )
	else:
		return DO_YOU_SELL_ITEM1 % (sellItemName, NumberToMoneyString(sellItemPrice) )

def DO_YOU_BUY_ITEM(buyItemName, buyItemCount, buyItemPrice) :
	if buyItemCount > 1 :
		return DO_YOU_BUY_ITEM2 % ( buyItemName, buyItemCount, buyItemPrice )
	else:
		return DO_YOU_BUY_ITEM1 % ( buyItemName, buyItemPrice )

def REFINE_FAILURE_CAN_NOT_ATTACH(attachedItemName) :
	return REFINE_FAILURE_CAN_NOT_ATTACH0 % (attachedItemName)

def REFINE_FAILURE_NO_SOCKET(attachedItemName) :
	return REFINE_FAILURE_NO_SOCKET0 % (attachedItemName)

def REFINE_FAILURE_NO_GOLD_SOCKET(attachedItemName) :
	return REFINE_FAILURE_NO_GOLD_SOCKET0 % (attachedItemName)

def HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, dropItemCount) :
	if dropItemCount > 1 :
		return HOW_MANY_ITEM_DO_YOU_DROP2 % (dropItemName, dropItemCount)
	else :
		return HOW_MANY_ITEM_DO_YOU_DROP1 % (dropItemName)

def FISHING_NOTIFY(isFish, fishName) :
	if isFish :
		return FISHING_NOTIFY1 % ( fishName )
	else :
		return FISHING_NOTIFY2 % ( fishName )

def FISHING_SUCCESS(isFish, fishName) :
	if isFish :
		return FISHING_SUCCESS1 % (fishName)
	else :
		return FISHING_SUCCESS2 % (fishName)

def NumberToMoneyString(n):
	if n > 0:
		return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT0)

	return "0 %s" % (MONETARY_UNIT0)

def NumberToSecondaryCoinString(n) :
	if n <= 0 :
		return "0 %s" % (MONETARY_UNIT_JUN)

	return "%s %s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]), MONETARY_UNIT_JUN)


def NumberToMoneyStringNEW(n) :
	if n <= 0 :
		return "0"
	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]))

def MoneyFormat(n):
	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ])) 

def SecondToHMS(time):
	if time <= 0:
		return "0 " + SECOND
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	text = ""
	if hour > 0:
		text += str(hour) + " " + HOUR
		text += " "
	if minute > 0:
		text += str(minute) + " " + MINUTE
		text += " "
	if second > 0:
		text += str(second) + " " + SECOND
	return text

def NumberToEXPString(n) :
	if n <= 0 :
		return "0"

	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]))

def NumberWithPoint(n) :
	if n <= 0 :
		return "0"

	return "%s" % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]))
	
def NumberToMoneyString2(n) :
	if long(n) <= 0 :
		return "0"

	return "%s " % ('.'.join([ i-3<0 and str(n)[:i] or str(n)[i-3:i] for i in range(len(str(n))%3, len(str(n))+1, 3) if i ]))

if app.ENABLE_SUNG_MAHI_TOWER:
	import constInfo
	def LoadSungMahiInfo():
		if len(constInfo.sungMahiRewardInfo) > 0 or len(constInfo.sungMahiElementInfo) > 0:
			return
		
		loadFiles = ["sungmahee_tower_reward", "sungmahee_tower_element"]
		fileIndex = 1
		
		for file in loadFiles:
			lineIndex = 1
			
			try:
				lines = open("locale/common/sungmahee_tower/standard/{}.txt".format(file), "r").readlines()
			except IOError:
				import dbg
				dbg.LogBox("LoadSungMahiInfo read error file({})".format(file))
				app.Abort()
			
			import dbg
			for line in lines:
				try:
					if not line:
						lineIndex += 1
						continue
					
					#skip first line
					if lineIndex == 1:
						lineIndex += 1
						continue
					
					tokens = line[:-1].split("\t")
					if fileIndex == 1:
						tempListInfo = []
						tempTokenInfo = []
						
						#set level info
						tempTokenInfo.append(tokens[0])
						for tokenInfo in xrange(len(tokens)):
							if (tokenInfo < 1):
								continue
							
							if len(tempListInfo) < 2:
								tempListInfo.append(tokens[tokenInfo])
								
							if len(tempListInfo) > 1:
								tempTokenInfo.append(tempListInfo)
								tempListInfo= []
						
						constInfo.sungMahiRewardInfo.append(tempTokenInfo)
					else:
						constInfo.sungMahiElementInfo.append(tokens)
					
					lineIndex += 1
				except:
					raise
					
			fileIndex += 1
	
	LoadSungMahiInfo()
	
	def SecondToM(time):
		if time < 60:
			return "1" + MINUTE

		minute = int((time / 60) % 60)
		text = ""

		if minute > 0:
			text += str(minute) + MINUTE

		return text
		
	def SecondToMS(time):
		if time < 60:
			return "{}".format(time) + SECOND
		
		second = int(time % 60)
		minute = int((time / 60) % 60)
		text = ""

		if minute > 0:
			text += str(minute) + MINUTE
			text += " "
		
		if minute > 0:
			text += "{}".format(second) + SECOND
			
		return text

def GetLetterImageName():
	return "icon/item/scroll_close.tga"
def GetLetterOpenImageName():
	return "icon/item/scroll_open.tga"
def GetLetterCloseImageName():
	return "icon/item/scroll_close.tga"

def GetBlueLetterImageName():
	return "icon/item/scroll_close_blue.tga"
def GetBlueLetterOpenImageName():
	return "icon/item/scroll_open_blue.tga"
def GetBlueLetterCloseImageName():
	return "icon/item/scroll_close_blue.tga"
	