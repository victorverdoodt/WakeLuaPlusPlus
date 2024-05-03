#pragma once

#include <lua.hpp>
#include <iostream>

class Script
{
public:
	lua_State* State;
	Script()
	{
		this->State = luaL_newstate();
		luaL_openlibs(this->State);
	}
	void ReportErrors(int state);
	void ExecuteFile(const char* file);
	void ExecuteString(const char* expression);
};