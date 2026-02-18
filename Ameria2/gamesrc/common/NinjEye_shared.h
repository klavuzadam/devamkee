//This file was generated, do not modify!
#ifndef _NINJEYE_GENERATED_HPP_
#define _NINJEYE_GENERATED_HPP_

#include <cstdint>

constexpr int32_t  CNinjEye_Key_Index1 = 3;
constexpr int32_t  CNinjEye_Key_Index2 = 4;
constexpr int32_t  CNinjEye_Key_Index3 = 10;
constexpr uint8_t  CNinjEye_Key_XOR1 = 172;
constexpr uint8_t  CNinjEye_Key_XOR2 = 210;
constexpr uint8_t  CNinjEye_Key_XOR3 = 252;
constexpr uint8_t  CNinjEye_Iv[] = { 149, 226, 1, 243, 0, 210, 219, 38, 80, 36, 67, 174, 150, 121, 25, 150 }; //!< AES IV
constexpr uint8_t  CNinjEye_Key[] = { 84, 109, 11, 241, 178, 155, 60, 29, 115, 32, 173, 225, 199, 243, 204, 173 }; //!< AES Key
constexpr int32_t  CNinjEye_ROROffset = 0; //!< ROR offset value
constexpr uint64_t CNinjEye_VersionKey = 778912347; //!< Metin2 Version key
constexpr char     CNinjEye_Hash[]     =  "c9759f479b26e42d2a784b2ff78d1796"; //!< NinjEye hash string
constexpr uint64_t CNinjEye_ValidationSeconds =  60; //!< NinjEye validation process interval (seconds)
constexpr uint64_t CNinjEye_ValidationTimeoutSeconds =  6; //!< NinjEye validation process timeout (seconds)

namespace ninjeye_utils 
{

#if WIN32
	__forceinline
#else
	constexpr inline 
#endif
	void MangleKey(uint8_t * Key) noexcept
	{
		Key[CNinjEye_Key_Index1] = Key[CNinjEye_Key_Index1] ^ CNinjEye_Key_XOR1;
		Key[CNinjEye_Key_Index2] = Key[CNinjEye_Key_Index2] ^ CNinjEye_Key_XOR2;
		Key[CNinjEye_Key_Index3] = Key[CNinjEye_Key_Index3] ^ CNinjEye_Key_XOR3;
	}

#if WIN32
	__forceinline
#else
	constexpr inline 
#endif
	void DemangleKey(uint8_t * Key) noexcept
	{
		Key[CNinjEye_Key_Index1] = Key[CNinjEye_Key_Index1] ^ CNinjEye_Key_XOR1;
		Key[CNinjEye_Key_Index2] = Key[CNinjEye_Key_Index2] ^ CNinjEye_Key_XOR2;
		Key[CNinjEye_Key_Index3] = Key[CNinjEye_Key_Index3] ^ CNinjEye_Key_XOR3;
	}
}

#endif
