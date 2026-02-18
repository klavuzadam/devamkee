#pragma once

class CPythonSystem : public CSingleton<CPythonSystem>
{
	public:
		enum EWindow
		{
			WINDOW_STATUS,
			WINDOW_INVENTORY,
			WINDOW_ABILITY,
			WINDOW_SOCIETY,
			WINDOW_JOURNAL,
			WINDOW_COMMAND,

			WINDOW_QUICK,
			WINDOW_GAUGE,
			WINDOW_MINIMAP,
			WINDOW_CHAT,

			WINDOW_MAX_NUM,
		};

		enum InventorySelectState
		{
			INVENTORY_NORMAL,
			INVENTORY_FIRST,
			INVENTORY_SECOND,
		};

		enum
		{
			FREQUENCY_MAX_NUM  = 30,
			RESOLUTION_MAX_NUM = 100
		};

		typedef struct SResolution
		{
			DWORD	width;
			DWORD	height;
			DWORD	bpp;		// bits per pixel (high-color = 16bpp, true-color = 32bpp)

			DWORD	frequency[20];
			BYTE	frequency_count;
		} TResolution;

		typedef struct SWindowStatus
		{
			int		isVisible;
			int		isMinimized;

			int		ixPosition;
			int		iyPosition;
			int		iHeight;
		} TWindowStatus;

		typedef struct SConfig
		{
			DWORD			width;
			DWORD			height;
			DWORD			bpp;
			DWORD			frequency;

			bool			is_software_cursor;
			bool			is_object_culling;
			int				iDistance;

			FLOAT			music_volume;
			BYTE			voice_volume;

			int				gamma;

			int				isSaveID;
			char			SaveID[20];

			bool			bWindowed;
			bool			bDecompressDDS;
			bool			bNoSoundCard;
			bool			bUseDefaultIME;
			BYTE			bSoftwareTiling;
			bool			bViewChat;
			bool			bAlwaysShowName;
			bool			bShowDamage;
			bool			bShowSalesText;
			bool			bShowOutline;
			bool			bFogEnvironment;
			float			fFogDistance;
			bool			bNightEnvironment;
			bool			bSnowingEnvironment;
			bool			bSnowTerrainTexture;
			BYTE			bShadowLevel;
			float			iFOVLevel;
			float			fScaleDamage;
			BYTE			bInventoryChange;

#ifdef WJ_SHOW_MOB_INFO
#ifdef ENABLE_SHOW_MOBAIFLAG
			bool			bShowMobAIFlag;
#endif
#ifdef ENABLE_SHOW_MOBLEVEL
			bool			bShowMobLevel;
#endif
#endif

#if defined(ENABLE_IKASHOP_RENEWAL) && defined(EXTEND_IKASHOP_PRO)
			float			shopRangeView;
#endif

#ifdef ENABLE_NEW_GAMEOPTION
			bool			questLetter;
			bool			countryFlag;
			bool			affectIcons;
#endif

#ifdef ENABLE_GRAPHIC_ON_OFF
			BYTE			iEffectLevel;
			bool			bPetStatus;
			bool			bMountStatus;
			bool			bNpcStatus;
#endif

#ifdef ENABLE_DISCORD_STUFF
			bool			bMessage;
#endif

#ifdef ENABLE_TRACK_WINDOW
			bool			bDungeonTrack;
			bool			bBossTrack;
#endif

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
			float			fobjectDist;
#endif

#ifdef STONE_SCALE
			float			fStoneScale;
#endif
		} TConfig;

	public:
		CPythonSystem();
		virtual ~CPythonSystem();

		void Clear();
		void SetInterfaceHandler(PyObject * poHandler);
		void DestroyInterfaceHandler();

#ifdef ENABLE_MAP_OBJECT_OPTIMIZATION
		float							GetObjectDistance() { return m_Config.fobjectDist; }
		void							SetObjectDistance(float fDist);
#endif

		// Config
		void							SetDefaultConfig();
		bool							LoadConfig();
		bool							SaveConfig();
		void							ApplyConfig();
		void							SetConfig(TConfig * set_config);
		TConfig *						GetConfig();
		void							ChangeSystem();

		float							GetScaleDamage() { return m_Config.fScaleDamage; }
		void							SetScaleDamage(float fScale) { m_Config.fScaleDamage = fScale; }

		// Interface
		bool							LoadInterfaceStatus();
		void							SaveInterfaceStatus();
		bool							isInterfaceConfig();
		const TWindowStatus &			GetWindowStatusReference(int iIndex);

		DWORD							GetWidth();
		DWORD							GetHeight();
		DWORD							GetBPP();

		DWORD							GetFrequency();
		bool							IsSoftwareCursor();
		bool							IsWindowed();
		bool							IsViewChat();
		bool							IsAlwaysShowName();
		bool							IsShowDamage();
		bool							IsShowSalesText();
		bool							IsUseDefaultIME();
		bool							IsNoSoundCard();
		bool							IsAutoTiling();
		bool							IsSoftwareTiling();
		void							SetSoftwareTiling(bool isEnable);
		void							SetViewChatFlag(int iFlag);
		void							SetAlwaysShowNameFlag(int iFlag);
		void							SetShowDamageFlag(int iFlag);
		void							SetShowSalesTextFlag(int iFlag);

		BYTE							GetInventoryChange() { return m_Config.bInventoryChange; }
		void							SetInventoryChange(BYTE iLevel) { m_Config.bInventoryChange = iLevel; }

		bool IsShowOutline() const
		{
			return m_Config.bShowOutline;
		}

#ifdef ENABLE_GRAPHIC_ON_OFF
		BYTE							GetEffectLevel() { return m_Config.iEffectLevel; }
		void							SetEffectLevel(BYTE iLevel) { m_Config.iEffectLevel = iLevel; }

		bool							GetPetRender() { return m_Config.bPetStatus; }
		void							SetPetRender(bool bStatus) { m_Config.bPetStatus = bStatus; }

		bool							GetMountRender() { return m_Config.bMountStatus; }
		void							SetMountRender(bool bStatus) { m_Config.bMountStatus = bStatus; }

		bool							GetShopNameStatus() { return m_Config.bNpcStatus; }
		void							SetShopNameStatus(bool bStatus) { m_Config.bNpcStatus = bStatus; }
#endif

		void SetFogEnvironment(const bool bFlag);
		bool IsFogEnvironment() const;

		void SetFogDistance(const float fDist);
		float GetFogDistance() const;

		void SetNightEnvironment(const bool bFlag);
		bool IsNightEnvironment() const;

		void SetSnowingEnvironment(const bool bFlag);
		bool IsSnowingEnvironment() const;

		void SetSnowTerrainTexture(const bool bFlag);
		bool IsSnowTerrainTexture() const;

		BYTE GetShadowLevel() const;
		bool SetShadowLevel(const BYTE bLevel);

#ifdef ENABLE_TRACK_WINDOW
		bool							GetDungeonTrack() { return m_Config.bDungeonTrack; }
		void							SetDungeonTrack(bool flag) { m_Config.bDungeonTrack = flag; }

		bool							GetBossTrack() { return m_Config.bBossTrack; }
		void							SetBossTrack(bool flag) { m_Config.bBossTrack = flag; }
#endif

		float                           GetFOVLevel() { return  m_Config.iFOVLevel; }
		void                            SetFOVLevel(const float fFOV) { m_Config.iFOVLevel = fMINMAX(30.0f, fFOV, 100.0f); }

		void SetShowOutlineFlag(const bool bShow)
		{
			m_Config.bShowOutline = bShow;
		}

#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBAIFLAG)
		bool							IsShowMobAIFlag();
		void							SetShowMobAIFlagFlag(int iFlag);
#endif
#if defined(WJ_SHOW_MOB_INFO) && defined(ENABLE_SHOW_MOBLEVEL)
		bool							IsShowMobLevel();
		void							SetShowMobLevelFlag(int iFlag);
#endif
#ifdef ENABLE_IKASHOP_RENEWAL
#ifdef EXTEND_IKASHOP_PRO
		void							SetShopRangeView(float distance) { m_Config.shopRangeView = MINMAX(600.0f, distance, 5000.f); }
		float							GetShopRangeView() const {return m_Config.shopRangeView;}
#endif
#endif

		// Window
		void							SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight);

		// SaveID
		int								IsSaveID();
		const char *					GetSaveID();
		void							SetSaveID(int iValue, const char * c_szSaveID);

		/// Display
		void							GetDisplaySettings();

		int								GetResolutionCount();
		int								GetFrequencyCount(int index);
		bool							GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp);
		bool							GetFrequency(int index, int freq_index, OUT DWORD *frequncy);
		int								GetResolutionIndex(DWORD width, DWORD height, DWORD bpp);
		int								GetFrequencyIndex(int res_index, DWORD frequency);
		bool							isViewCulling();

		// Sound
		float							GetMusicVolume();
		int								GetSoundVolume();
		void							SetMusicVolume(float fVolume);
		void							SetSoundVolumef(float fVolume);

		int								GetDistance();

#ifdef ENABLE_NEW_GAMEOPTION
		void							SetQuestLetter(bool flag) { m_Config.questLetter = flag; }
		bool							GetQuestLetter() { return m_Config.questLetter; }
		void							SetCountryFlag(bool flag) { m_Config.countryFlag = flag; }
		bool							GetCountryFlag() { return m_Config.countryFlag; }
		void							SetAffectIcons(bool flag) { m_Config.affectIcons = flag; }
		bool							GetAffectIcons() { return m_Config.affectIcons; }
#endif

#ifdef ENABLE_DISCORD_STUFF
		bool							GetStuffMessage() { return m_Config.bMessage; }
		void							SetStuffMessage(bool flag) { m_Config.bMessage = flag; }
#endif

	protected:
		TResolution						m_ResolutionList[RESOLUTION_MAX_NUM];
		int								m_ResolutionCount;

		TConfig							m_Config;
		TConfig							m_OldConfig;

		bool							m_isInterfaceConfig;
		PyObject *						m_poInterfaceHandler;
		TWindowStatus					m_WindowStatus[WINDOW_MAX_NUM];

#ifdef STONE_SCALE
	public:
		float GetStoneScale();
		void  SetStoneScale(float fScaleVal);
#endif
};