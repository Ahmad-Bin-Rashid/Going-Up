#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <memory>
#include <cmath>
#include <ctime>
#include <variant>
#include "Utility.h"
#include "Room.h"
#include "Queue.h"
#include <string>
#include "Enums.h"
#include "DoublyLinkedList.h"
#include <unordered_map>

#define MAX_MAP_X 50
#define MAX_MAP_Y 30

class Character;

class AttackCard {
protected:
    std::string name;
    int damage;
public:
    AttackCard(std::string name, int damage) : name(name), damage(damage) {}
    virtual void use(Character& character) = 0;
    std::string getName() const { return name; }
    virtual bool operator==(const AttackCard& other) const {
        return name == other.name && damage == other.damage;
    }
    virtual ~AttackCard() = default;
};

class SingleSlash : public AttackCard {
public:
    SingleSlash() : AttackCard("Single Slash", 1) {}

    void use(Character& character);
};

class DoubleSlash : public AttackCard {
public:
    DoubleSlash() : AttackCard("Double Slash", 2) {}

    void use(Character& character);
};

class Bow : public AttackCard {
public:
    Bow() : AttackCard("Bow", 2) {}

    void use(Character& character);
};

using CardVariant = std::variant<DoubleSlash, Bow>;

class Character {
protected:
    int health;
    int maxHealth;
    Room currentRoom;
    CharacterDirection direction;
    std::vector<CardVariant> cards;

public:
    Character(int health, int maxHealth, Room currentRoom)
        : health(health), maxHealth(maxHealth), currentRoom(currentRoom), direction(CharacterDirection::Right) {}

    virtual void takeDamage(int damage) {
        health -= damage;
    }

    virtual void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    virtual void printStatus() const {
        std::cout << "Health: " << health << "/" << maxHealth << std::endl;
    }

    virtual bool isAlive() const {
        return health > 0;
    }

    void SetCurrentRoom(Room room) {
        currentRoom = room;
    }

    void addCard(const CardVariant& card) {
        cards.push_back(card);
    }

    void removeCard(const CardVariant& card) {
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
    }

    void useCard(size_t index, Character& target) {
        if (index < cards.size()) {
            std::visit([&target](auto& card) { card.use(target); }, cards[index]);
        }
    }

    std::vector<CardVariant> getCards() const {
        return cards;
    }

    virtual ~Character() = default;
};

class Player : public Character {
public:
    Player(Room currentRoom) : Character(100, 100, currentRoom) {}

    void takeDamage(int damage) override {
        health -= damage;
        if (health <= 0) {
            std::cout << "Player defeated!" << std::endl;
        }
    }

    void heal(int amount) override {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    void printStatus() const override {
        std::cout << "Player Health: " << health << "/" << maxHealth << std::endl;
    }

    ~Player() = default;
};

class Enemy : public Character {
public:
    Enemy(Room currentRoom) : Character(20, 20, currentRoom) {}

    void takeDamage(int damage) override {
        health -= damage;
        if (health <= 0) {
            std::cout << "Enemy defeated!" << std::endl;
        }
    }

    void printStatus() const override {
        std::cout << "Enemy Health: " << health << "/" << maxHealth << std::endl;
    }

    bool operator==(const Enemy& other) const {
        return health == other.health && maxHealth == other.maxHealth && currentRoom == other.currentRoom;
    }

    ~Enemy() = default;
};

void SingleSlash::use(Character& character) {
    character.takeDamage(damage);
}

void DoubleSlash::use(Character& character) {
    character.takeDamage(damage);
}

void Bow::use(Character& character) {
    character.takeDamage(damage);
}

class Combat {
private:
    Player& player;
    std::vector<Enemy> enemies;

public:
    DoublyLinkedList<std::unordered_map<int, std::string>> dll;
    Combat(Player& player, std::vector<Enemy>& enemies) : player(player), enemies(enemies) {}

    void makeFloor() {

        // Add some empty tiles
        for (int i = 1; i <= 3; ++i) {
            dll.pushBack({{i, "X"}});
        }

        // Add the player
        dll.pushBack({{4, "P"}});

        // Add some more empty tiles
        for (int i = 5; i <= 7; ++i) {
            dll.pushBack({{i, "X"}});
        }

        // Add enemies with unique names
        for (size_t i = 0; i < enemies.size(); ++i) {
            std::string enemyName = "E" + std::to_string(i + 1);
            dll.pushBack({{8 + static_cast<int>(i), enemyName}});
        }

        // Add some more empty tiles
        for (int i = 8 + static_cast<int>(enemies.size()); i <= 10 + static_cast<int>(enemies.size()); ++i) {
            dll.pushBack({{i, "X"}});
        }

        // Print the initial floor layout
        printTiles(dll);

        Sleep(2000);

        // Start the combat
        startCombat(dll);
    }

    void startCombat(DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        while (true) {
            printTiles(dll);
            int choice;
            std::cout << "Enemy's turn: " << std::endl;
            std::cout << "1. Move Forward" << std::endl;
            std::cout << "2. Move Backward" << std::endl;
            std::cout << "3. Attack" << std::endl;
            std::cin >> choice;

            if (choice == 1) {
                moveEnemiesForward(dll);
            } else if (choice == 2) {
                moveEnemiesBackward(dll);
            } else if (choice == 3) {
                attack(dll);
            }

            if (!player.isAlive()) {
                std::cout << "Player defeated!" << std::endl;
                Sleep(2000);
                break;
            }

            printTiles(dll);
            std::cout << "Player's turn: " << std::endl;
            std::cout << "1. Move Forward" << std::endl;
            std::cout << "2. Move Backward" << std::endl;
            std::cout << "3. Use Card" << std::endl;
            std::cin >> choice;

            if (choice == 1) {
                movePlayerForward(dll);
            } else if (choice == 2) {
                movePlayerBackward(dll);
            } else if (choice == 3) {
                std::cout << "Select a card to use: " << std::endl;
                std::vector<CardVariant> cards = player.getCards();
                for (size_t i = 0; i < cards.size(); ++i) {
                    std::cout << i << ". ";
                    std::visit([](const auto& card) { std::cout << card.getName() << std::endl; }, cards[i]);
                }
                std::cin >> choice;

                if (choice < 0 || choice >= cards.size()) {
                    std::cout << "Invalid choice!" << std::endl;
                    continue;
                }

                for (auto& enemy : enemies) {
                    player.useCard(choice, enemy);
                    if (!enemy.isAlive()) {
                        enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
                    }
                }
            }

            if (enemies.empty()) {
                std::cout << "All enemies defeated!" << std::endl;
                break;
            }
        }
    }

    void movePlayerForward(DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        int position = dll.getPositionFromString("P");
        if (position != -1 && position < dll.getSize() && dll.getDataFromPosition(position + 1).begin()->second == "X") {
            dll.setNodeAt(position + 1, {{position + 1, "P"}});
            dll.setNodeAt(position, {{position, "X"}});
        }
    }

    void movePlayerBackward(DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        int position = dll.getPositionFromString("P");
        if (position != -1 && position > 1 && dll.getDataFromPosition(position - 1).begin()->second == "X") {
            dll.setNodeAt(position - 1, {{position - 1, "P"}});
            dll.setNodeAt(position, {{position, "X"}});
        }
    }

    void moveEnemiesForward(DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        for (int i = dll.getSize(); i >= 1; --i) {
            if (dll.getDataFromPosition(i).begin()->second == "E1" || dll.getDataFromPosition(i).begin()->second == "E2") {
                if (i < dll.getSize() && dll.getDataFromPosition(i + 1).begin()->second == "X") {
                    dll.setNodeAt(i + 1, {{i + 1, dll.getDataFromPosition(i).begin()->second}});
                    dll.setNodeAt(i, {{i, "X"}});
                }
            }
        }
    }

    void moveEnemiesBackward(DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        for (int i = 1; i <= dll.getSize(); ++i) {
            if (dll.getDataFromPosition(i).begin()->second == "E1" || dll.getDataFromPosition(i).begin()->second == "E2") {
                if (i > 1 && dll.getDataFromPosition(i - 1).begin()->second == "X") {
                    dll.setNodeAt(i - 1, {{i - 1, dll.getDataFromPosition(i).begin()->second}});
                    dll.setNodeAt(i, {{i, "X"}});
                }
            }
        }
    }

    void attack(DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        int playerPos = dll.getPositionFromString("P");
        for (int i = 1; i <= dll.getSize(); ++i) {
            if ((dll.getDataFromPosition(i).begin()->second == "E1" || dll.getDataFromPosition(i).begin()->second == "E2") &&
                (std::abs(playerPos - i) == 1)) {
                dll.setNodeAt(i, {{i, "X"}});
                std::cout << "Enemy at position " << i << " defeated!" << std::endl;
            }
        }
    }

    void printTiles(const DoublyLinkedList<std::unordered_map<int, std::string>>& dll) {
        dll.printForward();
    }

    ~Combat() = default;
};

class Floor {
private:
    int floorNumber;
    std::vector<Room> rooms;
    static std::set<std::pair<int, int>> occupiedPositions;

public:
    Floor(int floorNumber, int minRooms, int maxRooms) : floorNumber(floorNumber) {
        GenerateMap(minRooms, maxRooms);
    }

    const std::vector<Room>& getRooms() const { return rooms; }

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
        for (auto& room : rooms) {
            // Skip connecting the boss room
            if (room.roomType == RoomType::Boss) {
                continue;
            }

            int minConnections = (room.roomType == RoomType::Spawn) ? 1 : 2;
            int maxConnections = (room.roomType == RoomType::Spawn) ? 1 : 3;

            // Create a list of rooms excluding the current room and the boss room
            std::vector<Room> potentialConnections;
            for (auto& other : rooms) {
                if (!(room == other) && other.roomType != RoomType::Boss) { // Exclude the current room and the boss room
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

        // Debugging output
        for (const auto& room : rooms) {
            std::cout << "Room " << room.roomNumber << " connections: ";
            for (const auto& connectedRoom : room.connectedRooms) {
                std::cout << connectedRoom << " ";
            }
            std::cout << std::endl;
        }
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
            if (!visited[i] && rooms[i].roomType != RoomType::Boss) { // Skip the boss room
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
                    rooms[nearestVisited].connectedRooms.push_back(i); // Bidirectional connection
                    visited[i] = true; // Mark as visited after connection
                }
            }
        }

        // Debugging output
        for (const auto& room : rooms) {
            std::cout << "Room " << room.roomNumber << " connections: ";
            for (const auto& connectedRoom : room.connectedRooms) {
                std::cout << connectedRoom << " ";
            }
            std::cout << std::endl;
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


    Room getBossRoom() const {
        for (const auto& room : rooms) {
            if (room.roomType == RoomType::Boss) {
                return room;
            }
        }
        throw std::runtime_error("Boss room not found");
    }

    Room getSpawnRoom() const {
        for (const auto& room : rooms) {
            if (room.roomType == RoomType::Spawn) {
                return room;
            }
        }
        throw std::runtime_error("Spawn room not found");
    }

    Room getRoomByPosition(std::pair<int, int> position) const {
        for (const auto& room : rooms) {
            if (room.position == position) {
                return room;
            }
        }
        throw std::runtime_error("Room not found");
    }

    void DrawMap(const std::set<int>& visitedRooms) const {
        drawConnections(visitedRooms);
        for (const auto& room : rooms) {
            if (visitedRooms.find(room.roomNumber) != visitedRooms.end()) {
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
    }

    void drawConnections(const std::set<int>& visitedRooms) const {
        for (const auto& room : rooms) {
            if (visitedRooms.find(room.roomNumber) != visitedRooms.end()) {
                for (int connectedRoomNumber : room.connectedRooms) {
                    if (visitedRooms.find(connectedRoomNumber) != visitedRooms.end()) {
                        const Room& connectedRoom = rooms[connectedRoomNumber];
                        drawLine(room.position.first, room.position.second, connectedRoom.position.first, connectedRoom.position.second);
                    }
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


class RoomBase {
public:
    bool isCleared = false;
    virtual ~RoomBase() = default;
    virtual void enterRoom(Player& player) = 0;
};

class LargeRoom : public RoomBase {
private:
    Room room;
    Combat combat;
    std::vector<Enemy> enemies;

    static std::vector<Enemy> createEnemies(const Room& room) {
        srand(time(nullptr));
        std::vector<Enemy> enemies;
        // Create enemies based on room properties
        enemies.push_back(Enemy(room));
        enemies.push_back(Enemy(room));
        return enemies;
    }

public:
    LargeRoom(const Room& room, Player& player)
        : room(room), enemies(createEnemies(room)), combat(player, enemies) {}

    void enterRoom(Player& player) override {
        if (isCleared) {
            std::cout << "You have already cleared this room!" << std::endl;
            Sleep(2000);
            return;
        } else {
            std::cout << "You have entered a Large room at " << room.position.first << ", " << room.position.second << std::endl;
            Sleep(2000);
            combat.makeFloor(); // Initialize the floor layout
            combat.startCombat(combat.dll);
            isCleared = true;
        }
    }
};

class TreasureRoom : public RoomBase {
private:
    Room room;
public:
    TreasureRoom(const Room& room) : room(room) {}
    void enterRoom(Player& player) override {
        if (isCleared) {
            std::cout << "You have already cleared this room!" << std::endl;
            Sleep(2000);
            return;
        }
        std::cout << "You have entered a Treasure room at " << room.position.first << ", " << room.position.second << std::endl;
        Sleep(2000);
        isCleared = true;
    }
};

class TrapRoom : public RoomBase {
private:
    Room room;
public:
    TrapRoom(const Room& room) : room(room) {}
    void enterRoom(Player& player) override {
        if (isCleared) {
            std::cout << "You have already cleared this room!" << std::endl;
            Sleep(2000);
            return;
        }
        std::cout << "You have entered a trap room at " << room.position.first << ", " << room.position.second << std::endl;
        Sleep(2000);
        isCleared = true;
    }
};

class BossRoom : public RoomBase {
private:
    Room room;

public:
    BossRoom(const Room& room) : room(room) {}
    void enterRoom(Player& player) override {
        if (isCleared) {
            std::cout << "You have already cleared this room!" << std::endl;
            Sleep(2000);
            return;
        }
        std::cout << "You have entered the Boss room at " << room.position.first << ", " << room.position.second << std::endl;
        Sleep(2000);
        isCleared = true;
    }
};


using RoomVariant = std::variant<std::unique_ptr<LargeRoom>, std::unique_ptr<TreasureRoom>, std::unique_ptr<TrapRoom>, std::unique_ptr<BossRoom>>;

class GameLogic {
private:
    Floor floor;
    Room currentRoom;
    Player player;
    std::set<int> visitedRooms; // Set to keep track of visited rooms
    std::unordered_map<Room, RoomVariant, RoomHash> map;

public:
    GameLogic(int floorNumber, int minRooms, int maxRooms)
        : floor(floorNumber, minRooms, maxRooms), currentRoom(floor.getRooms().front()), player(currentRoom) {
        visitedRooms.insert(currentRoom.roomNumber); // Mark the spawn room as visited
    }

    void MainLoop() {
        system("cls");
        floor.DrawMap(visitedRooms); // Pass visited rooms to DrawMap
        std::cout << "Welcome to the dungeon!" << std::endl;
        Sleep(2000);
        std::vector<Room> rooms = floor.getRooms();

        for (const auto& room : rooms) {
            switch (room.roomType) {
                case RoomType::Large: {
                    map[room] = std::make_unique<LargeRoom>(room, player);
                    break;
                }
                case RoomType::Treasure: {
                    map[room] = std::make_unique<TreasureRoom>(room);
                    break;
                }
                case RoomType::Trap: {
                    map[room] = std::make_unique<TrapRoom>(room);
                    break;
                }
                case RoomType::Boss: {
                    map[room] = std::make_unique<BossRoom>(room);
                    break;
                }
                case RoomType::Spawn: {
                    break;
                }
            }
        }

        while (player.isAlive()) {
            SetCursorAtXY(0, MAX_MAP_Y + 1);
            std::cout << "Current Room: " << currentRoom << std::endl;
            std::cout << "Player: ";
            player.printStatus();

            std::cout << "Connected Rooms: " << std::endl;
            int count = 0;
            for (int connectedRoomNumber : currentRoom.connectedRooms) {
                std::cout << count << ". ";
                if (visitedRooms.find(connectedRoomNumber) != visitedRooms.end()) {
                    std::cout << roomTypeToString(rooms[connectedRoomNumber].roomType) << "  (" <<  rooms[connectedRoomNumber].position.first << ", " << rooms[connectedRoomNumber].position.second << ")" << std::endl;
                } else {
                    std::cout << "??" << std::endl;
                }
                count++;
            }

            std::cout << "Select a room to move to: ";
            int choice;
            std::cin >> choice;

            if (choice < 0 || choice >= currentRoom.connectedRooms.size()) {
                std::cout << "Invalid choice!" << std::endl;
                continue;
            }

            int nextRoomNumber = currentRoom.connectedRooms[choice];
            currentRoom = rooms[nextRoomNumber];
            visitedRooms.insert(currentRoom.roomNumber); // Mark the new room as visited

            auto it = map.find(currentRoom);
            if (it != map.end()) {
                std::visit([&](auto& roomPtr) {
                    roomPtr->enterRoom(player);
                }, it->second);
            }

            system("cls");
            floor.DrawMap(visitedRooms); // Redraw the map with updated visited rooms
        }
    }
};



std::set<std::pair<int, int>> Floor::occupiedPositions;
int main() {
    GameLogic game(1, 5, 10);
    game.MainLoop();
    return 0;
}

void testing() {
    // std::vector<Room> rooms;
    // rooms.push_back(Room(0, RoomType::Spawn, {0, 0}));
    // rooms.push_back(Room(1, RoomType::Boss, {10, 10}));
    // rooms.push_back(Room(2, RoomType::Large, {5, 5}));
    // rooms.push_back(Room(3, RoomType::Large, {15, 15}));
    // rooms.push_back(Room(4, RoomType::Large, {20, 20}));
    // rooms.push_back(Room(5, RoomType::Large, {25, 25}));
    // rooms.push_back(Room(6, RoomType::Large, {30, 30}));
    // rooms.push_back(Room(7, RoomType::Large, {35, 35}));
    // rooms.push_back(Room(8, RoomType::Large, {40, 40}));
    // rooms.push_back(Room(9, RoomType::Large, {45, 45}));
    // rooms.push_back(Room(10, RoomType::Large, {50, 50}));

    // for (auto& room : rooms) {
    //     std::cout << room << std::endl;
    // }

    // Floor floor(1, 5, 10);
    // floor.printMapAndConnections();
    // floor.DrawMap({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // std::cout << roomTypeToString(RoomType::Spawn) << std::endl;
    // std::cout << roomTypeToString(RoomType::Boss) << std::endl;
    // std::cout << roomTypeToString(RoomType::Trap) << std::endl;
    // std::cout << roomTypeToString(RoomType::Large) << std::endl;
    // std::cout << roomTypeToString(RoomType::Treasure) << std::endl;

    // std::cout << stringToRoomType("Spawn") << std::endl;
    // std::cout << stringToRoomType("Boss") << std::endl;
    // std::cout << stringToRoomType("Trap") << std::endl;
    // std::cout << stringToRoomType("Large") << std::endl;
    // std::cout << stringToRoomType("Treasure")

//////////////////////////// DLL Driver Main ////////////////////////

// DoublyLinkedList<std::unordered_map<int, std::string>> dll;

//     dll.pushBack({{1, "X"}});
//     dll.pushBack({{2, "P"}});
//     dll.pushBack({{3, "X"}});
//     dll.pushBack({{4, "E1"}});
//     dll.pushBack({{5, "E2"}});

//     while (true)
//     {
//     int choice;
//     std::cout << "Menu: " << std::endl;
//     std::cout << "1. Move Player Forward" << std::endl;
//     std::cout << "2. Move Player Backward" << std::endl;
//     std::cout << "3. Move Enemies Forward" << std::endl;
//     std::cout << "4. Move Enemies Backward" << std::endl;
//     std::cin >> choice;

//     if (choice == 1) {
//         int position = dll.getPositionFromString("P");

//         if (position != -1 && position < dll.getSize() && dll.getDataFromPosition(position + 1).begin()->second == "X") {
//             dll.setNodeAt(position + 1, {{position + 1, "P"}});
//             dll.setNodeAt(position, {{position, "X"}});
//         }
//     } else if (choice == 2) {
//         int position = dll.getPositionFromString("P");

//         if (position != -1 && position > 1 && dll.getDataFromPosition(position - 1).begin()->second == "X") {
//             dll.setNodeAt(position - 1, {{position - 1, "P"}});
//             dll.setNodeAt(position, {{position, "X"}});
//         }
//     } else if (choice == 3) {
//         for (int i = dll.getSize(); i >= 1; --i) {
//             if (dll.getDataFromPosition(i).begin()->second == "E1" || dll.getDataFromPosition(i).begin()->second == "E2") {
//                 if (i < dll.getSize() && dll.getDataFromPosition(i + 1).begin()->second == "X") {
//                     dll.setNodeAt(i + 1, {{i + 1, dll.getDataFromPosition(i).begin()->second}});
//                     dll.setNodeAt(i, {{i, "X"}});
//                 }
//             }
//         }
//     } else if (choice == 4) {
//         for (int i = 1; i <= dll.getSize(); ++i) {
//             if (dll.getDataFromPosition(i).begin()->second == "E1" || dll.getDataFromPosition(i).begin()->second == "E2") {
//                 if (i > 1 && dll.getDataFromPosition(i - 1).begin()->second == "X") {
//                     dll.setNodeAt(i - 1, {{i - 1, dll.getDataFromPosition(i).begin()->second}});
//                     dll.setNodeAt(i, {{i, "X"}});
//                 }
//             }
//         }
//     }

//     dll.printForward();

//     }
}