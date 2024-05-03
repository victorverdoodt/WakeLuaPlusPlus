#ifndef GAME_H
#define GAME_H

#include "Item.h"
#include "Creature.h"
#include "Protocol.h"
#include "LocalPlayer.h"
#include "Outfit.h"


class Game
{
public:
	LocalPLayerPtr* GetLocalPlayer() {
		return m_localPlayer;
	}

	ProtocolGamePtr* GetGameProtocol() {
		return m_protocolGame;
	}

	CreaturePtr* GetAttackTarget() {
		return m_attackingCreature;
	}

	CreaturePtr* GetFollowTarget() {
		return m_followingCreature;
	}

	void SetAttackTarget(CreaturePtr * creature) {
		m_attackingCreature = creature;
		m_attackingCreature->SetId(creature->GetId());
	}

	char* GetCharacterName() {
		if (m_characterName_lenght > 16) {
			return ReadMemory<char*>((DWORD)m_characterName);
		}
		return m_characterName;
	}

	char* GetWorldName() {
		if (m_worldName_lenght > 15) {
			return ReadMemory<char*>((DWORD)m_worldName);
		}
		return m_worldName;
	}

	bool isOnline() {
		return m_online;
	}

	bool isDead() {
		return m_dead;
	}

private:
	class LocalPLayerPtr* m_localPlayer; //0x0000
	class CreaturePtr* m_attackingCreature; //0x0004
	class CreaturePtr* m_followingCreature; //0x0008
	class ProtocolGamePtr* m_protocolGame; //0x000C
	char pad_0010[48]; //0x0010
	bool m_online; //0x0040
	char pad_0041[1]; //0x0041
	bool m_dead; //0x0042
	char pad_0043[57]; //0x0043
	uint8_t m_pvpMode; //0x007C
	char pad_007D[7]; //0x007D
	uint8_t m_lastWalkDir; //0x0084
	char pad_0085[23]; //0x0085
	uint8_t m_characterName_lenght; //0x009C
	char pad_009D[3]; //0x009D
	char m_characterName[16]; //0x00A0
	char pad_00B0[4]; //0x00B0
	uint8_t m_worldName_lenght; //0x00B4
	char pad_00B5[3]; //0x00B5
	char m_worldName[16]; //0x00B8
}; //Size: 0x00C8

extern Game g_game;

#endif