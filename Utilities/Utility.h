#pragma once
#pragma comment(lib, "User32.lib")

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "../src/Room.h"
#include "CustomNamespace.h"
#include <random>


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

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void fullsc()
{
    HWND Hwnd = GetForegroundWindow();
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    LONG winstyle = GetWindowLong(Hwnd, GWL_STYLE);
    SetWindowLong(Hwnd, GWL_STYLE, (winstyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(Hwnd,HWND_TOP,0,0,x,y,0);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}


std::vector<std::vector<std::string>> readFramesFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file!");
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); 
    std::string content = buffer.str();

    std::vector<std::vector<std::string>> frames;
    size_t start = 0;
    size_t end = content.find('&');

    while (end != std::string::npos) {
        std::string frameContent = content.substr(start, end - start);
        std::vector<std::string> frame;

        std::stringstream frameStream(frameContent);
        std::string line;
        while (std::getline(frameStream, line)) {
            frame.push_back(line);
        }

        frames.push_back(frame);
        start = end + 1;
        end = content.find('&', start);
    }

    if (start < content.length()) {
        std::string frameContent = content.substr(start);
        std::vector<std::string> frame;

        std::stringstream frameStream(frameContent);
        std::string line;
        while (std::getline(frameStream, line)) {
            frame.push_back(line);
        }

        frames.push_back(frame);
    }

    return frames;
}