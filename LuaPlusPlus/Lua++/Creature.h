//Creature BattleList Data Struct / Functions
#ifndef CREATURE_H
#define CREATURE_H
#include "declarations.h"
class CreaturePtr
{
	//Data Functions
public:
	//is local player
	bool isDiff(CreaturePtr* p);


	bool isValid();

	//Get Creature Position Vector
	Position GetPosition();

	//Get Creature Name
	char* GetName();

	std::string getNameString();

	//Get Health Bar Percent
	int getHealthPercent();

	//Get Look Direction
	int GetLookDirection();

	//Set Creature Look Direction
	void SetLookDirection(int dir);

	//Get Creature Id
	int GetId();

	void SetId(int id);

	//Get Creature Outfit Struct
	Outfit getOutfit() {
		return Outfit(this->m_outfit_addon, this->m_outfit_looktype, this->m_outfit_head, this->m_outfit_body, this->m_outfit_legs, this->m_outfit_feet);
	}

	//Set Creature Outfit Struct
	void setOutfit(Outfit outfit) {
		this->m_outfit_looktype = outfit.LookType; this->m_outfit_head = outfit.Head; this->m_outfit_body = outfit.Body; this->m_outfit_legs = outfit.Legs; this->m_outfit_feet = outfit.Feet;
	}

	//Get Creature Light
	int GetLight();

	//Set Creature Light
	void SetLight(int light);

	//Get CreatureSpeed
	int GetSpeed();

	//Set Creature Speed
	void SetSpeed(int speed);


	bool isSummon();

	bool isPlayer();

	bool isMonsterOrNpc();

	bool isWalking();

private:
	char pad_0000[12]; //0x0000
	uint16_t m_pos_x; //0x000C
	char pad_000E[2]; //0x000E
	uint16_t m_pos_y; //0x0010
	char pad_0012[2]; //0x0012
	uint8_t m_pos_z; //0x0014
	char pad_0015[7]; //0x0015
	uint32_t m_id; //0x001C
	char pad_0020[4]; //0x0020
	uint8_t m_name_lenght; //0x0024
	char pad_0025[3]; //0x0025
	char m_name[15]; //0x0028
	char pad_0037[1]; //0x0037
	uint8_t m_healthPercent; //0x0038
	char pad_0039[3]; //0x0039
	uint8_t m_direction; //0x003C
	char pad_003D[3]; //0x003D
	uint8_t m_outfit_visible; //0x0040
	char pad_0041[3]; //0x0041
	uint8_t m_outfit_looktype; //0x0044
	char pad_0045[7]; //0x0045
	uint8_t m_outfit_head; //0x004C
	char pad_004D[3]; //0x004D
	uint8_t m_outfit_body; //0x0050
	char pad_0051[3]; //0x0051
	uint8_t m_outfit_legs; //0x0054
	char pad_0055[3]; //0x0055
	uint8_t m_outfit_feet; //0x0058
	char pad_0059[3]; //0x0059
	uint8_t m_outfit_addon; //0x005C
	char pad_005D[83]; //0x005D
	uint16_t m_light; //0x00B0
	char pad_00B2[2]; //0x00B2
	int32_t m_speed; //0x00B4
	uint8_t m_skull; //0x00B8
	uint8_t m_shield; //0x00B9
	uint8_t m_emblem; //0x00BA
	uint8_t m_type; //0x00BB
	uint8_t m_icon; //0x00BC
	char pad_00BD[31]; //0x00BD
	uint8_t m_flag1; //0x00DC
	uint8_t m_flag2; //0x00DD
	char pad_00DE[1]; //0x00DE
	uint8_t m_flag3; //0x00DF
	char pad_00E0[360]; //0x00E0
	bool m_walking; //0x0248
}; //Size: 0x0249

#endif
