#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>
#include <wtypes.h>
#include "const.h"

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) elems.push_back(item);
	}
	return elems;
}


static std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

static std::wstring s2ws(const std::string& str)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

static std::string ws2s(const std::wstring& wstr)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

#define baseAddr (DWORD)GetModuleHandle(NULL)

template <class T> T ReadMemory(DWORD dwAddrToRead)
{
	return *(T*)dwAddrToRead;
}

template <class T> bool WriteMemory(DWORD Address, T Valuee)
{
	*(T*)Address = Valuee;
	return true;
}

inline char* GetStr(DWORD offset) {
	auto smallBufferSize = 0xE0;
	auto lengthAddress = offset + smallBufferSize;
	auto length = *(int*)(offset + smallBufferSize);

	if (length < smallBufferSize)
		return (char*)offset;
	else
		return (char*)(*(DWORD*)offset);
}
#define BLOCK_SIZE 32

static bool isPartOf(const char* w1, const char* w2)
{
	int i = 0;
	int j = 0;


	while (w1[i] != '\0') {
		if (w1[i] == w2[j])
		{
			int init = i;
			while (w1[i] == w2[j] && w2[j] != '\0')
			{
				j++;
				i++;
			}
			if (w2[j] == '\0') {
				return true;
			}
			j = 0;
		}
		i++;
	}
	return false;
}