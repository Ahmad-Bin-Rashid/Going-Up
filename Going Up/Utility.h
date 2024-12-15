#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Room.h"


std::string roomTypeToString(RoomType roomType) {
    switch (roomType) {
        case RoomType::Spawn: return "Spawn";
        case RoomType::Boss: return "Boss";
        case RoomType::Trap: return "Trap";
        case RoomType::Large: return "Large";
        case RoomType::Treasure: return "Treasure";
        default: return "Unknown";
    }
}

RoomType stringToRoomType(const std::string& type) {
    if (type == "Spawn") return RoomType::Spawn;
    if (type == "Boss") return RoomType::Boss;
    if (type == "Trap") return RoomType::Trap;
    if (type == "Large") return RoomType::Large;
    if (type == "Treasure") return RoomType::Treasure;
    return RoomType::Spawn;
}
