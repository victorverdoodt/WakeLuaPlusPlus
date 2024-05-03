#include "LocalPlayer.h"
#include "Game.h"
//is local player
	//is local player
bool LocalPLayerPtr::isDiff(LocalPLayerPtr* p) {
	if (this != p)
		return true;

	return false;
}


bool LocalPLayerPtr::isValid() {
	if (this->m_pos_x < 65535 && this->m_pos_y > 0)
		return true;

	return false;
}

//Get Creature Position Vector
Position LocalPLayerPtr::GetPosition() {
	return Position(this->m_pos_x, this->m_pos_y, this->m_pos_z);
}

bool LocalPLayerPtr::hasSight(const Position& pos)
{
	return GetPosition().isInRange(pos, 9 - 1, 9 - 1);
}

//Get Creature Name
char* LocalPLayerPtr::GetName() {
	if (this->m_name_lenght > 16) {
		return ReadMemory<char*>((DWORD)this->m_name);
	}
	return this->m_name;
}

std::string LocalPLayerPtr::getNameString() {
	std::string someString(this->GetName());
	return someString;
}

//Get Health Bar Percent
int LocalPLayerPtr::getHealthPercent() {
	return this->m_healthPercent;
}

//Get Look Direction
int LocalPLayerPtr::GetLookDirection() {
	return this->m_direction;
}

//Set Creature Look Direction
void LocalPLayerPtr::SetLookDirection(int dir) {
	this->m_direction = dir;
}

//Get Creature Id
int LocalPLayerPtr::GetId() {
	return this->m_id;
}

//Get Creature Light
int LocalPLayerPtr::GetLight() {
	return this->m_light;
}

//Set Creature Light
void LocalPLayerPtr::SetLight(int light) {
	this->m_light = light;
}

//Get CreatureSpeed
int LocalPLayerPtr::GetSpeed() {
	return this->m_speed;
}

//Set Creature Speed
void LocalPLayerPtr::SetSpeed(int speed) {
	this->m_speed = speed;
}

Position LocalPLayerPtr::GetWalkDestination() {
	return Position(this->m_autoWalkDestination_x, this->m_autoWalkDestination_y, this->m_autoWalkDestination_z);
}

void LocalPLayerPtr::SetWalkDestination(Position& DestPos) {
	this->m_autoWalkDestination_x = DestPos.x;
	this->m_autoWalkDestination_y = DestPos.y;
	this->m_autoWalkDestination_z = DestPos.z;
}

bool LocalPLayerPtr::isAutoWalking() {
	if (this->m_autoWalkDestination_x >= 65535 && this->m_autoWalkDestination_y >= 65535 && this->m_autoWalkDestination_z >= 255)
		return false;

	return true;
}

bool LocalPLayerPtr::isWalking() {
	return this->m_walking;
}

int LocalPLayerPtr::getHealth() {
	return std::round(this->m_health);
}

int LocalPLayerPtr::getMaxHealth() {
	return std::round(this->m_maxHealth);
}

int LocalPLayerPtr::getMana() {
	return std::round(this->m_mana);
}

int LocalPLayerPtr::getMaxMana() {
	return std::round(this->m_maxMana);
}

int LocalPLayerPtr::getLevel() {
	return std::round(this->m_level);
}

void LocalPLayerPtr::setLookType(int type) {
	m_outfit_looktype = type;
}

ItemPtr* LocalPLayerPtr::getSlotItem(int id) {
	return this->m_inventoryItems[id];
}