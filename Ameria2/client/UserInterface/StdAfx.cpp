// stdafx.cpp : source file that includes just the standard includes
//	UserInterface.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
std::string httpGet(const std::string& url) {
	HINTERNET hInternet = InternetOpen(TEXT("MyApp"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL) {
		return "empty";
	}
	HINTERNET hUrl = InternetOpenUrl(hInternet, TEXT(url.c_str()), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hUrl == NULL) {
		InternetCloseHandle(hInternet);
		return "empty";
	}
	std::string contents;
	char buffer[4096];
	DWORD bytesRead = 0;
	while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
		contents.append(buffer, bytesRead);
	}
	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);
	return contents;
}

#ifdef __AUTO_HUNT__
float GetDistanceNew(const TPixelPosition& PixelPosition, const TPixelPosition& c_rPixelPosition)
{
	const float fdx = PixelPosition.x - c_rPixelPosition.x;
	const float fdy = PixelPosition.y - c_rPixelPosition.y;
	return sqrtf((fdx * fdx) + (fdy * fdy));
}
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp>
void split_argument(const char* argument, std::vector<std::string>& vecArgs, const char* arg)
{
	boost::split(vecArgs, argument, boost::is_any_of(arg), boost::token_compress_on);
}
#endif