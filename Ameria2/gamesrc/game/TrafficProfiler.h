/**
 *
 * @file	TrafficProfiler.h
 * @brief	TrafficProfiler class definition file
 * @author	Bang2ni
 * @version	05/07/07 Bang2ni - First release.
 *
 */

#ifndef _METIN_II_TRAFFICPROFILER_H_
#define _METIN_II_TRAFFICPROFILER_H_

/**
 * @class	TrafficProfiler
 * @brief	Network I/O traffic À» ÆÐÅ¶ ´ÜÀ§·Î ÃøÁ¤ÇÏ´Â profiler.
 * @author	Bang2ni
 * @version	05/07/07 Bang2ni - First release.
 *
 * ½Ã°£´ë º°·Î Network I/O ÀÇ traffic À» ÆÐÅ¶ ´ÜÀ§·Î ÃøÁ¤ÇÏ°í, Text file ÇüÅÂ·Î º¸°í¼­¸¦ ÀÛ¼ºÇÑ´Ù.
 */
class TrafficProfiler : public singleton< TrafficProfiler >
{
	public:

		/// I/O ¹æÇâ
		enum IODirection {
			IODIR_INPUT	= 0,	///< Input
			IODIR_OUTPUT,	///< Output
			IODIR_MAX
		};

	public:

		/// Constructor
		TrafficProfiler( void );

		/// Destructor
		~TrafficProfiler( void );

		/// Profiling ¿¡ ÇÊ¿äÇÑ ÃÊ±âÈ­¸¦ ÇÑ´Ù.
		/**
		 * @param [in]	dwFlushCycle Flush ÁÖ±â. ÃÊ ´ÜÀ§ÀÌ´Ù.
		 * @param [in]	pszLogFileName Profiling log file ÀÇ ÀÌ¸§
		 * @return	false ÀÏ °æ¿ì profiling log file À» open ÇÏÁö ¸øÇß´Ù.
		 *
		 * profiling log file À» open(»ý¼º) ÇÑ´Ù.
		 */
		bool	Initialize( DWORD dwFlushCycle, const char* pszLogFileName );

		/// Profiling À» À§ÇØ Àü¼ÛµÆ°Å³ª Àü¼Û ÇÒ Packet À» Report ÇÑ´Ù.
		/**
		 * @param [in]	dir Profiling ÇÒ Packet ÀÇ ¹æÇâ
		 * @param [in]	byHeader Packet Çì´õ
		 * @param [in]	dwSize Packet ÀÇ ÃÑ size
		 * @return	Initialize µÇÁö ¾Ê¾Ò´Ù¸é false ¸¦ ¹ÝÈ¯ÇÑ´Ù.
		 *
		 * Packet ¿¡ ÇØ´çÇÏ´Â size ¸¦ ´©Àû½ÃÅ²´Ù.
		 * Initialize ÀÌÈÄ³ª ÃÖ±Ù Flush µÈ ÀÌÈÄ¿¡ Flush ÁÖ±â ¸¸Å­ ½Ã°£ÀÌ Èå¸¥ ÈÄ È£ÃâµÈ´Ù¸é Report ÀÌÈÄ Flush ÇÑ´Ù.
		 */
		bool	Report( IODirection dir, BYTE byHeader, DWORD dwSize )
		{
			ComputeTraffic( dir, byHeader, dwSize );
			if ( (DWORD)(time( NULL ) - m_tmProfileStartTime) >= m_dwFlushCycle )
				return Flush();
			return true;
		}

		/// ÇöÀç±îÁö Report µÈ ³»¿ëÀ» ÆÄÀÏ¿¡ ¾´´Ù.
		/**
		 * @return	Initialize µÇÁö ¾Ê¾Ò´Ù.
		 */
		bool	Flush( void );

	private:

		/// Profling ¿¡ °ü·ÃµÈ variables ¸¦ ÃÊ±âÈ­ ÇÑ´Ù.
		void	InitializeProfiling( void );

		/// Report µÈ Packet ÀÇ traffic ¸¦ °è»êÇÑ´Ù.
		/**
		 * @param [in]	dir Profiling ÇÒ Packet ÀÇ ¹æÇâ
		 * @param [in]	byHeader Packet Çì´õ
		 * @param [in]	dwSize Packet ÀÇ ÃÑ size
		 */
		void	ComputeTraffic( IODirection dir, BYTE byHeader, DWORD dwSize )
		{

			TrafficInfo& rTrafficInfo = m_aTrafficVec[ dir ][ byHeader ];

			m_dwTotalTraffic += dwSize;
			m_dwTotalPacket += !rTrafficInfo.second;

			rTrafficInfo.first += dwSize;
			rTrafficInfo.second++;
		}

		/// Traffic info type.
		/**
		 * first: ´©ÀûµÈ ÃÑ size
		 * second: ÀÌ packet ÀÌ Àü¼ÛµÈ È½¼ö
		 */
		typedef std::pair< DWORD, DWORD >	TrafficInfo;

		/// Traffic info vector.
		typedef std::vector< TrafficInfo >	TrafficVec;

		FILE*		m_pfProfileLogFile;	///< Profile log file pointer
		DWORD		m_dwFlushCycle;		///< Flush ÁÖ±â
		time_t		m_tmProfileStartTime;	///< ÇÁ·ÎÆÄÀÏÀ» ½ÃÀÛÇÑ ½Ã°£. Flush µÉ ¶§¸¶´Ù Update µÈ´Ù.
		DWORD		m_dwTotalTraffic;	///< Report µÈ ÃÑ Traffic ¿ë·®
		DWORD		m_dwTotalPacket;	///< Report µÈ ÃÑ Packet ¼ö
		TrafficVec	m_aTrafficVec[ IODIR_MAX ];	///< Report µÈ Traffic À» ÀúÀåÇÒ vectorÀÇ ¹è¿­. °¢ ¹æÇâ¸¶´Ù vector ¸¦ °¡Áø´Ù.
};

#endif // _METIN_II_TRAFFICPROFILER_H_
