//
//   Copyright (C) 2023 NinjEye
//   Author(s)    : Mihai PANDURU (mpanduru@ninjeye.net)
//

#include "StdAfx.h"
#include "NinjEye.hpp"

std::promise<VOID> NinjEye::Internals::gPromise;
ULONG NinjEye::Internals::gInternalFunctionAddress = 0UL;
