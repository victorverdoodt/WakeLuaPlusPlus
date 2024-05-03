#pragma once
class TileItem
{
public:
	int getServerId() {
		return this->m_serverId;
	}

	Position getPosition() {
		return Position(this->m_position_x, this->m_position_y, this->m_position_z);
	}

private:
	char pad_0000[12]; //0x0000
	uint16_t m_position_x; //0x000C
	char pad_000E[2]; //0x000E
	uint16_t m_position_y; //0x0010
	char pad_0012[2]; //0x0012
	uint8_t m_position_z; //0x0014
	char pad_0015[21]; //0x0015
	uint16_t m_serverId; //0x002A
	char pad_002C[2]; //0x002C
	uint8_t m_countOrSubType; //0x002E
}; //Size: 0x002F