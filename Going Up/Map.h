#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <ctime>
#include "Room.h"
#include "Queue.h"
#include "Utility.h"


#define MAX_MAP_X 50
#define MAX_MAP_Y 30

class Floor {
private:
    int floorNumber;
    std::vector<Room> rooms;
    static std::set<std::pair<int, int>> occupiedPositions;

public:
    Floor(int floorNumber, int minRooms, int maxRooms) : floorNumber(floorNumber) {
        GenerateMap(minRooms, maxRooms);
    }

    void GenerateMap(int minRooms, int maxRooms) {
        srand(time(nullptr));
        int roomCount = rand() % (maxRooms - minRooms + 1) + minRooms;

        // Selecting a Random Position for the Boss Room
        std::vector<std::pair<int, int>> BossPositions = {{MAX_MAP_X, MAX_MAP_Y}, {MAX_MAP_X, 0}, {0, MAX_MAP_Y}};
        std::pair<int, int> bossPosition = BossPositions[rand() % BossPositions.size()];

        Room spawn(0, RoomType::Spawn, {0, 0});
        Room boss(1, RoomType::Boss, bossPosition);
        rooms.push_back(spawn);
        rooms.push_back(boss);

        rooms.emplace_back(Room(2, RoomType::Large, RandomCoord()));
        rooms.emplace_back(Room(3, RoomType::Large, RandomCoord()));

        for (int i = 4; i < roomCount; ++i) {
            RoomType randomType = static_cast<RoomType>(1 + rand() % 3);
            rooms.emplace_back(Room(i, randomType, RandomCoord()));
        }

        connectRooms();
    }

    void connectRooms() {

        // Continue with normal connection logic
        for (auto& room : rooms) {
            int minConnections = (room.roomType == RoomType::Spawn || room.roomType == RoomType::Boss) ? 1 : 2;
            int maxConnections = (room.roomType == RoomType::Spawn || room.roomType == RoomType::Boss) ? 1 : 3;

            // Create a list of rooms excluding the current room
            std::vector<Room> potentialConnections;
            for (auto& other : rooms) {
                if (!(room == other)) { // Exclude the current room
                    potentialConnections.push_back(other);
                }
            }

            // Sort rooms by distance to the current room
            std::sort(potentialConnections.begin(), potentialConnections.end(),
                    [&room](const Room& a, const Room& b) {
                        return room.distanceTo(a) < room.distanceTo(b);
                    });

            // Determine the number of connections to make
            int connectionsToAdd = rand() % (maxConnections - minConnections + 1) + minConnections;

            // Add connections to the nearest rooms
            for (int i = 0; i < connectionsToAdd && i < potentialConnections.size(); ++i) {
                Room& targetRoom = potentialConnections[i];

                // Avoid duplicate connections
                if (std::find(room.connectedRooms.begin(), room.connectedRooms.end(), targetRoom.roomNumber) == room.connectedRooms.end()) {
                    room.connectedRooms.push_back(targetRoom.roomNumber);
                    targetRoom.connectedRooms.push_back(room.roomNumber); // Bidirectional connection
                }
            }
        }

        // Ensure graph connectivity
        ensureGraphConnectivity();
    }


    void ensureGraphConnectivity() {

        // Track visited rooms
        std::vector<bool> visited(rooms.size(), false);
        Queue toVisit;
        toVisit.enqueue(0); // Start with the first room (Spawn)

        // Perform BFS to mark reachable rooms
        while (!toVisit.isEmpty()) {
            int current = toVisit.dequeue();
            if (visited[current]) continue;

            visited[current] = true;

            // Enqueue all connected rooms
            for (int connectedRoomNumber : rooms[current].connectedRooms) {
                if (!visited[connectedRoomNumber]) {
                    toVisit.enqueue(connectedRoomNumber);
                }
            }
        }

        // Check for unvisited rooms and connect them to the nearest visited room
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (!visited[i]) {
                int nearestVisited = -1;
                double minDistance = std::numeric_limits<double>::infinity();

                // Find the nearest visited room
                for (size_t j = 0; j < rooms.size(); ++j) {
                    if (visited[j]) {
                        double distance = rooms[i].distanceTo(rooms[j]);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestVisited = j;
                        }
                    }
                }

                // Connect the unvisited room to the nearest visited room
                if (nearestVisited != -1) {
                    rooms[i].connectedRooms.push_back(nearestVisited);
                    rooms[nearestVisited].connectedRooms.push_back(i);
                    visited[i] = true; // Mark as visited after connection
                }
            }
        }
    }

    std::pair<int, int> RandomCoord() const {
        int x, y;

        // Keep generating coordinates until a unique one and not near others are found
        do {
            x = rand() % MAX_MAP_X + 1;
            y = rand() % MAX_MAP_Y + 1;

            // Ensure non-nearby coordinates
            if (!occupiedPositions.empty()) {
                auto lastPos = *occupiedPositions.rbegin(); // Last used position
                if (std::abs(x - lastPos.first) <= 10 || std::abs(y - lastPos.second) <= 3) {
                    continue; // Skip near positions
                }
            }

        } while (occupiedPositions.find({x, y}) != occupiedPositions.end());

        occupiedPositions.insert({x, y});
        return {x, y};
    }


     void DrawMap() const {
        drawConnections();
        for (const auto& room : rooms) {
            char symbol = ' ';
            switch (room.roomType) {
                case RoomType::Spawn:    symbol = 'S'; break;
                case RoomType::Treasure: symbol = 'T'; break;
                case RoomType::Trap:     symbol = 'X'; break;
                case RoomType::Large:    symbol = 'L'; break;
                case RoomType::Boss:     symbol = 'B'; break;
            }

            SetCursorAtXY(room.position.first, room.position.second);
            std::cout << symbol;
        }
    }

    void drawConnections() const {
        for (const auto& room : rooms) {
            for (int connectedRoomNumber : room.connectedRooms) {
                if (connectedRoomNumber >= 0 && connectedRoomNumber < rooms.size()) {
                    const Room& connectedRoom = rooms[connectedRoomNumber];
                    drawLine(room.position.first, room.position.second, connectedRoom.position.first, connectedRoom.position.second);
                }
            }
        }
    }

    void drawLine(int x1, int y1, int x2, int y2) const {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            SetCursorAtXY(x1, y1);
            std::cout << '.';

            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

    void printMapAndConnections() {
        for (const auto& room : rooms) {
            std::cout << room << " Connected Rooms: ";
            for (int connectedRoomNumber : room.connectedRooms) {
                std::cout << connectedRoomNumber << " ";
            }
            std::cout << std::endl;
        }
    }
};