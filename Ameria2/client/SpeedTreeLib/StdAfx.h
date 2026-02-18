#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <assert.h>
//#include <crtdbg.h>

#include <m2_services.hpp>
#include <m2_length.hpp>
#include <m2_itemlength.hpp>
#include <m2_tables.hpp>
#include <m2_constants.hpp>

//#include "Forest.h"
#include "SpeedTreeForestDirectX8.h"
#include "../UserInterface/Locale_inc.h"

// Armadillo nanomite protection
#ifndef NANOBEGIN
	#ifdef __BORLANDC__
		#define NANOBEGIN     __emit__ (0xEB,0x03,0xD6,0xD7,0x01)
		#define NANOEND       __emit__ (0xEB,0x03,0xD6,0xD7,0x00)
	#else
		#define NANOBEGIN     __asm _emit 0xEB __asm _emit 0x03 __asm _emit 0xD6 __asm _emit 0xD7 __asm _emit 0x01
		#define NANOEND       __asm _emit 0xEB __asm _emit 0x03 __asm _emit 0xD6 __asm _emit 0xD7 __asm _emit 0x00
	#endif
#endif
