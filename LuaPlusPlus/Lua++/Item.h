#ifndef ITEM_H
#define ITEM_H

class ItemPtr
{
public:
	int getServerId();
	int getClientId();
	Position getPosition();
	int getSlotItem();
	void useItem();

private:
	char pad_0000[12]; //0x0000
	uint16_t m_position_x; //0x000C
	char pad_000E[2]; //0x000E
	uint16_t m_slotid; //0x0010
	char pad_0012[2]; //0x0012
	uint8_t m_position_z; //0x0014
	char pad_0015[5]; //0x0015
	uint16_t m_clientId; //0x001A
	char pad_001C[10]; //0x001C
	uint16_t m_clientId2; //0x0026
	char pad_0028[2]; //0x0028
	uint16_t m_serverId; //0x002A
	char pad_002C[2]; //0x002C
	uint8_t m_countOrSubType; //0x002E
}; //Size: 0x002F

#endif
