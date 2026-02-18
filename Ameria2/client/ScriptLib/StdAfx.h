#pragma once

#include "../eterLib/StdAfx.h"
#include "../eterGrnLib/StdAfx.h"

#ifdef AT
#undef AT // @warme667
#endif

#pragma warning(disable:5033) // supress 'register' storage class

#ifdef _DEBUG
#undef _DEBUG
#include <python-2.7/Python.h>
#define _DEBUG
#else
#include <python-2.7/Python.h>
#endif
#include <python-2.7/node.h>
#include <python-2.7/grammar.h>
#include <python-2.7/token.h>
#include <python-2.7/parsetok.h>
#include <python-2.7/errcode.h>
#include <python-2.7/compile.h>
typedef struct _mod *mod_ty;
#include <python-2.7/symtable.h>
#include <python-2.7/eval.h>
#include <python-2.7/marshal.h>
#include <python-2.7/modsupport.h>

#ifdef BYTE
#undef BYTE
#endif

#ifdef AT
#undef AT
#endif

#include "PythonUtils.h"
#include "PythonLauncher.h"
#include "PythonMarshal.h"
#include "Resource.h"

void initdbg();

// PYTHON_EXCEPTION_SENDER
class IPythonExceptionSender
{
	public:
		void Clear()
		{
			m_strExceptionString = "";
		}

		void RegisterExceptionString(const char * c_szString)
		{
			m_strExceptionString += c_szString;
		}

		virtual void Send() = 0;

	protected:
		std::string m_strExceptionString;
};

extern IPythonExceptionSender * g_pkExceptionSender;

void SetExceptionSender(IPythonExceptionSender * pkExceptionSender);
// END_OF_PYTHON_EXCEPTION_SENDER
