#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include "Enums.h"

struct Room {
    int roomNumber;
    RoomType roomType;
    std::pair<int, int> position;
    bool isVisited;
    std::vector<int> connectedRooms;

    bool operator==(const Room& other) const {
        return roomNumber == other.roomNumber;
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
            // Assuming you have a way to check if a room is visited
            // This part of the code needs context on how you manage rooms
            // For now, we assume all rooms in connectedRooms are visited
            visitedRooms.push_back(roomNumber);
        }
        return visitedRooms;
    }
};

struct RoomHash {
    std::size_t operator()(const Room& room) const {
        return std::hash<int>()(room.roomNumber);
    }
};

#endif // ROOM_H