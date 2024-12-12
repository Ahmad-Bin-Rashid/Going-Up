#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <iostream>

enum class RoomType 
{
    Spawn,
    Treasure,
    Trap,
    Large,
    Boss
};

struct Room {
    int roomNumber;
    RoomType roomType;
    std::pair<int, int> position;
    bool isVisited;
    std::vector<int> connectedRooms;
    int operator==(const Room& other) {
        return roomNumber == other.roomNumber;
    }
    friend std::ostream& operator<<(std::ostream& os, const Room& room) {
        os << "Room " << room.roomNumber << ": " << "Type: " << static_cast<int>(room.roomType) << " Position: (" << room.position.first << ", " << room.position.second << ")";
        return os;
    }

    Room(int roomNumber, RoomType roomType, std::pair<int, int> position) : roomNumber(roomNumber) {
        this->roomType = roomType;
        this->position = position;
        this->isVisited = false;
        connectedRooms = {};
    }

    double distanceTo(Room other) {
        return sqrt(pow(position.first - other.position.first, 2) + pow(position.second - other.position.second, 2));
    }

    std::vector<Room> getConnectedRooms() {
        std::vector<Room> connectedRooms;
        for (int i = 0; i < connectedRooms.size(); i++) {
            if (connectedRooms[i].isVisited == true) 
                connectedRooms.push_back(connectedRooms[i]);
            else
                continue;
        }
        return connectedRooms;
    }
};

#endif // ROOM_H