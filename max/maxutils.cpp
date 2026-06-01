#include "maxutils.h"

std::string fmnext::convertWideToStdString(const std::wstring& str)
{
	std::string result;
	result.resize(str.size());

	wcstombs_s(nullptr, result.data(), str.size() + 1, str.c_str(), _TRUNCATE);

	return result;
}

std::wstring fmnext::convertStdStringToWide(const std::string& str)
{
	std::wstring result;
	result.resize(str.size());

	mbstowcs_s(nullptr, result.data(), str.size() + 1, str.c_str(), _TRUNCATE);

	return result;
}

void fmnext::PrinttoListener(const std::string& str)
{
	mprintf(L"%s\n", fmnext::convertStdStringToWide(str).c_str());
}

void fmnext::PrinttoListener(const std::wstring& str)
{
	mprintf(L"%s\n", str.c_str());
}