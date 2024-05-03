#include "declarations.h"
#include "Item.h"
int ItemPtr::getServerId() {
	return this->m_serverId;
}

int ItemPtr::getClientId() {
	return this->m_clientId;
}

Position ItemPtr::getPosition() {
	return  Position(this->m_position_x, this->m_slotid, this->m_position_z);
}

int ItemPtr::getSlotItem() {
	return this->m_slotid;
}
void ItemPtr::useItem()
{
	//Functions.sendUseItem(Engine::getGameProtocol(), this->getPosition(), this->getServerId(), this->getPosition().z, 1);
}