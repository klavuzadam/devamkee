#include "StdAfx.h"
#include "AnticheatManager.h"
#include "CheatQueueManager.h"
#include "../eterBase/dirent.h"

#include <XORstr.h>


inline static int getdir(const std::string & dir, std::vector <std::string> & files)
{
	DIR * dp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		return errno;
	}

	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL)
	{
		auto file = std::string(dirp->d_name);
		if (file != "." && file != "..")
			files.push_back(std::string(dirp->d_name));
	}

	closedir(dp);
	return 0;
}

void CAnticheatManager::CheckMainFolderFiles()
{
	auto dir = std::string(".");
	auto files = std::vector<std::string>();

	getdir(dir, files);

	for (size_t i = 0; i < files.size(); i++) 
	{
		std::string file = files[i];
		std::transform(file.begin(), file.end(), file.begin(), tolower);

		if (file.substr(file.find_last_of(".") + 1) == XOR("mix") ||
			file.substr(file.find_last_of(".") + 1) == XOR("flt") || 
			file.substr(file.find_last_of(".") + 1) == XOR("asi") || 
			file.substr(file.find_last_of(".") + 1) == XOR("m3d") ||
			file.substr(file.find_last_of(".") + 1) == XOR("def")
#ifdef _RELEASE
			|| file.substr(file.find_last_of(".") + 1) == XOR("py")
#endif
			)
		{
			TraceError(XOR("Unallowed file found on main folder! File: %s"), file.c_str());
			abort();
		}

		if (file == XOR("mss32.dll") && GetFileMd5(file) != "a9e7ba647a766e8c604f23a7d98b57c9")
		{
			TraceError(XOR("mss32.dll file is corrupted! Please delete it and restart game"));
			abort();
		}

		if (file == XOR("devil.dll") && GetFileMd5(file) != "d87f07bbcb73806a963bb9cfff7f4c47")
		{
			TraceError(XOR("devil.dll file is corrupted! Please delete it and restart game"));
			abort();
		}
	}
}

void CAnticheatManager::CheckLibFolderForPythonLibs()
{
	if (IsDirExist(XOR("stdlib")))
	{
		TraceError(XOR("Please delete stdlib folder and restart game."));
		abort();
	}
}

void CAnticheatManager::CheckMilesFolderForMilesPlugins()
{
	auto dir = std::string(XOR("miles"));
	auto files = std::vector<std::string>();

	getdir(dir, files);

	if (files.size() > 9)
	{
		TraceError(XOR("Unknown file detected on miles folder! Please delete miles folder and restart game."));
		abort();
	}

	static std::map <std::string /* strFileName */, std::string /* strMd5 */> mapKnownFiles =
	{
		{ XOR("mssa3d.m3d"),	XOR("d8faab4403ff77c319801bdb7cdac793") },
		{ XOR("mssds3d.m3d"),	XOR("13d38198a40a05473ddd812704b6b2ac") },
		{ XOR("mssdsp.flt"),	XOR("2bb4258d8c6665d504a75b7ac82ab4d6") },
		{ XOR("mssdx7.m3d"),	XOR("07510e9243d95cf41b551a26df604411") },
		{ XOR("msseax.m3d"),	XOR("117128170f399eff23b862701559b22f") },
		{ XOR("mssmp3.asi"),	XOR("dba21dc54251a6a79c6265abf9dca598") },
		{ XOR("mssrsx.m3d"),	XOR("f859e5500bef92528edf70bb9fbb66ce") },
		{ XOR("msssoft.m3d"),	XOR("9a9bc91e30aeaf00dff0f01251c7a40f") },
		{ XOR("mssvoice.asi"), XOR("a06cb19d78450fbb0d6296fc20f8e61e") },
	};

	for (const auto & strCurrFolderFile : files)
	{
		auto strCurrFileLower = strCurrFolderFile;
		std::transform(strCurrFileLower.begin(), strCurrFileLower.end(), strCurrFileLower.begin(), tolower);

		auto it = mapKnownFiles.find(strCurrFileLower);
		if (it == mapKnownFiles.end())
		{
			TraceError(XOR("Unknown file detected on miles folder! File: %s"), strCurrFolderFile.c_str());
			abort();
		}

		std::string szPath = XOR("miles/");
		auto strCurrentMd5 = GetFileMd5(szPath + strCurrFileLower);
		auto strCorrectMd5 = it->second;
		if (strCurrentMd5 != strCorrectMd5)
		{
			TraceError(XOR("Corrupted file detected on miles folder! File: %s"), strCurrFolderFile.c_str());
			abort();
		}
	}
}

void CAnticheatManager::CheckYmirFolder()
{
	if (IsDirExist(XOR("d:/ymir work")) || IsDirExist(XOR("d:\\ymir work")))
	{
		TraceError(XOR("Unallowed folder: 'd:/ymir work' detected! Please delete it and restart game"));
		abort();
	}
}

