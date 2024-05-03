#include "LuaMain.h"
#include "console.h"
#include <experimental\filesystem>
#include "Offsets.h"
#include <curl/curl.h>
#include <wtypes.h>


#pragma comment(lib, "static-release-x86/libcurl_a.lib")
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

namespace fs = std::experimental::filesystem;
using namespace LuaIntf;
static bool show_app_console = true;
static bool g_started = false;
static bool g_menu = false;

CFunctions Functions;


DWORD getHDID() {
	DWORD lVolSerialNbr = 0;

	GetVolumeInformation(L"C:\\", 0, 0, &lVolSerialNbr, 0, 0, 0, 0);

	return lVolSerialNbr;
}

bool AuthHDID() {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/vyctor17/rawdb/master/att.txt");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		std::size_t found = readBuffer.find(std::to_string(getHDID()));
		if (found != std::string::npos)
			return true;
	}
	return false;
}

void PopulateBattleList() {
	auto count = ReadMemory<int>((baseAddr + battleList) + Count);
	Engine::CreatureCache.clear();
	//std::cout << "Creatures Found:" << count << "\n";
	if (count > 0) {
		auto bufferPointer = ReadMemory<DWORD>((baseAddr + battleList) + BufferPointer);
		auto currentNodeAddress = bufferPointer + NodePointer;

		int i = 1;
		bool stop = false;
		bool findsummon = false;
		while (!stop) {
			auto nodePointer = ReadMemory<DWORD>(currentNodeAddress);
			if (nodePointer) {
				auto creaturePointer = ReadMemory <CreaturePtr*>(nodePointer + Value);
				if (creaturePointer) {
					if (creaturePointer->isDiff((CreaturePtr*)Engine::getLocalPlayer())) {
						auto pos = creaturePointer->GetPosition();
						if (creaturePointer->isValid()) {
							Engine::CreatureCache.push_back(creaturePointer);
							if (creaturePointer->isSummon()) {
								//std::cout << "Summon? :" << creaturePointer->GetName() << "\n";
								findsummon = true;
								Engine::mySummon = creaturePointer;
							}
							//std::cout << "Pointer: " << creaturePointer << ", Health: " << creaturePointer->getHealthPercent() << ", Name:" << creaturePointer->GetName() << ", Pos:" << creaturePointer->GetPosition() << "\n";
						}
					}
				}
			}

			i++;
			if (i <= count) {
				currentNodeAddress = nodePointer + NextPointer;
			}
			else {
				if (!findsummon) {
					Engine::mySummon = nullptr;
				}
				stop = true;
			}
		}

	}
}

DWORD_PTR* FindDevice(DWORD Base, DWORD Len)
{
	unsigned long i = 0, n = 0;

	for (i = 0; i < Len; i++)
	{
		if (*(BYTE *)(Base + i + 0x00) == 0xC7)n++;
		if (*(BYTE *)(Base + i + 0x01) == 0x06)n++;
		if (*(BYTE *)(Base + i + 0x06) == 0x89)n++;
		if (*(BYTE *)(Base + i + 0x07) == 0x86)n++;
		if (*(BYTE *)(Base + i + 0x0C) == 0x89)n++;
		if (*(BYTE *)(Base + i + 0x0D) == 0x86)n++;

		if (n == 6)
			return (DWORD_PTR*)(Base + i + 2); n = 0;
	}
	return(0);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static WNDPROC def_wndproc = nullptr;

class splitstring : public std::string {
	std::vector<std::string> flds;
public:
	splitstring(std::string s) : std::string(s) { };
	std::vector<std::string>& split(char delim, int rep = 0);
};

std::vector<std::string>& splitstring::split(char delim, int rep)
{
	if (!flds.empty()) flds.clear();
	std::string work = data();
	std::string buf = "";
	int i = 0;
	while (i < work.length()) {
		if (work[i] != delim)
			buf += work[i];
		else if (rep == 1) {
			flds.push_back(buf);
			buf = "";
		}
		else if (buf.length() > 0) {
			flds.push_back(buf);
			buf = "";
		}
		i++;
	}
	if (!buf.empty())
		flds.push_back(buf);
	return flds;
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

void sendMessages(wchar_t* teste) {
	//var lParam = new COPYDATASTRUCT { cbData = (int)channel, dwData = data.Length * 2 + 2, lpData = data };
	HWND wndsend = 0;
	wndsend = FindWindowA(0, "Receiver");
	if (wndsend == 0)
	{
		TextConsole::GetInstance().AddLog("Couldn't find window.");
	}
	else {
		LPCWSTR strDataToSend;
		strDataToSend = teste;
		COPYDATASTRUCT cpd;
		cpd.cbData = (wcslen(strDataToSend) + 1) * 2;
		cpd.lpData = (PVOID)strDataToSend;
		::SendMessage(wndsend, 74U, (WPARAM)::GetDesktopWindow(), (LPARAM)&cpd);
	}
}
bool auth = false;
void ProcessMessage(std::string msg) {

	splitstring s(msg);
	std::vector<std::string> cmd = s.split('|');
	if (cmd.size() > 0) {
		if (cmd[0] == "LOGIN") {
			TextConsole::GetInstance().AddLog(cmd[1] + " " + cmd[2]);
		}
		else if (cmd[0] == "SETPATH") {
			Engine::g_Path = cmd[1];
			sendMessages(L"SETPATH");
			TextConsole::GetInstance().AddLog("Path set to " + cmd[1]);
		}
		else if (cmd[0] == "LOAD") {
			if (!auth) {
				auth = AuthHDID();
			}
			if (auth) {
				Engine::LoadScript(cmd[1]);
				TextConsole::GetInstance().AddLog("Loaded " + cmd[1]);
			}
			else {
				TextConsole::GetInstance().AddLog("Auth Fail Id: " + std::to_string(getHDID()));
			}
		}
		else if (cmd[0] == "UNLOAD") {
			std::string scriptNameN = cmd[1];
			if (cmd[1].find_last_of(".") != std::string::npos)
			{
				scriptNameN = cmd[1].substr(0, cmd[1].find_last_of("."));
			}
			for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
			{
				std::string scriptName = Lua::getGlobal<std::string>((*it)->State, "SCRIPT_NAME");
				std::transform(scriptNameN.begin(), scriptNameN.end(), scriptNameN.begin(), ::tolower);
				if (scriptName == scriptNameN) {
					Engine::Instances.erase(it);
					TextConsole::GetInstance().AddLog("Unloaded " + cmd[1]);
				}
			}
		}
	}
}




LRESULT WINAPI hkWndProc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param) {

	if (message_type == 74U) {
		COPYDATASTRUCT* cds = (COPYDATASTRUCT*)l_param;
		int len = (cds->cbData / 2) - 1;

		std::string msg = string_format("%.*S\n", len, cds->lpData);
		ProcessMessage(msg);
		/* Also fixed the parameter list for "%.*s" */
		
	}

	if (g_menu) {
		ImGui_ImplWin32_WndProcHandler(FindWindow(NULL, L"pxgclient_dx9"), message_type, w_param, l_param);
		//return 1;
	}

	return CallWindowProc(def_wndproc, window, message_type, w_param, l_param);
};

void CustomImGUIStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(14, 14);
	style.FramePadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(8, 8);
	style.ItemInnerSpacing = ImVec2(8, 8);

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.98f, 0.39f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.88f, 0.41f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.47f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.98f, 0.61f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
}

void LSMenu()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (g_menu && g_started)
	{

		ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Wake Engine", &g_menu, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("DummyTabBar", ImGuiTabBarFlags_None))
			{
				
				for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
				{
					LuaRef func = Lua::getGlobal((*it)->State, "onMenu");
					if (func.isFunction())
					{
						func();
					}
				}
				TextConsole::GetInstance().Run("Console", &show_app_console);
				ImGui::EndTabBar();
			}
		}
		//if (ImGui::IsItemHovered() || (ImGui::IsAnyItemFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
			//ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
		ImGui::End();
		// Rendering
	}
	ImGui::EndFrame();


	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

static void ImGuiEx(HWND Chwnd, IDirect3DDevice9 * CDevice)
{
	if (GetAsyncKeyState(VK_INSERT))
	{
		g_menu = !g_menu;
	}

	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "onUpdate");
		if (func.isFunction())
		{
			func();
		}
	}
	
	if (!g_started) {
		Engine::GamePtr = ReadMemory<Game>(baseAddr + gGame);
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		CustomImGUIStyle();
		//ImGui::StyleColorsClassic();
		def_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Chwnd, GWLP_WNDPROC, LONG_PTR(hkWndProc)));
		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(Chwnd);
		ImGui_ImplDX9_Init(CDevice);

	

	}

	LSMenu();

	if (!g_started) {
		/*if (!(fs::exists(Engine::GetStartupPath("\\LuaPlusPlus"))) || !(fs::exists(Engine::GetStartupPath("\\LuaPlusPlus\\Scripts"))) || !(fs::exists(Engine::GetStartupPath("\\LuaPlusPlus\\Scripts\\Common"))))
		{
			fs::create_directory(Engine::GetStartupPath("\\LuaPlusPlus"));
			fs::create_directory(Engine::GetStartupPath("\\LuaPlusPlus\\Scripts"));
			fs::create_directory(Engine::GetStartupPath("\\LuaPlusPlus\\Scripts\\Common"));
			TextConsole::GetInstance().AddLog("[Lua++] Created Directories");
		}
		for (fs::directory_iterator i = fs::directory_iterator(fs::path(Engine::GetStartupPath("\\LuaPlusPlus\\Scripts"))); i != fs::directory_iterator(); i++)
		{
			if (!fs::is_directory(i->path()) && i->path().filename().string().find_last_of(".lua") != std::string::npos)
			{
				std::string str = i->path().filename().string();
				//ScriptsMenu->CheckBox(str.substr(0, str.find_last_of(".lua") - 3).c_str(), false);
				Engine::Scripts.push_back(str.substr(0, str.find_last_of(".lua") - 3));
			}
		}

		for (std::string str : Engine::Scripts)
		{
			Engine::LoadScript(str);
		}*/
		g_started = true;
		TextConsole::GetInstance().AddLog("Lua Engine succesfully load!");
	}

	if (Engine::GamePtr.isOnline() && !Engine::GamePtr.isDead()) {
		//std::cout << "LocalPlayerPointer:" << GamePtr.GetLocalPlayer() << ", Name:" << GamePtr.GetLocalPlayer()->GetName() << ", game: " << "\n";
		PopulateBattleList();
	}
}

void MainLoop() {
	//Engine::GamePtr = ReadMemory<Game>(baseAddr + gGame);
}

typedef HRESULT(WINAPI *oEndScene)(LPDIRECT3DDEVICE9);
oEndScene Original_EndScene;
HRESULT WINAPI Hooked_EndScene(LPDIRECT3DDEVICE9 Device) {
	//MainLoop();
	HWND hwnd = FindWindow(NULL, L"PXG Client");
	MainLoop();
	ImGuiEx(hwnd, Device);

	return Original_EndScene(Device);
}

int D3Dinit(void)
{

	HMODULE hD3D = NULL;


	do
	{
		hD3D = GetModuleHandleA("d3d9.dll");
		Sleep(10);
	} while (!hD3D);


	DWORD_PTR * VTablePtr = FindDevice((DWORD)hD3D, 0x128000);


	if (VTablePtr == NULL)
	{
		MessageBox(NULL, L"D3DDevice Pointer Not Found!", 0, MB_ICONSTOP);
		ExitProcess(0);
	}

	DWORD_PTR * VTable = 0;
	*(DWORD_PTR *)&VTable = *(DWORD_PTR *)VTablePtr;
	Original_EndScene = (oEndScene)DetourFunction((PBYTE)VTable[42], (PBYTE)Hooked_EndScene);

	return 0;
}

typedef void(__thiscall* _cancelAttack)(void*, DWORD*);
_cancelAttack cancelAttack;
void __fastcall hk_cancelAttack(void* ecx, void* edx, DWORD* inputMessage)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "onLostTarget");
		if (func.isFunction())
		{
			func();
		}
	}
	//WriteMemory<CreaturePtr*>(baseAddr + gGame + 0x4, nullptr);
	return cancelAttack(ecx, inputMessage);
}

typedef void(__thiscall* _onUseItem)(void*, const Position&, int, int, int);
_onUseItem onUseItem;
//E1F30
//(const Position& position, int itemId, int stackpos, int index)
void __fastcall hk_onUseItem(void* ecx, void* edx, const Position& position, int itemId, int stackpos, int index)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "onUseItem");
		if (func.isFunction())
		{
			func(position, itemId, stackpos, index);
		}
	}

	TextConsole::GetInstance().AddLog("%i", position.y);

	return onUseItem(ecx, position, itemId, stackpos, index);
}

//(int a1, int a2, _DWORD *a3)
typedef void(__thiscall* _onUseItem2)(void*,  const DWORD*);
_onUseItem2 onUseItem2;
void __fastcall hk_onUseItem2(void* ecx, void* edx, const DWORD* a3)
{
	TextConsole::GetInstance().AddLog("useItem");
	return onUseItem2(ecx, a3);
}

typedef void(__thiscall* _onUseItemWith)(void*, const Position&, int, int, const Position&, int, int);
_onUseItemWith onUseItemWith;

void __fastcall hk_onUseItemWith(void* ecx, void* edx, const Position& fromPos, int itemId, int fromStackPos, const Position& toPos, int toThingId, int toStackPos)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "onUseItemWith");
		if (func.isFunction())
		{
			func(fromPos, itemId, fromStackPos, toPos, toThingId, toStackPos);
		}
	}
	return onUseItemWith(ecx, fromPos, itemId, fromStackPos, toPos, toThingId, toStackPos);
}

typedef void(__thiscall* _onTalk)(void*, int, int, const std::string&, const std::string&);
_onTalk onTalk;
void __fastcall hk_onTalk(void* ecx, void* edx, int mode, int channelId, const std::string& receiver, const std::string& message)
{
	for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
	{
		LuaRef func = Lua::getGlobal((*it)->State, "onTalk");
		if (func.isFunction())
		{
			TextConsole::GetInstance().Save("%s", message);

			func(mode, channelId, TextConsole::GetInstance().getSave());
		}
	}
	TextConsole::GetInstance().AddLog("ProtocolTalk: %p - %s", ecx, message);

	return onTalk(ecx, mode, channelId, receiver, message);
}


typedef void(__thiscall* _onGTalk)(void*, Otc::MessageMode, int, const std::string&);
_onGTalk onGTalk;
void __fastcall hk_onGTalk(void* ecx, void* edx, Otc::MessageMode mode, int channelId, const std::string& message)
{
	TextConsole::GetInstance().AddLog("GameTalk: %p - %s", ecx, message);

	return onGTalk(ecx, mode, channelId, message);
}

//void __fastcall sub_4F32A0(_DWORD *a1, int a2, int *a3, int a4, int a5) --Addthing
typedef void(__thiscall* _onAddThing)(void*, int, const Position&, int);
_onAddThing onAddThing;
void __fastcall hk_onAddThing(void* ecx, void* edx, int a1, const Position& pos, int stackpos)
{
	
	//if (Functions.GetTile((DWORD*)ecx, pos) > 0) {
		TextConsole::GetInstance().AddLog("ECX = %p, Thing=%i, (X=%i, Y=%i, Z=%i, StackPos=%i)", ecx, a1, pos.x, pos.y, pos.z,stackpos);
	//}
	//TextConsole::GetInstance().AddLog("AddThing = %p, %i", ecx, stackpos);
	return onAddThing(ecx, a1, pos, stackpos);
}


typedef int(__thiscall* _onGetTile)(void* , const Position&);
_onGetTile onGetTile;
int __fastcall hk_onGetTile(void* ecx, void* edx, const Position& pos)
{

	//if (Functions.GetTile((DWORD*)ecx, pos) > 0) {
	//TextConsole::GetInstance().AddLog("pointer= %p  (X=%i, Y=%i, Z=%i)", ecx, pos.x, pos.y, pos.z);
	//}
	//TextConsole::GetInstance().AddLog("AddThing = %p, %i", ecx, stackpos);
	return onGetTile(ecx, pos);
}

void __stdcall Start() {

	Functions.stopAutoWalk = (Typedefs::stopAutoWalk)((DWORD)GetModuleHandle(NULL) + 0xEBD20);
	Functions.autoWalk = (Typedefs::autoWalk)((DWORD)GetModuleHandle(NULL) + 0xEC180);
	Functions.sendAttack = (Typedefs::sendAttack)((DWORD)GetModuleHandle(NULL) + 0x13D490);
	Functions.sendUseItem = (Typedefs::sendUseItem)((DWORD)GetModuleHandle(NULL) + cOnUseItem);
	Functions.sendUseOnCreature = (Typedefs::sendUseOnCreature)((DWORD)GetModuleHandle(NULL) + cOnUseItemOnCreature);
	Functions.sendUseItemWith = (Typedefs::sendUseItemWith)((DWORD)GetModuleHandle(NULL) + cOnUseItemWith);


	//void __fastcall sub_4F32A0(_DWORD *a1, int a2, int *a3, int a4, int a5) --Addthing

	Functions.GetTile = (Typedefs::GetTile)((DWORD)GetModuleHandle(NULL) + 0xF0030);
	Functions.getItems = (Typedefs::getItems)((DWORD)GetModuleHandle(NULL) + 0x117670);
	Functions.getTopThing = (Typedefs::getTopThing)((DWORD)GetModuleHandle(NULL) + 0x114E40);
	Functions.findItemTypeByClientId = (Typedefs::findItemTypeByClientId)((DWORD)GetModuleHandle(NULL) + 0x1096B0);

	//Functions.addThing = (Typedefs::addThing)((DWORD)GetModuleHandle(NULL) + 0xF32A0);

	

	Functions.isContainer = (Typedefs::isContainer)((DWORD)GetModuleHandle(NULL) + 0x1FFE90);
	Functions.getStackPos = (Typedefs::getStackPos)((DWORD)GetModuleHandle(NULL) + 0x108A20);

	Functions.sendTalk = (Typedefs::sendTalk)((DWORD)GetModuleHandle(NULL) + cOnTalk);
	Functions.Talk = (Typedefs::Talk)((DWORD)GetModuleHandle(NULL) + 0xDD540);
	onGTalk = (_onGTalk)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + 0xDD450), (PBYTE)hk_onGTalk);

	cancelAttack = (_cancelAttack)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + cCancelAttack), (PBYTE)hk_cancelAttack);
	

	onGetTile = (_onGetTile)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + 0xF0030), (PBYTE)hk_onGetTile);
	onUseItem = (_onUseItem)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + cOnUseItem), (PBYTE)hk_onUseItem);
	onUseItemWith = (_onUseItemWith)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + cOnUseItemWith), (PBYTE)hk_onUseItemWith);
	onTalk = (_onTalk)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + cOnTalk), (PBYTE)hk_onTalk);
	onAddThing = (_onAddThing)DetourFunction((PBYTE)((DWORD)GetModuleHandle(NULL) + cOnAddThing), (PBYTE)hk_onAddThing);

	D3Dinit();

	TextConsole::GetInstance().RegisterCommand("load", [](std::vector<std::string> cmd) {
		if (!cmd.empty() && !cmd[1].empty()) {
			auto pp = Engine::GetStartupPath("/LuaPlusPlus/Scripts");
			if (!pp.empty()) {
				Engine::LoadScript(pp + cmd[1]);
				TextConsole::GetInstance().AddLog("Loaded " + cmd[1]);
			}
		}
	});

	TextConsole::GetInstance().RegisterCommand("unload", [](std::vector<std::string> cmd) {
		if (!cmd.empty() && !cmd[1].empty()) {
			for (std::vector<Script*>::iterator it = Engine::Instances.begin(); it != Engine::Instances.end(); ++it)
			{
				std::string scriptName = Lua::getGlobal<std::string>((*it)->State, "SCRIPT_NAME");
				if (scriptName == cmd[1]) {
					Engine::Instances.erase(it);
				}
			}
			TextConsole::GetInstance().AddLog("Unloaded " + cmd[1]);
		}
	});

	TextConsole::GetInstance().RegisterCommand("loadall", [](std::vector<std::string> cmd) {
		for (std::string str : Engine::Scripts)
		{
			Engine::LoadScript(str);
		}
		TextConsole::GetInstance().AddLog("Loaded " + cmd[1]);
	});

	TextConsole::GetInstance().RegisterCommand("unloadall", [](std::vector<std::string> cmd) {
		if (!cmd.empty() && !cmd[1].empty()) {
			Engine::Instances.clear();
			TextConsole::GetInstance().AddLog("Unloaded " + cmd[1]);
		}
		TextConsole::GetInstance().AddLog("Unloaded all scripts");
	});
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//AllocConsole();
		//freopen("CONIN$", "r", stdin);
		//freopen("CONOUT$", "w", stdout);

		std::cout << "[+] Successfully attached to process.\n";

		//CreateThread(0, 0, MainThread, hModule, 0, 0);
		std::cout << "Autenticado com sucesso!. \n";
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0);

		return TRUE;
		// Alternatively just do easy things here if you dont need an own thread,
		// like patching out things etc...

		break;
	case DLL_THREAD_ATTACH:
		return TRUE;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}