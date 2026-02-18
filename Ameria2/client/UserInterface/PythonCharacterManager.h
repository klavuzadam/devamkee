#pragma once

#include "AbstractCharacterManager.h"
#include "InstanceBase.h"
#include "../GameLib/PhysicsObject.h"

class CPythonCharacterManager : public CSingleton<CPythonCharacterManager>, public IAbstractCharacterManager, public IObjectManager
{
	public:
		// Character List
		typedef std::list<CInstanceBase *>			TCharacterInstanceList;
		typedef std::map<DWORD, CInstanceBase *>	TCharacterInstanceMap;

		class CharacterIterator;

	public:
		CPythonCharacterManager();
		virtual ~CPythonCharacterManager();

		virtual void AdjustCollisionWithOtherObjects(CActorInstance* pInst );

		void EnableSortRendering(bool isEnable);

		bool IsRegisteredVID(DWORD dwVID);
		bool IsAliveVID(DWORD dwVID);
		bool IsDeadVID(DWORD dwVID);
		bool IsCacheMode();

#ifdef ENABLE_ULTIMATE_REGEN
		void GetMobWithVnum(DWORD bossVnum, std::vector<CInstanceBase*>& m_Data);
#endif

		bool OLD_GetPickedInstanceVID(DWORD* pdwPickedActorID);
		CInstanceBase* OLD_GetPickedInstancePtr();
		D3DXVECTOR2& OLD_GetPickedInstPosReference();

		CInstanceBase* FindClickableInstancePtr();

#ifdef __AUTO_HUNT__
		CInstanceBase* FindVictim(CInstanceBase* pkInstMain, float fMaxDistance, TPixelPosition* startPoint);
#endif

		void InsertPVPKey(DWORD dwVIDSrc, DWORD dwVIDDst);
		void RemovePVPKey(DWORD dwVIDSrc, DWORD dwVIDDst);
		void ChangeGVG(DWORD dwSrcGuildID, DWORD dwDstGuildID);

		void GetInfo(std::string* pstInfo);

		void ClearMainInstance();
		bool SetMainInstance(DWORD dwVID);
		CInstanceBase* GetMainInstancePtr();

		void								SCRIPT_SetAffect(DWORD dwVID, DWORD eAffect, BOOL isVisible);
		void								SetEmoticon(DWORD dwVID, DWORD eEmoticon);
		bool								IsPossibleEmoticon(DWORD dwVID);
		void								ShowPointEffect(DWORD dwVID, DWORD ePoint);
		bool								RegisterPointEffect(DWORD ePoint, const char* c_szFileName);

		// System
		void								Destroy();

		void								DeleteAllInstances();

		bool								CreateDeviceObjects();
		void								DestroyDeviceObjects();

		void								Update();
		void								Deform();
		void								Render();
		void								RenderShadowMainInstance();
		void								RenderShadowAllInstances();
		void								RenderCollision();

		// Create/Delete Instance
		CInstanceBase *						CreateInstance(const CInstanceBase::SCreateData& c_rkCreateData);
		CInstanceBase *						RegisterInstance(DWORD VirtualID);

		void								DeleteInstance(DWORD VirtualID);
		void								DeleteInstanceByFade(DWORD VirtualID);
		void								DeleteVehicleInstance(DWORD VirtualID);

#ifdef HIDE_OBJECTS
		void								SetVisibleShops(bool visible);
#endif
		
		void 								DestroyAliveInstanceMap();
		void 								DestroyDeadInstanceList();

		inline CharacterIterator			CharacterInstanceBegin() { return CharacterIterator(m_kAliveInstMap.begin());}
		inline CharacterIterator			CharacterInstanceEnd() { return CharacterIterator(m_kAliveInstMap.end());}

		// Access Instance
		void								SelectInstance(DWORD VirtualID);
		CInstanceBase *						GetSelectedInstancePtr();

		CInstanceBase *						GetInstancePtr(DWORD VirtualID);
		CInstanceBase *						GetInstancePtrByName(const char *name);

		// Pick
		int									PickAll();
		CInstanceBase *						GetCloseInstance(CInstanceBase * pInstance);

		// Refresh TextTail
		void								RefreshAllPCTextTail();
		void								RefreshAllGuildMark();

	protected:
		void								UpdateTransform();
		void								UpdateDeleting();

#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
		// Automatic Hunting
	public:
		void SetAutoOnOff(const bool c_bOnOff
#ifndef USE_PREMIUM_AFFECT
		, const bool c_bCommand = false
#endif
		);
		bool GetAutoOnOff() { return m_bAutoHuntOnOff; }
	
		void SetAutoSystemByType(const uint8_t c_iType, const bool c_bOnOff);
		uint8_t GetAutoSystemByType(const uint8_t c_iType) { return m_abAutoHuntType[c_iType]; }
	
		void SetLastPixelPosition(TPixelPosition rkInstPos) { m_rkLastPixelPos = rkInstPos; }
		TPixelPosition GetLastPixelPosition() { return m_rkLastPixelPos; }
	
	protected:
		bool m_bAutoHuntOnOff;
#ifndef USE_PREMIUM_AFFECT
		bool m_bAutoHuntCommand;
#endif
		bool m_abAutoHuntType[CInstanceBase::AUTO_ONOFF_MAX];
	
		TPixelPosition m_rkLastPixelPos;
#endif

#ifdef ENABLE_ENTITY_LOADING
	// Entity
	public:
		void CreateEntity(const DWORD c_dwVID, CInstanceBase::SCreateData& c_rkCreateData);

	protected:
		using EntityMap = std::map<DWORD, CInstanceBase::SCreateData>;
		EntityMap m_kEntityMap;
#endif

	protected:
		void __Initialize();

		void __DeleteBlendOutInstance(CInstanceBase* pkInstDel);

		void __OLD_Pick();
		void __NEW_Pick();

		void __UpdateSortPickedActorList();
		void __UpdatePickedActorList();
		void __SortPickedActorList();

		void __RenderSortedAliveActorList();
		void __RenderSortedDeadActorList();

	protected:
		CInstanceBase *						m_pkInstMain;
		CInstanceBase *						m_pkInstPick;
		CInstanceBase *						m_pkInstBind;
		D3DXVECTOR2							m_v2PickedInstProjPos;

		TCharacterInstanceMap				m_kAliveInstMap;
		TCharacterInstanceList				m_kDeadInstList;

		std::vector<CInstanceBase*>			m_kVct_pkInstPicked;

		DWORD								m_adwPointEffect[255];

	public:
		class CharacterIterator
		{
		public:
			CharacterIterator(){}
			CharacterIterator(const TCharacterInstanceMap::iterator & it) : m_it(it) {}

			inline CInstanceBase * operator * () {	return m_it->second; }

			inline CharacterIterator & operator ++()
			{
				++m_it;
				return *this;
			}

			inline CharacterIterator operator ++(int)
			{
				CharacterIterator new_it = *this;
				++(*this);
				return new_it;
			}

			inline CharacterIterator & operator = (const CharacterIterator & rhs)
			{
				m_it = rhs.m_it;
				return (*this);
			}

			inline bool operator == (const CharacterIterator & rhs) const
			{
				return m_it == rhs.m_it;
			}

			inline bool operator != (const CharacterIterator & rhs) const
			{
				return m_it != rhs.m_it;
			}

			private:
				TCharacterInstanceMap::iterator m_it;
		};
};
