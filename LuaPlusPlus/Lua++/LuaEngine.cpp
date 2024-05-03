#include "Script.h"
#include "Engine.h"

void Script::ReportErrors(int state)
{
	if (state != 0)
	{
		TextConsole::GetInstance().AddLog(lua_tostring(State, state));
		std::cout << lua_tostring(State, state);
		lua_pop(State, 1);
	}
}

void Script::ExecuteFile(const char* file)
{
	if (file == NULL)
		return;
	int state = luaL_dofile(State, file);
	ReportErrors(state);
}

void Script::ExecuteString(const char* expression)
{
	if (expression == NULL)
		return;
	int state = luaL_dostring(State, expression);
	ReportErrors(state);
}
