#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include "../Utilities/Enums.h"

struct Room {
    int roomNumber;
    RoomType roomType;
    std::pair<int, int> position;
    bool isVisited;
    std::vector<int> connectedRooms;

    struct Hash {
        size_t operator()(const Room& room) const {
            size_t h1 = std::hash<int>()(room.roomNumber);
            size_t h2 = std::hash<int>()(room.position.first);
            size_t h3 = std::hash<int>()(room.position.second);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };

    // Equality operator
    bool operator==(const Room& other) const {
        return roomNumber == other.roomNumber &&
               position == other.position &&
               roomType == other.roomType;
    }

    friend std::ostream& operator<<(std::ostream& os, const Room& room) {
        os << "Room " << room.roomNumber << ": " << "Type: " << static_cast<int>(room.roomType) << " Position: (" << room.position.first << ", " << room.position.second << ")";
        return os;
    }

    Room(int roomNumber, RoomType roomType, std::pair<int, int> position) 
        : roomNumber(roomNumber), roomType(roomType), position(position), isVisited(false) {}

    double distanceTo(const Room& other) const {
        return std::sqrt(std::pow(position.first - other.position.first, 2) + std::pow(position.second - other.position.second, 2));
    }

    std::vector<int> getConnectedRooms() const {
        std::vector<int> visitedRooms;
        for (int roomNumber : connectedRooms) {
            visitedRooms.push_back(roomNumber);
        }
        return visitedRooms;
    }
};

#endif // ROOM_H