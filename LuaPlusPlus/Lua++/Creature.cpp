#include "Creature.h"
#include "LocalPlayer.h"
#include "Item.h"
#include "Game.h"
bool CreaturePtr::isDiff(CreaturePtr* p) {
	if (this != p)
		return true;

	return false;
}


bool CreaturePtr::isValid() {
	if (this->m_pos_x < 65535 && this->m_pos_y > 0)
		return true;

	return false;
}

//Get Creature Position Vector
Position CreaturePtr::GetPosition() {
	return Position(this->m_pos_x, this->m_pos_y, this->m_pos_z);
}

//Get Creature Name
char* CreaturePtr::GetName() {
	if (this->m_name_lenght > 15) {
		return ReadMemory<char*>((DWORD)this + 0x28);
	}
	return this->m_name;
}

std::string CreaturePtr::getNameString() {
	std::string someString(this->GetName());
	return someString;
}

//Get Health Bar Percent
int CreaturePtr::getHealthPercent() {
	return this->m_healthPercent;
}

//Get Look Direction
int CreaturePtr::GetLookDirection() {
	return this->m_direction;
}

//Set Creature Look Direction
void CreaturePtr::SetLookDirection(int dir) {
	this->m_direction = dir;
}

//Get Creature Id
int CreaturePtr::GetId() {
	return this->m_id;
}

void CreaturePtr::SetId(int id) {
	this->m_id = id;
}

//Get Creature Light
int CreaturePtr::GetLight() {
	return this->m_light;
}

//Set Creature Light
void CreaturePtr::SetLight(int light) {
	this->m_light = light;
}

//Get CreatureSpeed
int CreaturePtr::GetSpeed() {
	return this->m_speed;
}

//Set Creature Speed
void CreaturePtr::SetSpeed(int speed) {
	this->m_speed = speed;
}


bool CreaturePtr::isSummon() {
	if (m_flag1 == 0 && m_flag2 == 1 && m_flag3 == 1)
		return true;
	return false;
}

bool CreaturePtr::isPlayer() {
	if (m_flag1 == 1 && m_flag2 == 1 && m_flag3 == 1)
		return true;
	return false;
}

bool CreaturePtr::isMonsterOrNpc() {
	if (m_flag1 == 0 && m_flag2 == 1 && m_flag3 == 0)
		return true;
	return false;
}

bool CreaturePtr::isWalking() {
	return this->m_walking;
}