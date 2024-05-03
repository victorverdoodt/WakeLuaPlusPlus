#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H
//Creature BattleList Data Struct / Functions
#include "Creature.h"
class LocalPLayerPtr
{
	//Data Functions
public:
	//is local player
	bool isDiff(LocalPLayerPtr* p);

	bool isValid();

	//Get Creature Position Vector
	Position GetPosition();

	bool hasSight(const Position& pos);

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

	Position GetWalkDestination();

	void SetWalkDestination(Position& DestPos);

	bool isAutoWalking();

	bool isWalking();

	int getHealth();

	int getMaxHealth();

	int getMana();

	int getMaxMana();

	int getLevel();

	void setLookType(int type);

	ItemPtr* getSlotItem(int id);


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
	char pad_00BD[395]; //0x00BD
	bool m_walking; //0x0248
	char pad_0249[31]; //0x0249
	uint16_t m_lastPrewalkDestination_x; //0x0268
	char pad_026A[2]; //0x026A
	uint16_t m_lastPrewalkDestination_y; //0x026C
	char pad_026E[2]; //0x026E
	uint8_t m_lastPrewalkDestination_z; //0x0270
	char pad_0271[3]; //0x0271
	uint16_t m_autoWalkDestination_x; //0x0274
	char pad_0276[2]; //0x0276
	uint16_t m_autoWalkDestination_y; //0x0278
	char pad_027A[2]; //0x027A
	uint8_t m_autoWalkDestination_z; //0x027C
	char pad_027D[3]; //0x027D
	uint16_t m_lastAutoWalkPosition_x; //0x0280
	char pad_0282[2]; //0x0282
	uint16_t m_lastAutoWalkPosition_y; //0x0284
	char pad_0286[2]; //0x0286
	uint8_t m_lastAutoWalkPosition_z; //0x0288
	char pad_0289[103]; //0x0289
	bool m_preWalking; //0x02F0
	bool m_lastPrewalkDone; //0x02F1
	bool m_secondPreWalk; //0x02F2
	bool m_serverWalking; //0x02F3
	bool m_knownCompletePath; //0x02F4
	bool m_premium; //0x02F5
	bool m_Known; //0x02F6
	bool m_pending; //0x02F7
	char pad_02F8[4]; //0x02F8
	class ItemPtr* m_inventoryItems[10]; //0x02FC
	char pad_0324[136]; //0x0324
	uint8_t m_vocation; //0x03AC
	char pad_03AD[3]; //0x03AD
	double m_health; //0x03B0
	double m_maxHealth; //0x03B8
	double m_freeCapacity; //0x03C0
	double m_totalCapacity; //0x03C8
	double m_experience; //0x03D0
	double m_level; //0x03D8
	double m_levelPercent; //0x03E0
	double m_mana; //0x03E8
	double m_maxMana; //0x03F0
	double m_magicLevel; //0x03F8
	double m_magicLevelPercent; //0x0400
	double m_baseMagicLevel; //0x0408
	double m_soul; //0x0410
	double m_stamina; //0x0418
	double m_regenerationTime; //0x0420
	double m_offlineTrainingTime; //0x0428
}; //Size: 0x0430

#endif