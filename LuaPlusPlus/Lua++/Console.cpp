#include "Engine.h"
#include "console.h"

TextConsole & TextConsole::GetInstance() {
	static TextConsole  instance;
	return instance;
}