#ifndef __METIN_II_GAME_DUNGEON_COOLDOWN_MGR_HPP__
#define __METIN_II_GAME_DUNGEON_COOLDOWN_MGR_HPP__

#if VSTD_HAS_PRAGMA_ONCE
#pragma once
#endif

#ifdef USE_DUNGEON_COOLDOWN_MANAGER
class CDungeonHwidCooldown
{
	public:
		CDungeonHwidCooldown(const char* cpszHwid, const int32_t mapindex, const uint32_t cooldowntime, const int slot, const uint32_t pid);
		virtual ~CDungeonHwidCooldown();

	private:
		std::string m_strHwid;
		uint32_t m_dwPID;
		int32_t m_iMapIndex;
		uint32_t m_tCooldownTime;
		int m_iSlot;

	public:
		void Update();

		uint32_t GetCooldownTime() const;
		bool SetCooldownTime(const uint32_t cooldowntime);

		uint32_t GetPID() const
		{
			return m_dwPID;
		}

		int GetSlot() const
		{
			return m_iSlot;
		}
};

class CDungeonCooldownMgr : public singleton<CDungeonCooldownMgr>
{
	public:
		CDungeonCooldownMgr();
		virtual ~CDungeonCooldownMgr();

	private:
		std::unordered_map<std::string, std::unique_ptr<CDungeonHwidCooldown>> m_mapCooldown;
		bool m_bUpdating;

	public:
		void Clear();
		void Initialize(TPacketDungeonCooldownMgrSet* data, size_t size);

		int8_t Set(LPCHARACTER ch, const int32_t mapindex, const uint32_t cooldowntime);
		bool Replace(LPCHARACTER ch, const int32_t mapindex, const uint32_t cooldowntime);
		uint32_t Get(LPCHARACTER ch, const int32_t mapindex);
		bool GetExcept(const std::string& stHwid, const int32_t mapindex, const int32_t count);
		int8_t Reset(LPCHARACTER ch, const int32_t mapindex);
		int8_t ResetSingle(LPCHARACTER ch, const int32_t mapindex);

		void RecvAdd(const TPacketDungeonCooldownMgrSet* p);
		void RecvExpire(const char* key);
};
#endif // USE_DUNGEON_COOLDOWN_MANAGER
#endif // __METIN_II_GAME_DUNGEON_COOLDOWN_MGR_HPP__
