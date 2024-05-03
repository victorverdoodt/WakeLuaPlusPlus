#pragma once
#include "LuaMain.h"
class Typedefs {
public:
	typedef void(__thiscall* sendAttack)(ProtocolGamePtr *a1, int a2, int a3);
	typedef void(__thiscall* Attack)(DWORD* a1, DWORD* a2);
	typedef int(__fastcall* ForceDisconnect)(int a1);
	typedef void(__fastcall* ForceWalk)(int P, int a1, int Dir);
	typedef void(__thiscall* sendWalkEast)(ProtocolGamePtr *gProtocol);

	typedef void(__thiscall* autoWalk)(DWORD * a1, const Position& destination);

	typedef void(__thiscall* stopAutoWalk)(DWORD * a1);
	typedef std::tuple<std::vector<Otc::Direction>, Otc::PathFindResult>(__thiscall* findPath)(DWORD* a1, const Position& startPos, const Position& goalPos, int maxComplexity, int flags);
	//4DA37B  F0910 E3350 EAE50


	//int __fastcall sub_4F0030(int a1, int a2, int a3)

	typedef int(__thiscall* GetId)(CreaturePtr * a1);
	typedef int(__thiscall* GetHealthP)(int a1);

	typedef bool(__cdecl* fnIsMonster)(CreaturePtr* pObj);
	typedef int(__cdecl* fnIsNpc)(CreaturePtr* pObj);
	typedef bool(__cdecl* fnIsPlayer)(CreaturePtr* pObj);

	typedef void(__thiscall* sendUseItem)(ProtocolGamePtr *a1, const Position& position, int itemId, int stackpos, int index);
	typedef void(__thiscall* sendUseOnCreature)(ProtocolGamePtr *a1, const Position& pos, int thingId, int stackpos, uint32_t creatureId);
	typedef void(__thiscall* sendUseItemWith)(ProtocolGamePtr *a1, const Position& fromPos, int itemId, int fromStackPos, const Position& toPos, int toThingId, int toStackPos);

	typedef void(__thiscall* addThing)(DWORD* a1, int a2, int* a3, int a4, int a5);

	typedef void(__thiscall* sendTalk)(ProtocolGamePtr *a1, Otc::MessageMode mode, int channelId, const std::string& receiver, const char* message);
	typedef void(__thiscall* Talk)(void* a1, Otc::MessageMode mode, int channelId, const std::string& message);

	typedef int * (__thiscall* GetTile)(DWORD* a1, const Position& position);
	typedef int * (__thiscall* getTopThing)(int* a1, int a2);
	typedef int * (__thiscall* getItems)(DWORD* a1, int a2);

	typedef bool(__thiscall* isContainer)(int a2);

	typedef DWORD * (__thiscall* findItemTypeByClientId)(DWORD* a1, int clientId);

	typedef int(__thiscall* getStackPos)(int a2);

};

class CFunctions {
public:
	Typedefs::Attack Attack;
	Typedefs::sendAttack sendAttack;
	Typedefs::ForceDisconnect ForceDisconnect;
	Typedefs::ForceWalk ForceWalk;
	Typedefs::sendWalkEast sendWalkEast;
	Typedefs::GetId GetId;
	Typedefs::GetHealthP GetHealthPercent;
	Typedefs::autoWalk autoWalk;
	Typedefs::fnIsMonster isMonter;
	Typedefs::fnIsNpc isNpc;
	Typedefs::findPath findPath;
	Typedefs::stopAutoWalk stopAutoWalk;

	Typedefs::sendUseItem sendUseItem;
	Typedefs::sendUseOnCreature sendUseOnCreature;
	Typedefs::sendUseItemWith sendUseItemWith;

	Typedefs::sendTalk sendTalk;
	Typedefs::Talk Talk;

	Typedefs::GetTile GetTile;
	Typedefs::getItems getItems;
	Typedefs::getTopThing  getTopThing;
	Typedefs::findItemTypeByClientId findItemTypeByClientId;

	Typedefs::isContainer isContainer;
	Typedefs::getStackPos getStackPos;
	Typedefs::addThing addThing;


};

extern CFunctions Functions;