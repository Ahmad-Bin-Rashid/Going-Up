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
class ScreenBuffer
{
    private:
        std::vector<std::string> buffer;
        int width, height;
    
    public:
        ScreenBuffer(int w, int h) : width(w), height(h), buffer(h, std::string(w, ' ')) {}

        void setChar(int x, int y, char c) 
        {
            if (y >= 0 && y < height && x >= 0 && x < width) 
                {
                    buffer[y][x] = c;
                }
        }

        void setString(int x, int y, const std::string &str) 
        {
            if (y >= 0 && y < height) 
            {
                for (size_t i = 0; i < str.size() && x + i < width; ++i) 
                {
                    buffer[y][x + i] = str[i];
                }
            }
        }
        void clear() 
        {
            for (auto &row : buffer) 
                {
                    row = std::string(width, ' ');
                }
        }
        void print() const 
        {
            for (const auto &row : buffer) 
                {
                    std::cout << row << '\n';
                }
        }
};