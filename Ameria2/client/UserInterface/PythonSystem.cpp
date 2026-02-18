#include "StdAfx.h"
#include "PythonSystem.h"
#include "PythonApplication.h"

#define DEFAULT_VALUE_ALWAYS_SHOW_NAME		true

void CPythonSystem::SetInterfaceHandler(PyObject * poHandler)
{
// NOTE : 레퍼런스 카운트는 바꾸지 않는다. 레퍼런스가 남아 있어 Python에서 완전히 지워지지 않기 때문.
//        대신에 __del__때 Destroy를 호출해 Handler를 NULL로 셋팅한다. - [levites]
//	if (m_poInterfaceHandler)
//		Py_DECREF(m_poInterfaceHandler);

	m_poInterfaceHandler = poHandler;

//	if (m_poInterfaceHandler)
//		Py_INCREF(m_poInterfaceHandler);
}

void CPythonSystem::DestroyInterfaceHandler()
{
	m_poInterfaceHandler = NULL;
}

void CPythonSystem::SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight)
{
	m_WindowStatus[iIndex].isVisible	= iVisible;
	m_WindowStatus[iIndex].isMinimized	= iMinimized;
	m_WindowStatus[iIndex].ixPosition	= ix;
	m_WindowStatus[iIndex].iyPosition	= iy;
	m_WindowStatus[iIndex].iHeight		= iHeight;
}

void CPythonSystem::GetDisplaySettings()
{
	memset(m_ResolutionList, 0, sizeof(TResolution) * RESOLUTION_MAX_NUM);
	m_ResolutionCount = 0;

#ifdef USE_D3D9
    LPDIRECT3D9 lpD3D = CPythonGraphic::Instance().GetD3D();
    D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
#else
    LPDIRECT3D8 lpD3D = CPythonGraphic::Instance().GetD3D();
    D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
#endif

	D3DDISPLAYMODE d3ddmDesktop;

#ifdef USE_D3D9
    lpD3D->GetAdapterIdentifier(0, D3DENUM_WHQL_LEVEL, &d3dAdapterIdentifier);
//    lpD3D->GetAdapterIdentifier(0, 0, &d3dAdapterIdentifier);
#else
    lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &d3dAdapterIdentifier);
#endif

	lpD3D->GetAdapterDisplayMode(0, &d3ddmDesktop);

	// 이 어뎁터가 가지고 있는 디스플래이 모드갯수를 나열한다..
#ifdef USE_D3D9
    DWORD dwNumAdapterModes = lpD3D->GetAdapterModeCount(0, d3ddmDesktop.Format);
#else
    DWORD dwNumAdapterModes = lpD3D->GetAdapterModeCount(0);
#endif

	for (UINT iMode = 0; iMode < dwNumAdapterModes; iMode++)
	{
		D3DDISPLAYMODE DisplayMode;
#ifdef USE_D3D9
        lpD3D->EnumAdapterModes(0, d3ddmDesktop.Format, iMode, &DisplayMode);
#else
        lpD3D->EnumAdapterModes(0, iMode, &DisplayMode);
#endif
		DWORD bpp = 0;

		// 800 600 이상만 걸러낸다.
		if (DisplayMode.Width < 800 || DisplayMode.Height < 600)
			continue;

		// 일단 16bbp 와 32bbp만 취급하자.
		// 16bbp만 처리하게끔 했음 - [levites]
		if (DisplayMode.Format == D3DFMT_R5G6B5)
			bpp = 16;
		else if (DisplayMode.Format == D3DFMT_X8R8G8B8)
			bpp = 32;
		else
			continue;

		int check_res = false;

		for (int i = 0; !check_res && i < m_ResolutionCount; ++i)
		{
			if (m_ResolutionList[i].bpp != bpp ||
				m_ResolutionList[i].width != DisplayMode.Width ||
				m_ResolutionList[i].height != DisplayMode.Height)
				continue;

			int check_fre = false;

			// 프리퀀시만 다르므로 프리퀀시만 셋팅해준다.
			for (int j = 0; j < m_ResolutionList[i].frequency_count; ++j)
			{
				if (m_ResolutionList[i].frequency[j] == DisplayMode.RefreshRate)
				{
					check_fre = true;
					break;
				}
			}

			if (!check_fre)
				if (m_ResolutionList[i].frequency_count < FREQUENCY_MAX_NUM)
					m_ResolutionList[i].frequency[m_ResolutionList[i].frequency_count++] = DisplayMode.RefreshRate;

			check_res = true;
		}

		if (!check_res)
		{
			// 새로운 거니까 추가해주자.
			if (m_ResolutionCount < RESOLUTION_MAX_NUM)
			{
				m_ResolutionList[m_ResolutionCount].width			= DisplayMode.Width;
				m_ResolutionList[m_ResolutionCount].height			= DisplayMode.Height;
				m_ResolutionList[m_ResolutionCount].bpp				= bpp;
				m_ResolutionList[m_ResolutionCount].frequency[0]	= DisplayMode.RefreshRate;
				m_ResolutionList[m_ResolutionCount].frequency_count	= 1;

				++m_ResolutionCount;
			}
		}
	}
}

int	CPythonSystem::GetResolutionCount()
{
	return m_ResolutionCount;
}

int CPythonSystem::GetFrequencyCount(int index)
{
	if (index >= m_ResolutionCount)
		return 0;

    return m_ResolutionList[index].frequency_count;
}

bool CPythonSystem::GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp)
{
	if (index >= m_ResolutionCount)
		return false;

	*width = m_ResolutionList[index].width;
	*height = m_ResolutionList[index].height;
	*bpp = m_ResolutionList[index].bpp;
	return true;
}

bool CPythonSystem::GetFrequency(int index, int freq_index, OUT DWORD *frequncy)
{
	if (index >= m_ResolutionCount)
		return false;

	if (freq_index >= m_ResolutionList[index].frequency_count)
		return false;

	*frequncy = m_ResolutionList[index].frequency[freq_index];
	return true;
}

int	CPythonSystem::GetResolutionIndex(DWORD width, DWORD height, DWORD bit)
{
	DWORD re_width, re_height, re_bit;
	int i = 0;

	while (GetResolution(i, &re_width, &re_height, &re_bit))
	{
		if (re_width == width)
			if (re_height == height)
				if (re_bit == bit)
					return i;
		i++;
	}

	return 0;
}

int	CPythonSystem::GetFrequencyIndex(int res_index, DWORD frequency)
{
	DWORD re_frequency;
	int i = 0;

	while (GetFrequency(res_index, i, &re_frequency))
	{
		if (re_frequency == frequency)
			return i;

		i++;
	}

	return 0;
}

DWORD CPythonSystem::GetWidth()
{
	return m_Config.width;
}

DWORD CPythonSystem::GetHeight()
{
	return m_Config.height;
}
DWORD CPythonSystem::GetBPP()
{
	return m_Config.bpp;
}
DWORD CPythonSystem::GetFrequency()
{
	return m_Config.frequency;
}

bool CPythonSystem::IsNoSoundCard()
{
	return m_Config.bNoSoundCard;
}

bool CPythonSystem::IsSoftwareCursor()
{
	return m_Config.is_software_cursor;
}

float CPythonSystem::GetMusicVolume()
{
	return m_Config.music_volume;
}

int CPythonSystem::GetSoundVolume()
{
	return m_Config.voice_volume;
}

void CPythonSystem::SetMusicVolume(float fVolume)
{
	m_Config.music_volume = fVolume;
}

void CPythonSystem::SetSoundVolumef(float fVolume)
{
	m_Config.voice_volume = int(5 * fVolume);
}

int CPythonSystem::GetDistance()
{
	return m_Config.iDistance;
}

int CPythonSystem::IsSaveID()
{
	return m_Config.isSaveID;
}

const char * CPythonSystem::GetSaveID()
{
	return m_Config.SaveID;
}

bool CPythonSystem::isViewCulling()
{
	return m_Config.is_object_culling;
}

void CPythonSystem::SetSaveID(int iValue, const char * c_szSaveID)
{
	if (iValue != 1)
		return;

	m_Config.isSaveID = iValue;
	strncpy(m_Config.SaveID, c_szSaveID, sizeof(m_Config.SaveID) - 1);
}

CPythonSystem::TConfig * CPythonSystem::GetConfig()
{
	return &m_Config;
}

void CPythonSystem::SetConfig(TConfig * pNewConfig)
{
	m_Config = *pNewConfig;
}

void CPythonSystem::SetDefaultConfig()
{
	m_Config = {};

	m_Config.width = 1024;
	m_Config.height = 768;
	m_Config.bpp = 32;

#ifdef LOCALE_SERVICE_WE_JAPAN
	m_Config.bWindowed = true;
#else
	m_Config.bWindowed = false;
#endif

	m_Config.is_software_cursor = false;
	m_Config.is_object_culling = true;
	m_Config.iDistance = 3;

	m_Config.gamma = 3;
	m_Config.music_volume = 1.0f;
	m_Config.voice_volume = 5;

	m_Config.bDecompressDDS = 0;
	m_Config.bSoftwareTiling = 0;
	m_Config.bViewChat = true;
	m_Config.bAlwaysShowName = DEFAULT_VALUE_ALWAYS_SHOW_NAME;
	m_Config.bShowDamage = true;
	m_Config.bShowSalesText = true;
	m_Config.bShowOutline = false;
	m_Config.bFogEnvironment = true;
	m_Config.fFogDistance = 8192.0f;
	m_Config.bNightEnvironment = false;
	m_Config.bSnowingEnvironment = false;
	m_Config.bSnowTerrainTexture = false;
	m_Config.bShadowLevel = 3;

	m_Config.iFOVLevel = 30.0f;

	m_Config.fScaleDamage = 0.5f;

	m_Config.bInventoryChange = 0;

#ifdef WJ_SHOW_MOB_INFO
#ifdef ENABLE_SHOW_MOBAIFLAG
	m_Config.bShowMobAIFlag = true;
#endif
#ifdef ENABLE_SHOW_MOBLEVEL
	m_Config.bShowMobLevel = true;
#endif
#endif

#if defined(ENABLE_IKASHOP_RENEWAL) && defined(EXTEND_IKASHOP_PRO)
	m_Config.shopRangeView = 5000.0f;
#endif

#ifdef ENABLE_NEW_GAMEOPTION
	m_Config.questLetter = true;
	m_Config.countryFlag = true;
	m_Config.affectIcons = true;
#endif

#ifdef ENABLE_GRAPHIC_ON_OFF
	m_Config.iEffectLevel = 0;
	m_Config.bPetStatus = true;
	m_Config.bMountStatus = true;
	m_Config.bNpcStatus = true;
#endif

#ifdef ENABLE_DISCORD_STUFF
	m_Config.bMessage = true;
#endif

#ifdef ENABLE_TRACK_WINDOW
	m_Config.bDungeonTrack = false;
	m_Config.bBossTrack = false;
#endif

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
	m_Config.fobjectDist = 25600.0;
#endif

#ifdef STONE_SCALE
	m_Config.fStoneScale = 1.0f;
#endif
}

bool CPythonSystem::IsWindowed()
{
	return m_Config.bWindowed;
}

bool CPythonSystem::IsViewChat()
{
	return m_Config.bViewChat;
}

void CPythonSystem::SetViewChatFlag(int iFlag)
{
	m_Config.bViewChat = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsAlwaysShowName()
{
	return m_Config.bAlwaysShowName;
}

void CPythonSystem::SetAlwaysShowNameFlag(int iFlag)
{
	m_Config.bAlwaysShowName = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowDamage()
{
	return m_Config.bShowDamage;
}

void CPythonSystem::SetShowDamageFlag(int iFlag)
{
	m_Config.bShowDamage = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowSalesText()
{
	return m_Config.bShowSalesText;
}

void CPythonSystem::SetShowSalesTextFlag(int iFlag)
{
	m_Config.bShowSalesText = iFlag == 1 ? true : false;
}

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
bool CPythonSystem::IsShowMobAIFlag()
{
	return m_Config.bShowMobAIFlag;
}

void CPythonSystem::SetShowMobAIFlagFlag(int iFlag)
{
	m_Config.bShowMobAIFlag = iFlag == 1 ? true : false;
}
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
bool CPythonSystem::IsShowMobLevel()
{
	return m_Config.bShowMobLevel;
}

void CPythonSystem::SetShowMobLevelFlag(int iFlag)
{
	m_Config.bShowMobLevel = iFlag == 1 ? true : false;
}
#endif

bool CPythonSystem::IsAutoTiling()
{
	if (m_Config.bSoftwareTiling == 0)
		return true;

	return false;
}

void CPythonSystem::SetSoftwareTiling(bool isEnable)
{
	if (isEnable)
		m_Config.bSoftwareTiling=1;
	else
		m_Config.bSoftwareTiling=2;
}

bool CPythonSystem::IsSoftwareTiling()
{
	if (m_Config.bSoftwareTiling==1)
		return true;

	return false;
}

bool CPythonSystem::IsUseDefaultIME()
{
	return m_Config.bUseDefaultIME;
}

bool CPythonSystem::LoadConfig()
{
	FILE* fp = nullptr;
	if (!(fp = fopen("metin2.cfg", "rt")))
	{
		return false;
	}

	char buf[256];
	char command[256];
	char value[256];

	while (fgets(buf, 256, fp))
	{
		if (sscanf(buf, " %s %s\n", command, value) == EOF)
		{
			break;
		}

		if (!stricmp(command, "WIDTH"))
		{
			m_Config.width = atoi(value);
		}
		else if (!stricmp(command, "HEIGHT"))
		{
			m_Config.height = atoi(value);
		}
		else if (!stricmp(command, "BPP"))
		{
			m_Config.bpp = atoi(value);
		}
		else if (!stricmp(command, "FREQUENCY"))
		{
			m_Config.frequency = atoi(value);
		}
		else if (!stricmp(command, "SOFTWARE_CURSOR"))
		{
			m_Config.is_software_cursor = atoi(value) ? true : false;
		}
		else if (!stricmp(command, "OBJECT_CULLING"))
		{
			m_Config.is_object_culling = atoi(value) ? true : false;
		}
		else if (!stricmp(command, "VISIBILITY"))
		{
			m_Config.iDistance = atoi(value);
		}
		else if (!stricmp(command, "MUSIC_VOLUME"))
		{
			if(strchr(value, '.') == 0)
			{ // Old compatiability
				m_Config.music_volume = pow(10.0f, (-1.0f + (((float) atoi(value)) / 5.0f)));

				if(atoi(value) == 0)
				{
					m_Config.music_volume = 0.0f;
				}
			}
			else
			{
				m_Config.music_volume = atof(value);
			}
		}
		else if (!stricmp(command, "VOICE_VOLUME"))
		{
			m_Config.voice_volume = (char) atoi(value);
		}
		else if (!stricmp(command, "GAMMA"))
		{
			m_Config.gamma = atoi(value);
		}
		else if (!stricmp(command, "IS_SAVE_ID"))
		{
			m_Config.isSaveID = atoi(value);
		}
		else if (!stricmp(command, "SAVE_ID"))
		{
			strncpy(m_Config.SaveID, value, 20);
		}
		else if (!stricmp(command, "PRE_LOADING_DELAY_TIME"))
		{
			g_iLoadingDelayTime = atoi(value);
		}
		else if (!stricmp(command, "WINDOWED"))
		{
			m_Config.bWindowed = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "USE_DEFAULT_IME"))
		{
			m_Config.bUseDefaultIME = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "SOFTWARE_TILING"))
		{
			m_Config.bSoftwareTiling = atoi(value);
		}
		else if (!stricmp(command, "DECOMPRESSED_TEXTURE"))
		{
			m_Config.bDecompressDDS = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "NO_SOUND_CARD"))
		{
			m_Config.bNoSoundCard = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "VIEW_CHAT"))
		{
			m_Config.bViewChat = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "ALWAYS_VIEW_NAME"))
		{
			m_Config.bAlwaysShowName = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "SHOW_DAMAGE"))
		{
			m_Config.bShowDamage = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "SHOW_SALESTEXT"))
		{
			m_Config.bShowSalesText = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "FOG_ENVIRONMENT"))
		{
			m_Config.bFogEnvironment = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "FOG_DISTANCE"))
		{
			m_Config.fFogDistance = atof(value);
		}
		else if (!stricmp(command, "NIGHT_ENVIRONMENT"))
		{
			m_Config.bNightEnvironment = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "SNOWING_ENVIRONMENT"))
		{
			m_Config.bSnowingEnvironment = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "SNOW_TERRAIN_TEXTURE"))
		{
			m_Config.bSnowTerrainTexture = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "SHADOW_LEVEL"))
		{
			m_Config.bShadowLevel = atoi(value);
		}
		else if (!stricmp(command, "SHOW_OUTLINE"))
		{
			m_Config.bShowOutline = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "perspective"))
		{
			m_Config.iFOVLevel = atof(value);
		}
		else if (!stricmp(command, "DAMAGE_SCALE"))
		{
			m_Config.fScaleDamage = atof(value);
		}
		else if (!stricmp(command, "INVENTORY_LEVEL"))
		{
			m_Config.bInventoryChange = atoi(value);
		}
#ifdef WJ_SHOW_MOB_INFO
#ifdef ENABLE_SHOW_MOBAIFLAG
		else if (!stricmp(command, "SHOW_MOBAIFLAG"))
		{
			m_Config.bShowMobAIFlag = atoi(value) == 1 ? true : false;
		}
#endif
#ifdef ENABLE_SHOW_MOBLEVEL
		else if (!stricmp(command, "SHOW_MOBLEVEL"))
		{
			m_Config.bShowMobLevel = atoi(value) == 1 ? true : false;
		}
#endif
#endif
#if defined(ENABLE_IKASHOP_RENEWAL) && defined(EXTEND_IKASHOP_PRO)
		else if (!stricmp(command, "SHOP_RANGE_VIEW"))
		{
			m_Config.shopRangeView = std::stof(value);
		}
#endif
#ifdef ENABLE_NEW_GAMEOPTION
		else if (!stricmp(command, "QUEST_LETTER"))
		{
			m_Config.questLetter = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "COUNTRY_FLAG"))
		{
			m_Config.countryFlag = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "AFFECT_ICONS"))
		{
			m_Config.affectIcons = atoi(value) == 1 ? true : false;
		}
#endif
#ifdef ENABLE_GRAPHIC_ON_OFF
		else if (!stricmp(command, "effect_level"))
		{
			m_Config.iEffectLevel = atoi(value);
		}
		else if (!stricmp(command, "pet_status"))
		{
			m_Config.bPetStatus = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "mount_status"))
		{
			m_Config.bMountStatus = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "npc_status"))
		{
			m_Config.bNpcStatus = atoi(value) == 1 ? true : false;
		}
#endif
#ifdef ENABLE_DISCORD_STUFF
		else if (!stricmp(command, "special_message"))
		{
			m_Config.bMessage = atoi(value) == 1 ? true : false;
		}
#endif
#ifdef ENABLE_TRACK_WINDOW
		else if (!stricmp(command, "DUNGEON_TRACK"))
		{
			m_Config.bDungeonTrack = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "BOSS_TRACK"))
		{
			m_Config.bBossTrack = atoi(value) == 1 ? true : false;
		}
#endif
#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
		else if (!stricmp(command, "OBJECT_DIST"))
		{
			m_Config.fobjectDist = atof(value) > 25600.0 ? 25600.0 : atof(value);
		}
#endif
#ifdef STONE_SCALE
		else if (!stricmp(command, "STONE_SCALE"))
		{
			if (strchr(value, '.') == 0)
			{
				m_Config.fStoneScale = pow(10.0f, (-1.0f + (((float)atoi(value)) / 5.0f)));

				if (atoi(value) == 0)
				{
					m_Config.fStoneScale = 0.0f;
				}
			}
			else
			{
				m_Config.fStoneScale = atof(value);
			}
		}
#endif
	}

	if (m_Config.bWindowed)
	{
		unsigned screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		unsigned screen_height = GetSystemMetrics(SM_CYFULLSCREEN);

		if (m_Config.width >= screen_width)
		{
			m_Config.width = screen_width;
		}

		if (m_Config.height >= screen_height)
		{
			m_Config.height = screen_height;
		}
	}

	m_OldConfig = m_Config;

	fclose(fp);

//	Tracef("LoadConfig: Resolution: %dx%d %dBPP %dHZ Software Cursor: %d, Music/Voice Volume: %d/%d Gamma: %d\n",
//		m_Config.width,
//		m_Config.height,
//		m_Config.bpp,
//		m_Config.frequency,
//		m_Config.is_software_cursor,
//		m_Config.music_volume,
//		m_Config.voice_volume,
//		m_Config.gamma);

	return true;
}

bool CPythonSystem::SaveConfig()
{
	FILE* fp = nullptr;

	if (!(fp = fopen("metin2.cfg", "wt")))
	{
		return false;
	}

	fprintf(fp, "WIDTH\t\t\t\t\t%d\n"
				"HEIGHT\t\t\t\t\t%d\n"
				"BPP\t\t\t\t\t%d\n"
				"FREQUENCY\t\t\t\t\t%d\n"
				"SOFTWARE_CURSOR\t\t\t\t\t%d\n"
				"OBJECT_CULLING\t\t\t\t\t%d\n"
				"VISIBILITY\t\t\t\t\t%d\n"
				"MUSIC_VOLUME\t\t\t\t\t%.3f\n"
				"VOICE_VOLUME\t\t\t\t\t%d\n"
				"GAMMA\t\t\t\t\t%d\n"
				"IS_SAVE_ID\t\t\t\t\t%d\n"
				"SAVE_ID\t\t\t\t\t%s\n"
				"PRE_LOADING_DELAY_TIME\t\t\t\t\t%d\n"
				"DECOMPRESSED_TEXTURE\t\t\t\t\t%d\n",
				m_Config.width,
				m_Config.height,
				m_Config.bpp,
				m_Config.frequency,
				m_Config.is_software_cursor,
				m_Config.is_object_culling,
				m_Config.iDistance,
				m_Config.music_volume,
				m_Config.voice_volume,
				m_Config.gamma,
				m_Config.isSaveID,
				m_Config.SaveID,
				g_iLoadingDelayTime,
				m_Config.bDecompressDDS);

	if (m_Config.bWindowed == 1)
	{
		fprintf(fp, "WINDOWED\t\t\t\t\t%d\n", m_Config.bWindowed);
	}

	if (m_Config.bViewChat == 0)
	{
		fprintf(fp, "VIEW_CHAT\t\t\t\t\t%d\n", m_Config.bViewChat);
	}

	if (m_Config.bAlwaysShowName != DEFAULT_VALUE_ALWAYS_SHOW_NAME)
	{
		fprintf(fp, "ALWAYS_VIEW_NAME\t\t\t\t\t%d\n", m_Config.bAlwaysShowName);
	}

	if (m_Config.bShowDamage == 0)
	{
		fprintf(fp, "SHOW_DAMAGE\t\t\t\t\t%d\n", m_Config.bShowDamage);
	}

	if (m_Config.bShowSalesText == 0)
	{
		fprintf(fp, "SHOW_SALESTEXT\t\t\t\t\t%d\n", m_Config.bShowSalesText);
	}

	if (!m_Config.bFogEnvironment)
	{
		fprintf(fp, "FOG_ENVIRONMENT\t\t\t\t\t%d\n", m_Config.bFogEnvironment);
	}

	fprintf(fp, "FOG_DISTANCE\t\t\t\t\t%f\n", m_Config.fFogDistance);

	if (m_Config.bNightEnvironment)
	{
		fprintf(fp, "NIGHT_ENVIRONMENT\t\t\t\t\t%d\n", m_Config.bNightEnvironment);
	}

	if (m_Config.bSnowingEnvironment)
	{
		fprintf(fp, "SNOWING_ENVIRONMENT\t\t\t\t\t%d\n", m_Config.bSnowingEnvironment);
	}

	if (m_Config.bSnowTerrainTexture)
	{
		fprintf(fp, "SNOW_TERRAIN_TEXTURE\t\t\t\t\t%d\n", m_Config.bSnowTerrainTexture);
	}

	fprintf(fp, "SHADOW_LEVEL\t\t\t\t\t%d\n", m_Config.bShadowLevel);

	fprintf(fp, "SHOW_OUTLINE\t\t\t\t\t%d\n", m_Config.bShowOutline);
	fprintf(fp, "USE_DEFAULT_IME\t\t\t\t\t%d\n", m_Config.bUseDefaultIME);
	fprintf(fp, "SOFTWARE_TILING\t\t\t\t\t%d\n", m_Config.bSoftwareTiling);

	fprintf(fp, "perspective\t\t\t\t\t%.1f\n", m_Config.iFOVLevel);

	fprintf(fp, "DAMAGE_SCALE\t\t\t\t\t%.3f\n", m_Config.fScaleDamage);

	fprintf(fp, "INVENTORY_LEVEL\t\t\t\t\t%d\n", m_Config.bInventoryChange);

#ifdef WJ_SHOW_MOB_INFO
#ifdef ENABLE_SHOW_MOBAIFLAG
	if (m_Config.bShowMobAIFlag == 0)
	{
		fprintf(fp, "SHOW_MOBAIFLAG\t\t\t\t\t%d\n", m_Config.bShowMobAIFlag);
	}
#endif

#ifdef ENABLE_SHOW_MOBLEVEL
	if (m_Config.bShowMobLevel == 0)
	{
		fprintf(fp, "SHOW_MOBLEVEL\t\t\t\t\t%d\n", m_Config.bShowMobLevel);
	}
#endif
#endif

#if defined(ENABLE_IKASHOP_RENEWAL) && defined(EXTEND_IKASHOP_PRO)
	fprintf(fp, "SHOP_RANGE_VIEW\t\t\t\t\t%f\n", m_Config.shopRangeView);
#endif

#ifdef ENABLE_NEW_GAMEOPTION
	fprintf(fp, "QUEST_LETTER\t\t\t\t\t%d\n", m_Config.questLetter);
	fprintf(fp, "COUNTRY_FLAG\t\t\t\t\t%d\n", m_Config.countryFlag);
	fprintf(fp, "AFFECT_ICONS\t\t\t\t\t%d\n", m_Config.affectIcons);
#endif

#ifdef ENABLE_GRAPHIC_ON_OFF
	fprintf(fp, "effect_level\t\t\t\t\t%d\n", m_Config.iEffectLevel);
	fprintf(fp, "pet_status\t\t\t\t\t%d\n", m_Config.bPetStatus);
	fprintf(fp, "mount_status\t\t\t\t\t%d\n", m_Config.bMountStatus);
	fprintf(fp, "npc_status\t\t\t\t\t%d\n", m_Config.bNpcStatus);
#endif

#ifdef ENABLE_DISCORD_STUFF
	fprintf(fp, "special_message\t\t\t\t\t%d\n", m_Config.bMessage);
#endif

#ifdef ENABLE_TRACK_WINDOW
	fprintf(fp, "DUNGEON_TRACK\t\t\t\t\t%d\n", m_Config.bDungeonTrack);
	fprintf(fp, "BOSS_TRACK\t\t\t\t\t%d\n", m_Config.bBossTrack);
#endif

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
	fprintf(fp, "OBJECT_DIST\t\t\t\t\t%.5f\n", m_Config.fobjectDist);
#endif

#ifdef STONE_SCALE
	fprintf(fp, "STONE_SCALE\t\t\t\t\t%.3f\n", m_Config.fStoneScale);
#endif

	fprintf(fp, "\n");

	fclose(fp);

	return true;
}

bool CPythonSystem::LoadInterfaceStatus()
{
	FILE * File;
	File = fopen("interface.cfg", "rb");

	if (!File)
		return false;

	fread(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
	return true;
}

void CPythonSystem::SaveInterfaceStatus()
{
	if (!m_poInterfaceHandler)
		return;

	PyCallClassMemberFunc(m_poInterfaceHandler, "OnSaveInterfaceStatus", Py_BuildValue("()"));

	FILE * File;

	File = fopen("interface.cfg", "wb");

	if (!File)
	{
		TraceError("Cannot open interface.cfg");
		return;
	}

	fwrite(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
}

bool CPythonSystem::isInterfaceConfig()
{
	return m_isInterfaceConfig;
}

const CPythonSystem::TWindowStatus & CPythonSystem::GetWindowStatusReference(int iIndex)
{
	return m_WindowStatus[iIndex];
}

void CPythonSystem::ApplyConfig() // 이전 설정과 현재 설정을 비교해서 바뀐 설정을 적용 한다.
{
	if (m_OldConfig.gamma != m_Config.gamma)
	{
		float val = 1.0f;

		switch (m_Config.gamma)
		{
			case 0: val = 0.4f;	break;
			case 1: val = 0.7f; break;
			case 2: val = 1.0f; break;
			case 3: val = 1.2f; break;
			case 4: val = 1.4f; break;
		}

		CPythonGraphic::Instance().SetGamma(val);
	}

	if (m_OldConfig.is_software_cursor != m_Config.is_software_cursor)
	{
		if (m_Config.is_software_cursor)
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_SOFTWARE);
		else
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_HARDWARE);
	}

	m_OldConfig = m_Config;

	ChangeSystem();
}

void CPythonSystem::ChangeSystem()
{
	// Shadow
	/*
	if (m_Config.is_shadow)
		CScreen::SetShadowFlag(true);
	else
		CScreen::SetShadowFlag(false);
	*/
	CSoundManager& rkSndMgr = CSoundManager::Instance();
	/*
	float fMusicVolume;
	if (0 == m_Config.music_volume)
		fMusicVolume = 0.0f;
	else
		fMusicVolume= (float)pow(10.0f, (-1.0f + (float)m_Config.music_volume / 5.0f));
		*/
	rkSndMgr.SetMusicVolume(m_Config.music_volume);

	/*
	float fVoiceVolume;
	if (0 == m_Config.voice_volume)
		fVoiceVolume = 0.0f;
	else
		fVoiceVolume = (float)pow(10.0f, (-1.0f + (float)m_Config.voice_volume / 5.0f));
	*/
	rkSndMgr.SetSoundVolumeGrade(m_Config.voice_volume);
}

void CPythonSystem::SetFogEnvironment(const bool bFlag)
{
	m_Config.bFogEnvironment = bFlag;

	CPythonBackground::Instance().SetFogEnvironment(m_Config.bFogEnvironment);
}

bool CPythonSystem::IsFogEnvironment() const
{
	return m_Config.bFogEnvironment;
}

void CPythonSystem::SetFogDistance(const float fDist)
{
	m_Config.fFogDistance = fDist;
}

float CPythonSystem::GetFogDistance() const
{
	return m_Config.fFogDistance;
}

void CPythonSystem::SetNightEnvironment(const bool bFlag)
{
	m_Config.bNightEnvironment = bFlag;
}

bool CPythonSystem::IsNightEnvironment() const
{
	return m_Config.bNightEnvironment;
}

void CPythonSystem::SetSnowingEnvironment(const bool bFlag)
{
	m_Config.bSnowingEnvironment = bFlag;
}

bool CPythonSystem::IsSnowingEnvironment() const
{
	return m_Config.bSnowingEnvironment;
}

void CPythonSystem::SetSnowTerrainTexture(const bool bFlag)
{
	m_Config.bSnowTerrainTexture = bFlag;

	CPythonBackground::Instance().SetSnowTerrainTexture(m_Config.bSnowTerrainTexture);
}

bool CPythonSystem::IsSnowTerrainTexture() const
{
	return m_Config.bSnowTerrainTexture;
}

BYTE CPythonSystem::GetShadowLevel() const
{
	return m_Config.bShadowLevel;
}

bool CPythonSystem::SetShadowLevel(BYTE bLevel)
{
	if (bLevel > 5 || bLevel == m_Config.bShadowLevel)
	{
		return false;
	}

	m_Config.bShadowLevel = MIN(bLevel, 5);
	CPythonBackground::instance().RefreshShadowLevel();
	return true;
}

void CPythonSystem::Clear()
{
	SetInterfaceHandler(NULL);
}

CPythonSystem::CPythonSystem()
{
	memset(&m_Config, 0, sizeof(TConfig));

	m_poInterfaceHandler = NULL;

	SetDefaultConfig();

	LoadConfig();

	ChangeSystem();

	if (LoadInterfaceStatus())
		m_isInterfaceConfig = true;
	else
		m_isInterfaceConfig = false;
}

CPythonSystem::~CPythonSystem()
{
	assert(m_poInterfaceHandler==NULL && "CPythonSystem MUST CLEAR!");
}

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
void CPythonSystem::SetObjectDistance(float fDist)
{
	if (fDist > 25600.0)
		fDist = 25600.0;
	m_Config.fobjectDist = fDist;
}
#endif

#ifdef STONE_SCALE
float CPythonSystem::GetStoneScale()
{
	return m_Config.fStoneScale;
}

void CPythonSystem::SetStoneScale(float fScaleVal)
{
	m_Config.fStoneScale = fScaleVal;
}
#endif
