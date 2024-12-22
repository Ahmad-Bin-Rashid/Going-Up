#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "../src/Room.h"
#include "CustomNamespace.h"


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


// Color Numbers 
// 0: Black
// 1: Red
// 2: Green
// 3: Yellow
// 4: Blue
// 5: Magenta
// 6: Cyan
// 7: White
// 8-15: Bright Colors
// 16-231: 216 Colors
// 232-255: Grayscale
void ChangeTextColor(int color) {
    ct::p | "\033[38;5;" | color | "m";
}
void ChangeBackgroundColor(int color) {
    ct::p | "\033[48;5;" | color | "m\033[2J\033[H";
}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
