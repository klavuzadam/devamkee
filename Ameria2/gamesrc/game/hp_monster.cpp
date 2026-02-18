#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "locale_service.h"


struct hp_monster_values
{
	DWORD vnum_mob;
	int map_index;
	
};

std::vector<hp_monster_values> load_hp_monster;

void LoadInfoHpMonster()
{
    FILE* fp;
    char file_name[256 + 1];
    char one_line[256];
    const char* delim = " \t\r\n";
    char* v, * token_string;
    DWORD vnum_mob = 0;
    int map_index = 0;

    hp_monster_values monster_values = { 0, 0 };

    snprintf(file_name, sizeof(file_name), "%s/monster_hp.txt", LocaleService_GetBasePath().c_str());

    fp = fopen(file_name, "r");
    if (!fp)
    {
        sys_err("Error: Could not open file %s for reading.", file_name);
        return;
    }

    while (fgets(one_line, sizeof(one_line), fp))
    {
        vnum_mob = 0;
        map_index = 0;

        if (one_line[0] == '#')
            continue;

        token_string = strtok(one_line, delim);
        if (NULL == token_string)
            continue;

        if ((v = strtok(NULL, delim)))
        {
            if (!str_to_number(vnum_mob, v))
            {
                sys_err("Error: Invalid MOB_VNUM value in file %s.", file_name);
                continue;
            }
        }

        if ((v = strtok(NULL, delim)))
        {
            if (!str_to_number(map_index, v))
            {
                sys_err("Error: Invalid MAP_INDEX value in file %s.", file_name);
                continue;
            }
        }

        TOKEN("MOB_VNUM")
        {
            monster_values.vnum_mob = vnum_mob;
            monster_values.map_index = map_index;
            load_hp_monster.push_back(monster_values);
        }
    }

    fclose(fp);
}


bool CheckMonsterHp(DWORD vnum_mob)
{
	for (int i = 0; i < load_hp_monster.size(); ++i)
	{
		if(load_hp_monster[i].vnum_mob == vnum_mob)
		{
			return true;
		}
	}
	return false;
}


bool GetMapIndexMonster(DWORD vnum_mob, int map_index)
{
	for (int i = 0; i < load_hp_monster.size(); ++i)
	{
		if(load_hp_monster[i].vnum_mob == vnum_mob){
			if(map_index == load_hp_monster[i].map_index){
				return true;
			}
		}
	}
	return false;
}

int GetValueIndexMonster(DWORD vnum_mob)
{
	for (int i = 0; i < load_hp_monster.size(); ++i)
	{
		if(load_hp_monster[i].vnum_mob == vnum_mob){
			return load_hp_monster[i].map_index;
		}
	}
	return 0;
}