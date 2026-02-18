#include "StdAfx.h"
#include "AnticheatManager.h"
#include "CheatQueueManager.h"
#include "../eterPack/EterPackManager.h"

#include <XOrstr.h>
#include <fstream>

#if 0
std::string GetMappedFileHash(const std::string & strFileName)
{
	std::string strHash = "";

	if (strFileName.empty())
		return strHash;

//	if (CFileSystem::instance().isExistInPack(strFileName.c_str(), __FUNCTION__) == false)
	if (CEterPackManager::Instance().isExistInPack(strFileName.c_str()) == false)
		return strHash;

	CMappedFile lpMpFile;
	const void * pData = NULL;
//	if (CFileSystem::Instance().GetFromPack(lpMpFile, strFileName.c_str(), &pData) == false)
	if (CEterPackManager::Instance().GetFromPack(lpMpFile, strFileName.c_str(), &pData) == false)
		return strHash;

	if (lpMpFile.IsNull())
		return strHash;

	if (!pData)
		return strHash;

	strHash = CAnticheatManager::Instance().GetMd5((const BYTE*)pData, lpMpFile.Size());

	return strHash;
}

static struct stCheckFileList {
	const char*			c_szFileName;
	const char*			c_szFileMd5;
} st_FileHashList[] = {
	//	 filename										     md5

	{ "d:/ymir work/pc/warrior/horse/wait.msa",		"" }, // 0


	{ "XXX", 0 },
};

void HashDump(const char* cArgFormat, ...) 
{
	char cTmpString[2000];

	va_list vaArgList;
	va_start(vaArgList, cArgFormat);
	wvsprintfA(cTmpString, cArgFormat, vaArgList);
	va_end(vaArgList);

	std::ofstream f("hashs.txt", std::ofstream::out | std::ofstream::app);
	f << cTmpString << std::endl;
	f.close();
}

void DumpMappedFileHashes()
{
	for (size_t i = 0; strcmp(st_FileHashList[i].c_szFileName, "XXX"); ++i)
	{
		auto strCurrentFileName = st_FileHashList[i].c_szFileName;

//		char szXoredName[1024];
//		sprintf_s(szXoredName, "XOR(\"%s\")", strCurrentFileName);

		auto strHash = GetMappedFileHash(strCurrentFileName);

//		char szXoredHash[1024];
//		sprintf_s(szXoredHash, "XOR(\"%s\")", strHash.c_str());

		HashDump("{ \"%s\",		\"%s\" }, // %d", strCurrentFileName, strHash.c_str(), i);
	}

}

void CAnticheatManager::CheckMappedFiles()
{
//	DumpMappedFileHashes();

#if 0
	for (size_t i = 0; strcmp(st_FileHashList[i].c_szFileName, XOR("XXX")); ++i)
	{
		auto strCurrentFileName = st_FileHashList[i].c_szFileName;
		auto strCurrentFileHash = st_FileHashList[i].c_szFileHash;

		auto strCorrectFileHash = GetMappedFileHash(strCurrentFileName);

		if (strCorrectFileHash.empty() == false && strCorrectFileHash != strCurrentFileHash)
		{

		}
	}
#endif
}

#endif

