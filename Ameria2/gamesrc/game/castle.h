/*********************************************************************
 * date        : 2007.04.07
 * file        : castle.h
 * author      : mhh
 * description :
 */

#ifndef _castle_h_
#define _castle_h_


#define MAX_CASTLE_GUARD_REGION		4	// °æºñº´ ¹èÄ¡ ±¸¿ª
#define MAX_CASTLE_GUARD_PER_REGION	10	// ÇÑÁö¿ª¿¡ ¹èÄ¡ÇÒ¼öÀÖ´Â °æºñº´±×·ì
#define MAX_CASTLE_FROG				20	// È²±Ý µÎ²¨ºñ
#define MAX_CASTLE_TOWER			10	// ºÀÈ­ ÃÖ´ë ¼ÒÈ¯ °³¼ö
#define MIN_CASTLE_TOWER			5	// ºÀÈ­ ÃÖ¼Ò ¼ÒÈ¯ °³¼ö


#define CASTLE_FROG_PRICE		100000000	// È²±ÝµÎ²¨ºñ °¡°Ý (1¾ï)
#define CASTLE_FROG_VNUM		11505	// È²±ÝµÎ²¨ºñ ¹øÈ£
//#define CASTLE_TOWER_VNUM		11506	// ºÀÈ­ ¹øÈ£
#define IS_CASTLE_MAP(map)		(181==(map)||182==(map)||(183)==(map))
//#define IS_CASTLE_TOWER(vnum)	(11506==(vnum)||11507==(vnum)||11508==(vnum)||11509==(vnum) || 11510==(vnum))


enum CASTLE_STATE
{
	CASTLE_SIEGE_NONE,			// ÆòÈ­¸ðµå
	CASTLE_SIEGE_STRUGGLE,		// °ø¼ºÁß
	CASTLE_SIEGE_END			// ¼ö¼º¿¡ ¼º°øÇß´Ù¸é 1½Ã°£µ¿¾È ºÀÈ­¸¦ ±ý ¼ö ÀÖ´Ù.
};


struct CASTLE_DATA
{
	LPCHARACTER	frog[MAX_CASTLE_FROG];	// È²±ÝµÎ²¨ºñ

	LPCHARACTER	guard[MAX_CASTLE_GUARD_REGION][MAX_CASTLE_GUARD_PER_REGION];	// °æºñº´ ¸®´õ
	DWORD		guard_group[MAX_CASTLE_GUARD_REGION][MAX_CASTLE_GUARD_PER_REGION];	// °æºñº´ ¸®´õ

	LPCHARACTER	tower[MAX_CASTLE_TOWER];	// ºÀÈ­

	LPEVENT		siege_event;
	LPEVENT		stone_event;
};




/* extern functions */
bool		castle_boot();
void		castle_save();
int			castle_siege(int empire, int tower_count);
void		castle_start_siege(int empire, int tower_count);
void		castle_end_siege();

LPCHARACTER castle_spawn_frog(int empire);
LPCHARACTER	castle_spawn_guard(int empire, DWORD group_vnum, int region_index);
bool		castle_spawn_tower(int empire, int tower_count);

void		castle_guard_die(LPCHARACTER ch, LPCHARACTER killer);
void		castle_frog_die(LPCHARACTER ch, LPCHARACTER killer);
void		castle_tower_die(LPCHARACTER ch, LPCHARACTER killer);

int			castle_guard_count(int empire, int region_index);
int			castle_frog_count(int empire);

bool		castle_is_guard_vnum(DWORD vnum);
int			castle_cost_of_hiring_guard(DWORD vnum);
bool		castle_can_attack(LPCHARACTER ch, LPCHARACTER victim);

bool		castle_frog_to_empire_money(LPCHARACTER ch);
bool		castle_is_my_castle(int empire, int map_index);
bool		castle_is_tower_vnum(DWORD vnum);

#endif	/* _castle_h_ */

