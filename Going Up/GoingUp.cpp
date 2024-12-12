#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <windows.h>
#include <limits>
#include <set>

#define MAX_MAP_X 50
#define MAX_MAP_Y 30


////////////// Utility Classes And Functions //////////////

void SetCursorAtXY(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, pos);
}

class Queue {
private:
    std::vector<int> data;
    int frontIndex;

public:
    Queue() : frontIndex(0) {}

    void enqueue(int value) {
        data.push_back(value);
    }

    int dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[frontIndex++];
    }

    bool isEmpty() const {
        return frontIndex >= data.size();
    }
};

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



class Floor {
private:
    int floorNumber;
    std::vector<Room> rooms;
    static std::set<std::pair<int, int>> occupiedPositions;

public:
    Floor(int floorNumber, int minRooms, int maxRooms) : floorNumber(floorNumber), rooms(rooms) {
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

        rooms.emplace_back(Room(2, RoomType::Large, {RandomCoord(), RandomCoord()}));
        rooms.emplace_back(Room(3, RoomType::Large, {RandomCoord(), RandomCoord()}));

        for (int i = 4; i < roomCount; ++i) {
            RoomType randomType = static_cast<RoomType>(1 + rand() % 3);
            rooms.emplace_back(Room(i, randomType, {RandomCoord(), RandomCoord()}));
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
                double minDistance = std::numeric_limits<double>::max();

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

    int RandomCoord() const {
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
        return x, y;
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

std::set<std::pair<int, int>> Floor::occupiedPositions;


class Player {
    private:
        int health;
        int maxHealth;
        Room currentRoom;

    public:
        Player(Room currentRoom) : health(100), maxHealth(100), currentRoom(currentRoom) {}

        void move(Room targetRoom) {
            if (std::find(currentRoom.connectedRooms.begin(), currentRoom.connectedRooms.end(), targetRoom.roomNumber) != currentRoom.connectedRooms.end()) {
                currentRoom = targetRoom;
            }
        }

        void takeDamage(int damage) {
            health -= damage;
        }

        void heal(int amount) {
            health += amount;
            if (health > maxHealth) {
                health = maxHealth;
            }
        }

        void printStatus() {
            std::cout << "Health: " << health << "/" << maxHealth << std::endl;
        }  

        bool isAlive() {
            return health > 0;
        }  
};

class Enemy {
    private:
        int health;
        int damage;
        Room currentRoom;
    
    public:
        Enemy(Room currentRoom) : health(50), damage(10), currentRoom(currentRoom) {}

        void move(Room targetRoom) {
            if (std::find(currentRoom.connectedRooms.begin(), currentRoom.connectedRooms.end(), targetRoom.roomNumber) != currentRoom.connectedRooms.end()) {
                currentRoom = targetRoom;
            }
        }

        void takeDamage(int damage) {
            health -= damage;
        }

        void printStatus() {
            std::cout << "Health: " << health << std::endl;
        }

        bool isAlive() {
            return health > 0;
        }
};

class Combat {
    private:
        Enemy enemy;
        Player player;
    
    public:
        Combat(Enemy enemy, Player& player) : enemy(enemy), player(player) {}

        void startCombat() {
            while (enemy.isAlive() && player.isAlive()) {
                player.takeDamage(10);
                enemy.takeDamage(10);
                player.printStatus();
                enemy.printStatus();
            }
        }
};

int main() {
    Floor floor(1, 5, 10);
    system("cls");
    floor.DrawMap();
    SetCursorAtXY(0, MAX_MAP_Y + 5);
    floor.printMapAndConnections();
    std::cin.get();
    return 0;
}