#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <psapi.h>
#include "CShieldCheckIntegrity.h"

#if defined(ENABLE_CSHIELD) && !defined(_DEBUG)
std::string GetExeFileName()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::string(buffer);
}

std::string GetExePath()
{
	std::string f = GetExeFileName();
	return f.substr(0, f.find_last_of("\\/"));
}


#ifdef ENABLE_CSHIELD_DEBUG
void ErrorMessage(std::string error)
{
	std::ofstream myfile("syserr.txt", std::ios::app);
	if (myfile.is_open())
	{
		myfile << error << "\n";
		myfile.close();
	}
}
#endif
#endif