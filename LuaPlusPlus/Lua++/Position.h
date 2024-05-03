#pragma once
#include <vector>
#include "common_helpers.h"
class Position
{
public:
	Position() : x(65535), y(65535), z(255) { }
	Position(int x, int y, int z) : x(x), y(y), z(z) { }

	Position translatedToDirection(Otc::Direction direction) {
		Position pos = *this;
		switch (direction) {
		case Otc::North:
			pos.y--;
			break;
		case Otc::East:
			pos.x++;
			break;
		case Otc::South:
			pos.y++;
			break;
		case Otc::West:
			pos.x--;
			break;
		case Otc::NorthEast:
			pos.x++;
			pos.y--;
			break;
		case Otc::SouthEast:
			pos.x++;
			pos.y++;
			break;
		case Otc::SouthWest:
			pos.x--;
			pos.y++;
			break;
		case Otc::NorthWest:
			pos.x--;
			pos.y--;
			break;
		default:
			break;
		}
		return pos;
	}

	Position translatedToReverseDirection(Otc::Direction direction) {
		Position pos = *this;
		switch (direction) {
		case Otc::North:
			pos.y++;
			break;
		case Otc::East:
			pos.x--;
			break;
		case Otc::South:
			pos.y--;
			break;
		case Otc::West:
			pos.x++;
			break;
		case Otc::NorthEast:
			pos.x--;
			pos.y++;
			break;
		case Otc::SouthEast:
			pos.x--;
			pos.y--;
			break;
		case Otc::SouthWest:
			pos.x++;
			pos.y--;
			break;
		case Otc::NorthWest:
			pos.x++;
			pos.y++;
			break;
		default:
			break;
		}
		return pos;
	}

	std::vector<Position> translatedToDirections(const std::vector<Otc::Direction>& dirs) const {
		Position lastPos = *this;
		std::vector<Position> positions;

		if (!lastPos.isValid())
			return positions;

		positions.push_back(lastPos);

		for (auto dir : dirs) {
			lastPos = lastPos.translatedToDirection(dir);
			if (!lastPos.isValid())
				break;
			positions.push_back(lastPos);
		}

		return positions;
	}

	static double getAngleFromPositions(const Position& fromPos, const Position& toPos) {
		// Returns angle in radians from 0 to 2Pi. -1 means positions are equal.
		int dx = toPos.x - fromPos.x;
		int dy = toPos.y - fromPos.y;
		if (dx == 0 && dy == 0)
			return -1;

		float angle = std::atan2(dy * -1, dx);
		if (angle < 0)
			angle += 2 * M_PI;

		return angle;
	}

	double getAngleFromPosition(const Position& position) const {
		return getAngleFromPositions(*this, position);
	}

	static Otc::Direction getDirectionFromPositions(const Position& fromPos,
		const Position& toPos)
	{
		float angle = getAngleFromPositions(fromPos, toPos) * RAD_TO_DEC;

		if (angle >= 360 - 22.5 || angle < 0 + 22.5)
			return Otc::East;
		else if (angle >= 45 - 22.5 && angle < 45 + 22.5)
			return Otc::NorthEast;
		else if (angle >= 90 - 22.5 && angle < 90 + 22.5)
			return Otc::North;
		else if (angle >= 135 - 22.5 && angle < 135 + 22.5)
			return Otc::NorthWest;
		else if (angle >= 180 - 22.5 && angle < 180 + 22.5)
			return Otc::West;
		else if (angle >= 225 - 22.5 && angle < 225 + 22.5)
			return Otc::SouthWest;
		else if (angle >= 270 - 22.5 && angle < 270 + 22.5)
			return Otc::South;
		else if (angle >= 315 - 22.5 && angle < 315 + 22.5)
			return Otc::SouthEast;
		else
			return Otc::InvalidDirection;
	}

	Otc::Direction getDirectionFromPosition(const Position& position) const {
		return getDirectionFromPositions(*this, position);
	}

	bool isMapPosition() const { return (x >= 0 && y >= 0 && z >= 0 && x < 65535 && y < 65535 && z <= Otc::MAX_Z); }
	bool isValid() const { return !(x == 65535 && y == 65535 && z == 255); }
	float distance(const Position& pos) const { return sqrt(pow((pos.x - x), 2) + pow((pos.y - y), 2)); }
	int manhattanDistance(const Position& pos) const { return std::abs(pos.x - x) + std::abs(pos.y - y); }

	void translate(int dx, int dy, short dz = 0) { x += dx; y += dy; z += dz; }
	Position translated(int dx, int dy, short dz = 0) const { Position pos = *this; pos.x += dx; pos.y += dy; pos.z += dz; return pos; }

	Position operator+(const Position& other) const { return Position(x + other.x, y + other.y, z + other.z); }
	Position& operator+=(const Position& other) { x += other.x; y += other.y; z += other.z; return *this; }
	Position operator-(const Position& other) const { return Position(x - other.x, y - other.y, z - other.z); }
	Position& operator-=(const Position& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	// Point conversion(s)
	//Position operator+(const Point& other) const { return Position(x + other.x, y + other.y, z); }
	//Position& operator+=(const Point& other) { x += other.x; y += other.y; return *this; }

	Position& operator=(const Position& other) { x = other.x; y = other.y; z = other.z; return *this; }
	bool operator==(const Position& other) const { return other.x == x && other.y == y && other.z == z; }
	bool operator!=(const Position& other) const { return other.x != x || other.y != y || other.z != z; }
	bool isInRange(const Position& pos, int xRange, int yRange) const { return std::abs(x - pos.x) <= xRange && std::abs(y - pos.y) <= yRange && z == pos.z; }
	bool isInRange(const Position& pos, int minXRange, int maxXRange, int minYRange, int maxYRange) const {
		return (pos.x >= x - minXRange && pos.x <= x + maxXRange && pos.y >= y - minYRange && pos.y <= y + maxYRange && pos.z == z);
	}
	// operator less than for std::map
	bool operator<(const Position& other) const { return x < other.x || y < other.y || z < other.z; }

	bool up(int n = 1) {
		int nz = z - n;
		if (nz >= 0 && nz <= Otc::MAX_Z) {
			z = nz;
			return true;
		}
		return false;
	}

	bool down(int n = 1) {
		int nz = z + n;
		if (nz >= 0 && nz <= Otc::MAX_Z) {
			z = nz;
			return true;
		}
		return false;
	}

	bool coveredUp(int n = 1) {
		int nx = x + n, ny = y + n, nz = z - n;
		if (nx >= 0 && nx <= 65535 && ny >= 0 && ny <= 65535 && nz >= 0 && nz <= Otc::MAX_Z) {
			x = nx; y = ny; z = nz;
			return true;
		}
		return false;
	}

	bool coveredDown(int n = 1) {
		int nx = x - n, ny = y - n, nz = z + n;
		if (nx >= 0 && nx <= 65535 && ny >= 0 && ny <= 65535 && nz >= 0 && nz <= Otc::MAX_Z) {
			x = nx; y = ny; z = nz;
			return true;
		}
		return false;
	}



	int x;
	int y;
	short z;

	friend std::ostream & operator<<(std::ostream & _stream, Position const & mc) {
		return _stream << "(X:" << mc.x << ", Y:" << mc.y << ", Z:" << mc.z << ")";
	}
};

struct PositionHasher : std::unary_function<Position, std::size_t> {
	std::size_t operator()(const Position& pos) const {
		return (((pos.x * 8192) + pos.y) * 16) + pos.z;
	}
};
