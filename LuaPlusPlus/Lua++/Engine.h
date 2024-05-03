#pragma once
#define WIN32_LEAN_AND_MEAN
#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 1
#include <atlstr.h>
#include <windows.h>
#include <vector>
#include <d3d9.h>
#include <lua.hpp>
#include <LuaIntf.h>
#include <map>
#include <functional>

#include "Struct.h"
#include "detours.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Console.h"
#include "Script.h"

static class Engine
{
public:
	static Game GamePtr;
	static ProtocolGamePtr* getGameProtocol();
	static LocalPLayerPtr* getLocalPlayer();

	static bool doAttack(CreaturePtr * creature);
	static bool doWalkToPosition(Position toPos);
	static void doStopAutoWalk();
	static void useInventoryItem(int itemId);
	static bool isContainer(int thing);
	static int getStackPos(int thing);
	static void useOnCreature(int itemId, CreaturePtr * creature);
	static void UseThing(int thing);
	static void useOnItem(int itemId, ItemPtr toItem);
	static void talkChannel(Otc::MessageMode mode, int channelId, const std::string message);
	static void talk(const char* message);
	static void useSlotItem(int slot);
	static std::string getCharacterName();
	static bool isOnline();
	static CreaturePtr* GetTarget();
	static void useItem(ItemPtr* item);
	static ItemPtr* getSlotItem(int slot);
	static CreaturePtr* getMySummon();
	static std::vector<CreaturePtr*> GetBList();
	static void SendKeyStroke(int key);
	static std::string getWorldName();
	static std::string GetLastMessage();


	static std::vector<Script*> Instances;
	static std::vector<std::string> Scripts;
	static std::string g_Path;
	static CreaturePtr* mySummon;
	static std::vector<CreaturePtr*> CreatureCache;
	static void Init(lua_State* L);
	static void InitImgui(lua_State* L);
	static std::string GetStartupPath(std::string append);
	static void LoadScript(std::string file);
};