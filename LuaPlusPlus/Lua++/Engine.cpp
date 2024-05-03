#include "declarations.h"
#include "Engine.h"
#include "LuaMain.h"
#include <algorithm>
#include "Offsets.h"


std::vector<Script*> Engine::Instances;
std::vector<std::string> Engine::Scripts;
std::string Engine::g_Path;
std::vector<CreaturePtr*> Engine::CreatureCache;
CreaturePtr* Engine::mySummon;
Game Engine::GamePtr;

namespace LuaIntf
{
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_SHARED_PTR_TYPE(std::shared_ptr)
}

#define MULTI_LINE_STRING(...) #__VA_ARGS__

ProtocolGamePtr*  Engine::getGameProtocol() {
	return GamePtr.GetGameProtocol();
}

LocalPLayerPtr* Engine::getLocalPlayer() {
	return GamePtr.GetLocalPlayer();
}

CreaturePtr* Engine::getMySummon() {
	return mySummon;
}

std::vector<CreaturePtr*> Engine::GetBList() {
	return CreatureCache;
}

std::string Engine::getCharacterName() {
	std::string someString(GamePtr.GetCharacterName());
	return someString;
}

std::string Engine::getWorldName() {
	std::string someString(GamePtr.GetWorldName());
	return someString;
}

bool Engine::isOnline() {
	return GamePtr.isOnline();
}

CreaturePtr* Engine::GetTarget() {
	return GamePtr.GetAttackTarget();
}

bool Engine::doWalkToPosition(Position toPos) {
	if (!getLocalPlayer()->isWalking()) {
		Functions.autoWalk((DWORD*)getLocalPlayer(), toPos);
		return true;
	}
	return false;
}

void Engine::doStopAutoWalk() {
	Functions.stopAutoWalk((DWORD*)getLocalPlayer());
}

void Engine::useInventoryItem(int itemId) {
	Position pos; // means that is a item in inventory
	pos.x = 0xFFFF;
	pos.y = 0;
	pos.z = 0;
	Functions.sendUseItem(getGameProtocol(), pos, itemId, 0, 0);
}

bool Engine::isContainer(int thing) {
	return Functions.isContainer(thing);
}

int Engine::getStackPos(int thing) {
	return Functions.getStackPos(thing);
}

void Engine::useOnCreature(int itemId, CreaturePtr * creature)
{
	if (!creature)
		return;

	Position pos; // means that is a item in inventory
	pos.x = 0xFFFF;
	pos.y = 0;
	pos.z = 0;

	if (creature)
		Functions.sendUseOnCreature(getGameProtocol(), pos, itemId, 0, creature->GetId());
}
//	bool isValid() const { return !(x == 65535 && y == 65535 && z == 255); }
void Engine::UseThing(int thing)
{
	ItemPtr item = ReadMemory<ItemPtr>(thing);
	Position pos = item.getPosition();
	if (pos.x == 65535 && pos.y == 65535 && pos.z == 255) // virtual item
		pos.x = 0xFFFF;
		pos.y = 0;
		pos.z = 0;

	Functions.sendUseItem(getGameProtocol(), item.getPosition(), item.getServerId(), getStackPos(thing), 1);
}

void Engine::useOnItem(int itemId, ItemPtr toItem) {
	Position pos; // means that is a item in inventory
	pos.x = 0xFFFF;
	pos.y = 0;
	pos.z = 0;
	//Functions.sendUseItemWith(Engine::getGameProtocol(), pos, itemId, 0, toItem.getPosition(), toItem.getServerId(), getStackPos(getTopThing(toItem.getPosition())));
}

// int mode, int channelId, const std::string& receiver, const std::string& message
void Engine::talkChannel(Otc::MessageMode mode, int channelId, const std::string message)
{
	Functions.sendTalk(getGameProtocol(), mode, channelId, "", message.c_str());
}

void Engine::talk(const char* message) {
	Functions.Talk(ReadMemory<DWORD*>(baseAddr + gGame), Otc::MessageSay, 0, message);
	//talkChannel(Otc::MessageSay, 0, message);
}

void Engine::useSlotItem(int slot) {
	auto slotItem = getLocalPlayer()->getSlotItem(slot);
	if (slotItem) {
		TextConsole::GetInstance().AddLog("%i - %i", slotItem->getClientId(), slotItem->getServerId());
		useInventoryItem(slotItem->getSlotItem());
	}
}

ItemPtr* Engine::getSlotItem(int slot) {
	return getLocalPlayer()->getSlotItem(slot);
}

bool Engine::doAttack(CreaturePtr* creature) {
	if (GetTarget() == (CreaturePtr*)getLocalPlayer())
		return false;

	if (creature && GetTarget())
		return false;

	WriteMemory<CreaturePtr*>(baseAddr + gGame + 0x4, creature);
	Functions.sendAttack(getGameProtocol(), creature->GetId(), 0);
	return true;
}

void Engine::useItem(ItemPtr* item)
{
	Functions.sendUseItem(getGameProtocol(), item->getPosition(), item->getServerId(), item->getPosition().z, 1);
}

void Engine::SendKeyStroke(int key)
{
	keybd_event(key, 0x9C, 0, 0);
	keybd_event(key, 0x9C, KEYEVENTF_KEYUP, 0);
}

void Engine::InitImgui(lua_State *L) {
	using namespace LuaIntf;
	LuaBinding(L)
		.beginModule("GUI")
		.addFunction("SetNextWindowPos", [](int x, int y) {
		ImGui::SetNextWindowPos(ImVec2(x, y));
	})
		.addFunction("Begin", [](const char* name, bool open, int flags) {
		return ImGui::Begin(name, &open, flags);
	})
		.addFunction("BeginOverlay", [](const char* name, bool open, int x, int y) {
		ImGui::SetNextWindowPos(ImVec2(x, y));
		return ImGui::Begin(name, &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
	})
		.addFunction("SetNextWindowSize", [](int x, int y, int flags) {
		ImGui::SetNextWindowSize(ImVec2(x, y), flags);
	})
		.addFunction("BeginTabBar", [](const char* name, int flags) {
		return ImGui::BeginTabBar(name, flags);
	})
		.addFunction("EndTabBar", []() {
		ImGui::EndTabBar();
	})
		.addFunction("BeginTabItem", [](const char* name) {
		return ImGui::BeginTabItem(name);
	})
		.addFunction("EndTabItem", []() {
		ImGui::EndTabItem();
	})
		.addFunction("End", []() {
		ImGui::End();
	})
		.addFunction("Columns", [](int q, const char* label) {
		ImGui::Columns(q, label);
	})
		.addFunction("NextColumn", []() {
		ImGui::NextColumn();
	})
		.addFunction("PushID", [](int id) {
		ImGui::PushID(id);
	})
		.addFunction("PopID", []() {
		ImGui::PopID();
	})
		.addFunction("TextLabel", [](const char* name, const char* text) {
		ImGui::LabelText(name, text);
	})
		.addFunction("Text", [](const char* text) {
		ImGui::Text(text);
	})
		.addFunction("TextWrapped", [](const char* text) {
		ImGui::TextWrapped(text);
	})
		.addFunction("TextColored", [](ImVec4 col, const char* text) {
		ImGui::TextColored(col, text);
	})
		.beginClass<Color>("Color")
		.addConstructor(LUA_ARGS())
		.addVariable("r", &Color::r)
		.addVariable("g", &Color::g)
		.addVariable("b", &Color::b)
		.endClass()
		.addFunction("ColorEdit3", [](const char* label, Color* col) {
		return ImGui::ColorEdit3(label, (float*)col);
	})
		.addFunction("Button", [](const char* label) {
		return ImGui::Button(label);
	})
		.addFunction("SmallButton", [](const char* label) {
		return ImGui::SmallButton(label);
	})
		.addFunction("Selectable", [](const char* label, bool selected) {
		return ImGui::Selectable(label, selected);
	})
		.addFunction("SliderInt", [](const char* label, int v, int min, int max) {
		bool changed = ImGui::SliderInt(label, &v, min, max);
		return std::tuple<bool, int>(changed, v);
	})
		.addFunction("InputInt", [](const char* label, int v) {
		bool changed = ImGui::InputInt(label, &v);
		return std::tuple<bool, int>(changed, v);
	})
		.addFunction("SliderFloat", [](const char* label, float v, float min, float max) {
		bool changed = ImGui::SliderFloat(label, &v, min, max);
		return std::tuple<bool, float>(changed, v);
	})
		.addFunction("CheckBox", [](const char* label, bool v) {
		bool changed = ImGui::Checkbox(label, &v);
		return  v;
	})
		.addFunction("TreeNode", [](const char* label) {
		return ImGui::TreeNode(label);
	})
		.addFunction("TreePop", []() {
		ImGui::TreePop();
	})
		.addFunction("Separator", []() {
		ImGui::Separator();
	})
		.addFunction("SameLine", [](int column_x, int spacing_w) {
		ImGui::SameLine(column_x, spacing_w);
	}, LUA_ARGS(_def<int, 0>, _def<int, -1>))
		.addFunction("Combo", [](const char* label, int current, LuaRef ref) {
		std::vector<const char*> list = LuaIntf::Lua::getList<std::vector<const char*>>(ref);
		ImGui::Combo(label, &current, list.data(), list.size());

		return current;
	})
		.addFunction("ListBox", [](const char* label, int current, LuaRef ref) {
		std::vector<const char*> list = LuaIntf::Lua::getList<std::vector<const char*>>(ref);
		ImGui::ListBox(label, &current, list.data(), list.size());

		return current;
	}).addFunction("InputText", [](std::string label, std::string& text, int max_size, int flags)
	{
		char* buffer = new char[max_size]();
		text.copy(buffer, text.size() < max_size ? text.size() : max_size);
		bool ret = ImGui::InputText(label.c_str(), buffer, max_size, flags);
		text = buffer;
		delete[] buffer;
		return std::tuple<bool, std::string>(ret, text);
	}, LUA_ARGS(std::string, _ref<std::string&>, _def<int, 255>, _def<int, 0>))
		.addFunction("KeyPressed", [](int vkey) {
		return !!(GetAsyncKeyState(vkey) & 0x8000);
	})
		.addFunction("AddLog", [](const char* message) {
		TextConsole::GetInstance().AddLog(message);
		std::cout << message;
	})
		.beginClass<ImGuiTextFilter>("Filter")
		.addConstructor(LUA_ARGS(const char*))
		.addFunction("Draw", &ImGuiTextFilter::Draw)
		.addFunction("Clear", &ImGuiTextFilter::Clear)
		.addFunction("PassFilter", &ImGuiTextFilter::PassFilter)
		.addFunction("Build", &ImGuiTextFilter::Build)
		.addFunction("IsActive", &ImGuiTextFilter::IsActive)
		.endClass()
	.endModule();
}


void Engine::Init(lua_State *L)
{
	using namespace LuaIntf;
	LuaBinding(L)
		.beginClass<Position>("Position")
		.addVariable("x", &Position::x)
		.addVariable("y", &Position::y)
		.addVariable("z", &Position::z)
		.endClass();

	LuaBinding(L)
		.beginClass<ItemPtr>("Item")
		.addFunction("GetClientId", &ItemPtr::getClientId)
		.addFunction("GetPosition", &ItemPtr::getPosition)
		.addFunction("GetServerId", &ItemPtr::getServerId)
		.addFunction("GetItemSlot", &ItemPtr::getSlotItem)
		.addFunction("DoUseItem", &ItemPtr::useItem)
		.endClass();

	LuaBinding(L)
		.beginClass<LocalPLayerPtr>("LocalPlayer")
		.addFunction("GetLight", &LocalPLayerPtr::GetLight)
		.addFunction("SetLight", &LocalPLayerPtr::SetLight)
		.addFunction("GetId", &LocalPLayerPtr::GetId)
		.addFunction("GetName", &LocalPLayerPtr::getNameString)
		.addFunction("GetLevel", &LocalPLayerPtr::getLevel)
		.addFunction("GetHealth", &LocalPLayerPtr::getHealth)
		.addFunction("GetMaxHealth", &LocalPLayerPtr::getMaxHealth)
		.addFunction("GetHealthPercent", &LocalPLayerPtr::getHealthPercent)
		.addFunction("GetMana", &LocalPLayerPtr::getMana)
		.addFunction("GetMaxMana", &LocalPLayerPtr::getMaxMana)
		.addFunction("GetLookDirection", &LocalPLayerPtr::GetLookDirection)
		.addFunction("IsWalking", &LocalPLayerPtr::isWalking)
		.addFunction("GetPosition", &LocalPLayerPtr::GetPosition)
		.endClass();

	LuaBinding(L)
		.beginClass<CreaturePtr>("Creature")
		.addFunction("GetLight", &CreaturePtr::GetLight)
		.addFunction("SetLight", &CreaturePtr::SetLight)
		.addFunction("GetId", &CreaturePtr::GetId)
		.addFunction("GetLookDirection", &CreaturePtr::GetLookDirection)
		.addFunction("GetName", &CreaturePtr::getNameString)
		.addFunction("IsPlayer", &CreaturePtr::isPlayer)
		.addFunction("IsNpc", &CreaturePtr::isMonsterOrNpc)
		.addFunction("IsSummon", &CreaturePtr::isSummon)
		.addFunction("GetHealthPercent", &CreaturePtr::getHealthPercent)
		.addFunction("GetPosition", &CreaturePtr::GetPosition)
		.addFunction("IsValid", &CreaturePtr::isValid)
		.addFunction("IsWalking", &CreaturePtr::isWalking)
		.endClass();

	LuaBinding(L)
		.beginModule("Engine")
		.addFunction("IsOnline", &isOnline)
		.addFunction("GetLocalPlayer", &getLocalPlayer)
		.addFunction("GetCharacterName", &getCharacterName)
		.addFunction("GetWorldName", &getWorldName)
		.addFunction("DoWalkTo", &doWalkToPosition)
		.addFunction("DoStopAttack", &doStopAutoWalk)
		.addFunction("DoTalk", &talk)
		.addFunction("DoUseInventoryItem", &useInventoryItem)
		.addFunction("DoUseInventoryItemOnCreature", &useOnCreature)
		.addFunction("DoUseInventoryItemOnItem", &useOnItem)
		.addFunction("DoUseInventorySlotItem", &useSlotItem)
		.addFunction("DoUseThing", &UseThing)
		.addFunction("DoAttack", &doAttack)
		.addFunction("GetMyPokemon", &getMySummon)
		.addFunction("GetBattleList", &GetBList)
		.addFunction("GetSlotItem", &getSlotItem)
		.addFunction("DoSendKey", &SendKeyStroke)
		.endModule();

	InitImgui(L);
}

std::string Engine::GetStartupPath(std::string append)
{
	if (!g_Path.empty()) {
		std::string x = g_Path;
		x.append("\\");
		x.append(append);
		return x;
	}
	return "";
}
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}
void Engine::LoadScript(std::string file)
{
	using namespace LuaIntf;
	std::string x = GetStartupPath("LuaPlusPlus\\Scripts\\");
	if (!x.empty()) {
		x.append(file);
		if (file.find(".lua") == std::string::npos)
		{
			x.append(".lua");
			file.append(".lua");
		}
		x.erase(std::remove(x.begin(), x.end(), '\n'), x.end());

		Script* luaptr = new Script();
		Instances.push_back(luaptr);

		Init(Instances.back()->State);
		std::string scriptName = file;
		if (file.find_last_of(".") != std::string::npos)
		{
			scriptName = file.substr(0, file.find_last_of("."));
		}

		std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::tolower);
		Lua::setGlobal(Instances.back()->State, "SCRIPT_NAME", scriptName);
		Lua::setGlobal(Instances.back()->State, "FILE_NAME", file.c_str());
		for (fs::directory_iterator i = fs::directory_iterator(fs::path(Engine::GetStartupPath("/LuaPlusPlus/Scripts/Common"))); i != fs::directory_iterator(); i++)
		{
			if (!fs::is_directory(i->path()) && i->path().filename().string().find_last_of(".lua") != std::string::npos)
			{
				std::string str = i->path().filename().string();
				//ScriptsMenu->CheckBox(str.substr(0, str.find_last_of(".lua") - 3).c_str(), false);
				std::string k = GetStartupPath("LuaPlusPlus/Scripts/Common/");
				k.append(str);
				Instances.back()->ExecuteFile(k.c_str());
			}
		}
		Instances.back()->ExecuteFile(x.c_str());
		LuaRef func = Lua::getGlobal(Instances.back()->State, "onStart");
		if (func.isFunction())
		{
			func();
		}
	}
}