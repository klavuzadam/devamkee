#ifndef __INC_METIN2_GAME_LOCALE_H__
#define __INC_METIN2_GAME_LOCALE_H__

#include "../common/CommonDefines.h"
#include <string>
#include <fstream>

namespace multilang
{
	void load_item_names();
	const std::string& find_item_name(DWORD vnum, BYTE lang);
}

extern "C"
{
	void locale_init(const char *filename);

	const char *locale_find(const char *string);
	#define LC_TEXT(str) locale_find(str)

	extern int g_iUseLocale;

};

#endif
