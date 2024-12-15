#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <memory>
#include <cmath>
#include <ctime>
#include <string>
#include <variant>
#include <unordered_map>
#include "Utility.h"
#include "Room.h"
#include "Enums.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include <chrono>
#include <thread>

#define MAX_MAP_X 50
#define MAX_MAP_Y 30

class Character;

class AttackCard {
protected:
    std::string name;
    int damage;
public:
    AttackCard(std::string name, int damage) : name(name), damage(damage) {}
    virtual void use(Character& character) const = 0;
    std::string getName() const { return name; }
    virtual bool operator==(const AttackCard& other) const {
        return name == other.name && damage == other.damage;
    }
    virtual ~AttackCard() = default;
};

class SingleSlash : public AttackCard {
public:
    SingleSlash() : AttackCard("Single Slash", 1) {}

    void use(Character& character) const override;
};

class DoubleSlash : public AttackCard {
public:
    DoubleSlash() : AttackCard("Double Slash", 2) {}

    void use(Character& character) const override;
};

class Bow : public AttackCard {
public:
    Bow() : AttackCard("Bow", 2) {}

    void use(Character& character) const override;
};

class Dash : public AttackCard {
public:
    Dash() : AttackCard("Dash", 3) {}

    void use(Character& character) const override;
};

class ShadowStep : public AttackCard {
public:
    ShadowStep() : AttackCard("ShadowStep", 2) {}

    void use(Character& character) const override;
};

using CardVariant = std::variant<DoubleSlash, Bow, SingleSlash, Dash, ShadowStep>;

class Character {
protected:
    int health;
    int maxHealth;
    int direction = 1; // Defailt is facing forward (+1)
    std::vector<CardVariant> cards;
    Stack<CardVariant> attackCardsStack;

public:
    Character(int health, int maxHealth)
        : health(health), maxHealth(maxHealth) {}

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

    void addCard(const CardVariant& card) {
        cards.push_back(card);
    }

    void removeCard(const CardVariant& card) {
        cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
    }

    void addCardToAttackStack(const CardVariant& card) {
        attackCardsStack.push(card);
    }

    void removeCardFromAttackStack() {
        attackCardsStack.pop();
    }

    std::vector<CardVariant> getCards() const {
        return cards;
    }

    int getDirection() const { return direction; }
    void changeDirection() { direction = -direction; }

    virtual ~Character() = default;
};

class Player : public Character {
public:
    Player() : Character(20, 20) {
        addCard(DoubleSlash());
        addCard(SingleSlash());
        addCard(Bow());
        addCard(Dash());
        addCard(ShadowStep());
    }

    void takeDamage(int damage) override {
        health -= damage;
        if (health <= 0) {
            health = 0;
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
    Enemy(int HP, int maxHP) : Character(HP, maxHP) {
        direction = -1; // Default is facing backward (-1)
    }

    void takeDamage(int damage) override {
        health -= damage;
        if (health <= 0) {
            health = 0;
        }
    }

    void printStatus() const override {
        std::cout << "Enemy Health: " << health << "/" << maxHealth << std::endl;
    }

    bool operator==(const Enemy& other) const {
        return health == other.health && maxHealth == other.maxHealth;
    }

    ~Enemy() = default;
};

class Goblin : public Enemy {
public:
    Goblin() : Enemy(4, 4) {
        addCard(DoubleSlash());
    }

    ~Goblin() = default;
};

class SkeletonWarrior : public Enemy {
public:
    SkeletonWarrior() : Enemy(5, 5) {
        addCard(SingleSlash());
        addCard(DoubleSlash());
    }

    ~SkeletonWarrior() = default;
};

class CaveSpider : public Enemy {
public:
    CaveSpider() : Enemy(3, 3) {
        addCard(DoubleSlash());
    }

    ~CaveSpider() = default;
};

class FireImp : public Enemy {
public:
    FireImp() : Enemy(4, 4) {
        addCard(Bow());
    }

    ~FireImp() = default;
};

using EnemyVariant = std::variant<Goblin, SkeletonWarrior, CaveSpider, FireImp>;

void SingleSlash::use(Character& character) const {
    character.takeDamage(damage);
}

void DoubleSlash::use(Character& character) const {
    character.takeDamage(damage);
}

void Bow::use(Character& character) const {
    character.takeDamage(damage);
}

// In Dash Player or Enemy Moves the tiles until they reach then damage the character and stop there.
void Dash::use(Character& character) const {
    character.takeDamage(damage);
}
// In ShawdowStep Player or Enemy Moves the tiles until they reach then damage the character and then move to the opposite side of character where they were present before or you can say give damage and jump it.
void ShadowStep::use(Character& character) const {
    character.takeDamage(damage);
}

class Combat {
private:
    Player& player;
    EnemyVariant& enemy;
    DoublyLinkedList<std::unordered_map<int, std::string>> dll;
    std::unordered_map<std::string, int> cooldowns;
    // Separate cooldown maps for player and enemy
    std::unordered_map<std::string, int> playerCooldowns;
    std::unordered_map<std::string, int> enemyCooldowns;

    int turnCounter = 0; // Tracks the turn count

public:
    Combat(Player& player, EnemyVariant& enemy)
        : player(player),
          enemy(enemy) {}

    void makeFloor() {
        dll.pushBack({{1, "P"}}); // Add player
        for (int i = 2; i <= 4; ++i) dll.pushBack({{i, "X"}}); // Empty tiles
        dll.pushBack({{5, "E"}}); // Add enemy
        for (int i = 6; i <= 9; ++i) dll.pushBack({{i, "X"}}); // Empty tiles

        startCombat();
    }

    void startCombat() {
        while (true) {
            printTiles();
            player.printStatus();
            std::visit([](auto& e) { e.printStatus(); }, enemy);

            int playerPos = dll.getPositionFromString("P");
            int enemyPos = dll.getPositionFromString("E");

            // Handle turns
            if (turnCounter % 2 == 0) {
                std::cout << "Enemy's turn.\n";
                enemyTurn(playerPos, enemyPos);
                if (!player.isAlive()) {
                    std::cout << "Player defeated!\n";
                    break;
                }
            } else {
                std::cout << "Player's turn.\n";
                playerTurn(playerPos, enemyPos);
                if (!std::visit([](auto& e) { return e.isAlive(); }, enemy)) {
                    std::cout << "Enemy defeated!\n";
                    break;
                }
            }

            // Decrease cooldowns
            updateCooldowns();

            // Reinitialize cards after three turns
            if (++turnCounter % 3 == 0) {
                reinitializeCards();
            }
        }
    }

    void playerTurn(int playerPos, int enemyPos) {
        std::cout << "Player's turn: \n1. Move Forward\n2. Move Backward\n3. Use Card\n4. Change Direction\n";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            movePlayer(player.getDirection());
            break; // Move in the direction player is facing
        case 2:
            movePlayer(-player.getDirection());
            break; // Move in the opposite direction
        case 3: {
            std::cout << "Select a card to use:\n";
            auto cards = player.getCards();
            for (size_t i = 0; i < cards.size(); ++i) {
                std::cout << i << ". ";
                std::visit([](const auto& card) { std::cout << card.getName() << "\n"; }, cards[i]);
            }
            int cardChoice;
            std::cin >> cardChoice;

            if (cardChoice < 0 || cardChoice >= cards.size()) {
                std::cout << "Invalid choice!\n";
                break;
            }

            auto selectedCard = cards[cardChoice];
            std::visit([&](auto& card) {
            if (playerCooldowns[card.getName()] == 0) {
                performAttack(card, enemy, playerCooldowns); // Pass playerCooldowns
            } else {
                std::cout << "Card is on cooldown or cannot be used!\n";
            }
        }, selectedCard);

            // Remove the card after use
            player.removeCard(selectedCard);
            break;
        }
        case 4:
            player.changeDirection();
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    }

    void enemyTurn(int playerPos, int enemyPos) {
        bool attacked = false;

        // Check if the enemy has any usable cards
        std::visit([&](auto& enemyCharacter) {
            for (auto& attackCard : enemyCharacter.getCards()) {
                std::visit([&](auto& card) {
                    if (cooldowns[card.getName()] == 0) {
                        performAttack(card, player, enemyCooldowns); // Call performAttack
                        attacked = true;
                    }
                }, attackCard);

                // Remove the card after use
                if (attacked) {
                    enemyCharacter.removeCard(attackCard);
                    break;
                }
            }
        }, enemy);

        if (!attacked) {
            // Move toward or away from the player
            if (playerPos < dll.getPositionFromString("E")) {
                moveEnemyToward(playerPos, enemyPos);
            } else {
                moveEnemyAway(playerPos, enemyPos);
            }
        }
    }

    void usePlayerCard(int playerPos, int enemyPos) {
        std::cout << "Select a card to use:\n";
        auto cards = player.getCards();
        for (size_t i = 0; i < cards.size(); ++i) {
            std::cout << i << ". ";
            std::visit([](const auto& card) { std::cout << card.getName() << "\n"; }, cards[i]);
        }
        int choice;
        std::cin >> choice;

        if (choice < 0 || choice >= cards.size()) {
            std::cout << "Invalid choice!\n";
            return;
        }

        std::visit([&](auto& card) {
            if (cooldowns[card.getName()] == 0 && canAttack(player, enemyPos, playerPos, card)) {
                performAttack(card, enemy, cooldowns);
                player.removeCard(cards[choice]);
            } else {
                std::cout << "Card is on cooldown or cannot be used!\n";
            }
        }, cards[choice]);
    }

    void moveEnemyToward(int playerPos, int enemyPos) {
        int direction = playerPos < enemyPos ? -1 : 1; // Determine direction toward player
        moveEnemy(direction);
    }

    void moveEnemyAway(int playerPos, int enemyPos) {
        int direction = playerPos < enemyPos ? 1 : -1; // Determine direction away from player
        moveEnemy(direction);
    }

    void moveEnemy(int direction) {
        int position = dll.getPositionFromString("E");
        int targetPos = position + direction;

        if (position != -1 && targetPos > 0 && targetPos <= dll.getSize() &&
            dll.getDataFromPosition(targetPos).begin()->second == "X") {
            dll.setNodeAt(targetPos, {{targetPos, "E"}});
            dll.setNodeAt(position, {{position, "X"}});
        }
    }

    void movePlayer(int direction) {
        int position = dll.getPositionFromString("P");
        int targetPos = position + direction;

        if (position != -1 && targetPos > 0 && targetPos <= dll.getSize() &&
            dll.getDataFromPosition(targetPos).begin()->second == "X") {
            dll.setNodeAt(targetPos, {{targetPos, "P"}});
            dll.setNodeAt(position, {{position, "X"}});
        }
    }

    void updateCooldowns() {
        // Update cooldowns for player
        for (auto& [cardName, cooldown] : playerCooldowns) {
            if (cooldown > 0) cooldown--;
        }

        // Update cooldowns for enemy
        for (auto& [cardName, cooldown] : enemyCooldowns) {
            if (cooldown > 0) cooldown--;
        }
    }


    void reinitializeCards() {
        // Reinitialize player cards if their card list is empty and cooldowns are over
        if (player.getCards().empty()) {
            bool canReinitialize = true;
            for (const auto& [cardName, cooldown] : playerCooldowns) {
                if (cooldown > 0) {
                    canReinitialize = false;
                    break;
                }
            }

            if (canReinitialize) {
                std::cout << "Reinitializing player cards.\n";
                player.addCard(DoubleSlash());
                player.addCard(SingleSlash());
                player.addCard(Bow());
            }
        }

        // Reinitialize enemy cards if their card list is empty and cooldowns are over
        std::visit([&](auto& enemyCharacter) {
            if (enemyCharacter.getCards().empty()) {
                bool canReinitialize = true;
                for (const auto& [cardName, cooldown] : enemyCooldowns) {
                    if (cooldown > 0) {
                        canReinitialize = false;
                        break;
                    }
                }

                if (canReinitialize) {
                    std::cout << "Reinitializing enemy cards.\n";
                    enemyCharacter.addCard(DoubleSlash());
                    enemyCharacter.addCard(SingleSlash());
                    enemyCharacter.addCard(Bow());
                }
            }
        }, enemy);
    }


    void printTiles() {
        dll.printForward();
    }

    template <typename T, typename CardType>
    bool canAttack(T& character, int targetPos, int attackerPos, const CardType& card) {
        if constexpr (std::is_same_v<CardType, SingleSlash>) {
            // Single Slash is effective if the target is adjacent AND the player/enemy is directing towards the target
            if (std::abs(targetPos - attackerPos) == 1) {
                if (character.getDirection() == 1) {
                    // Facing forward: target must be one tile ahead
                    return targetPos > attackerPos;
                } else {
                    // Facing backward: target must be one tile behind
                    return targetPos < attackerPos;
                }
            }
            return false;
        } else if constexpr (std::is_same_v<CardType, DoubleSlash>) {
            // DoubleSlash can attack if the target is adjacent to the attacker
            return std::abs(targetPos - attackerPos) == 1;
        } else if constexpr (std::is_same_v<CardType, Bow>) {
            // The player is directing towards the enemy
            if (character.getDirection() == 1) {
                // Forward direction, attacker must be behind target within range
                return (targetPos > attackerPos);
            } else {
                // Backward direction, attacker must be ahead of target within range
                return (targetPos < attackerPos);
            }
        } else if constexpr (std::is_same_v<CardType, Dash>) {
            return std::abs(targetPos - attackerPos) > 1;
        } else if constexpr (std::is_same_v<CardType, ShadowStep>) {
            // ShadowStep can attack if the target is within a certain range (e.g., 5 tiles) and in the direction the character is facing
            int range = 5;
            if (character.getDirection() == 1) {
                return targetPos > attackerPos && targetPos <= attackerPos + range;
            } else {
                return targetPos < attackerPos && targetPos >= attackerPos - range;
            }
        } else {
            // Default case for other card types
            return false;
        }
    }


    template <typename T, typename CardType>
    void performAttack(const CardType& card, T& target, std::unordered_map<std::string, int>& cooldowns) {
        std::cout << card.getName() << " is used!\n";

        bool isInRange = false;

        if constexpr (std::is_same_v<CardType, Dash>) {
            int attackerPos = dll.getPositionFromString("P");
            int targetPos = dll.getPositionFromString("E");

            // Move attacker toward target until 1 tile away
            while (std::abs(attackerPos - targetPos) > 1) {
                int direction = (targetPos > attackerPos) ? 1 : -1;
                int newPos = attackerPos + direction;

                if (dll.getDataFromPosition(newPos).begin()->second == "X") {
                    // Update positions in the linked list
                    dll.setNodeAt(newPos, {{newPos, "P"}});
                    dll.setNodeAt(attackerPos, {{attackerPos, "X"}});
                    attackerPos = newPos;

                    printTiles(); // Optional: Print tiles after each move for debugging
                } else {
                    break; // Stop if movement is blocked
                }
            }

            // Check if Dash card can attack the target
            if (std::abs(attackerPos - targetPos) == 1) {
                isInRange = true; // Target is in range for Dash
            }
        } else {
            // Handle other cards (like Single Slash, Bow, etc.)
            int attackerPos = dll.getPositionFromString("P");
            int targetPos = dll.getPositionFromString("E");

            // Check if the card can attack the target
            if (canAttack(player, targetPos, attackerPos, card)) {
                isInRange = true; // Target is in range
            }
        }

        // Apply effect if in range
        if (isInRange) {
            if constexpr (std::is_same_v<T, std::variant<Goblin, SkeletonWarrior, CaveSpider, FireImp>>) {
                // Use std::visit to extract the actual type and invoke the card's use function
                std::visit([&](auto& actualTarget) {
                    card.use(actualTarget);
                }, target);
            } else {
                // Directly use the card on the target if it's not a variant
                card.use(target);
            }
        } else {
            // Print message if the card is ineffective
            std::cout << card.getName() << " was ineffective (enemy out of range).\n";
        }

        // Apply cooldown to the correct map
        cooldowns[card.getName()] = 3; // Example cooldown
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
        srand(static_cast<unsigned>(time(nullptr)));
        bool validMap = false;

        while (!validMap) {
            rooms.clear();
            occupiedPositions.clear();

            int roomCount = rand() % (maxRooms - minRooms + 1) + minRooms;
            std::vector<std::pair<int, int>> bossPositions = {
                {MAX_MAP_X, MAX_MAP_Y}, {MAX_MAP_X, 0}, {0, MAX_MAP_Y}
            };
            std::pair<int, int> bossPosition = bossPositions[rand() % bossPositions.size()];

            Room spawn(0, RoomType::Spawn, {0, 0});
            Room boss(1, RoomType::Boss, bossPosition);
            rooms.push_back(spawn);
            rooms.push_back(boss);

            for (int i = 2; i < roomCount; ++i) {
                RoomType randomType = static_cast<RoomType>(1 + rand() % 3);
                rooms.emplace_back(Room(i, randomType, RandomCoord()));
            }

            connectRooms();
            validMap = validateMap();

            if (!validMap) {
                std::cout << "Map generation failed, regenerating...\n";
            }
        }

        std::cout << "Valid map generated with " << rooms.size() << " rooms.\n";
    }

    bool validateMap() const {
        for (const auto& room : rooms) {
            if (room.roomType == RoomType::Boss) continue;
            if (room.connectedRooms.empty()) {
                std::cout << "Room " << room.roomNumber << " has no connections!\n";
                return false;
            }
        }
        return true;
    }

    void connectRooms() {
        for (auto& room : rooms) {
            if (room.roomType == RoomType::Boss || room.roomType == RoomType::Spawn) continue;

            int minConnections = 2;
            int maxConnections = 3;

            std::vector<Room*> potentialConnections;
            for (auto& other : rooms) {
                if (&room != &other && other.roomType != RoomType::Boss) {
                    potentialConnections.push_back(&other);
                }
            }

            std::sort(potentialConnections.begin(), potentialConnections.end(),
                    [&room](const Room* a, const Room* b) {
                        return room.distanceTo(*a) < room.distanceTo(*b);
                    });

            int connectionsToAdd = rand() % (maxConnections - minConnections + 1) + minConnections;
            for (int i = 0; i < connectionsToAdd && i < potentialConnections.size(); ++i) {
                Room* targetRoom = potentialConnections[i];
                if (std::find(room.connectedRooms.begin(), room.connectedRooms.end(), targetRoom->roomNumber) == room.connectedRooms.end()) {
                    room.connectedRooms.push_back(targetRoom->roomNumber);
                    targetRoom->connectedRooms.push_back(room.roomNumber);
                }
            }
        }

        ensureGraphConnectivity();
        printMapAndConnections();
    }

    void ensureGraphConnectivity() {
        std::vector<bool> visited(rooms.size(), false);
        Queue toVisit;
        toVisit.enqueue(0);

        while (!toVisit.empty()) {
            int current = toVisit.front();
            toVisit.dequeue();
            if (visited[current]) continue;

            visited[current] = true;
            for (int connectedRoom : rooms[current].connectedRooms) {
                if (!visited[connectedRoom]) {
                    toVisit.enqueue(connectedRoom);
                }
            }
        }

        for (size_t i = 0; i < rooms.size(); ++i) {
            if (!visited[i] && rooms[i].roomType != RoomType::Boss) {
                int nearestVisited = -1;
                double minDistance = std::numeric_limits<double>::infinity();

                for (size_t j = 0; j < rooms.size(); ++j) {
                    if (visited[j]) {
                        double distance = rooms[i].distanceTo(rooms[j]);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestVisited = j;
                        }
                    }
                }

                if (nearestVisited != -1) {
                    rooms[i].connectedRooms.push_back(nearestVisited);
                    rooms[nearestVisited].connectedRooms.push_back(i);
                    visited[i] = true;
                }
            }
        }

        // Ensure no room (except the spawn and boss rooms) has fewer than 2 connections
        for (auto& room : rooms) {
            if (room.roomType != RoomType::Boss && room.roomType != RoomType::Spawn && room.connectedRooms.size() < 2) {
                int nearestRoom = -1;
                double minDistance = std::numeric_limits<double>::infinity();

                for (auto& other : rooms) {
                    if (&room != &other && other.roomType != RoomType::Boss && other.roomType != RoomType::Spawn) {
                        double distance = room.distanceTo(other);
                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestRoom = other.roomNumber;
                        }
                    }
                }

                if (nearestRoom != -1) {
                    room.connectedRooms.push_back(nearestRoom);
                    rooms[nearestRoom].connectedRooms.push_back(room.roomNumber);
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
        do {
            x = rand() % (MAX_MAP_X + 1);
            y = rand() % (MAX_MAP_Y + 1);
        } while (occupiedPositions.find({x, y}) != occupiedPositions.end());

        occupiedPositions.insert({x, y});
        return {x, y};
    }

    Room getBossRoom() const {
        for (const auto& room : rooms) {
            if (room.roomType == RoomType::Boss) return room;
        }
        throw std::runtime_error("Boss room not found");
    }

    Room getSpawnRoom() const {
        for (const auto& room : rooms) {
            if (room.roomType == RoomType::Spawn) return room;
        }
        throw std::runtime_error("Spawn room not found");
    }

    void DrawMap(const std::set<int>& visitedRooms) const {
        for (const auto& room : rooms) {
            if (visitedRooms.find(room.roomNumber) != visitedRooms.end()) {
                std::cout << "Room " << room.roomNumber << " (" << room.position.first << ", " << room.position.second << "): " << roomTypeToString(room.roomType) << std::endl;
            } else {
                std::cout << "Room " << room.roomNumber << " (" << room.position.first << ", " << room.position.second << "): ??" << std::endl;
            }
        }
    }

    void printMapAndConnections() const {
        for (const auto& room : rooms) {
            std::cout << "Room " << room.roomNumber << " connections: ";
            for (const auto& connected : room.connectedRooms) {
                std::cout << connected << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Define static member
std::set<std::pair<int, int>> Floor::occupiedPositions;


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
    EnemyVariant enemy;

    EnemyVariant generateRandomEnemy() {
        int randomIndex = rand() % 4;
        switch (randomIndex) {
            case 0: return EnemyVariant{std::in_place_type<Goblin>};
            case 1: return EnemyVariant{std::in_place_type<SkeletonWarrior>};
            case 2: return EnemyVariant{std::in_place_type<CaveSpider>};
            case 3: return EnemyVariant{std::in_place_type<FireImp>};
            default: return EnemyVariant{std::in_place_type<Goblin>};
        }
    }

public:
    LargeRoom(const Room& room, Player& player)
        : room(room), enemy(generateRandomEnemy()), combat(player, enemy) 
    {}

    void enterRoom(Player& player) override {
        if (isCleared) {
            std::cout << "You have already cleared this room!" << std::endl;
            Sleep(2000);
            return;
        } else {
            Sleep(2000);
            combat.makeFloor();
            combat.startCombat();
            isCleared = true;
            std::cout << "Room is cleared!" << std::endl;
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
    std::unordered_map<int, RoomVariant> map;

public:
    GameLogic(int floorNumber, int minRooms, int maxRooms)
        : floor(floorNumber, minRooms, maxRooms), currentRoom(floor.getSpawnRoom()), player() {
        visitedRooms.insert(currentRoom.roomNumber); // Mark the spawn room as visited
    }

    void MainLoop() {
        system("cls");
        floor.DrawMap(visitedRooms); // Pass visited rooms to DrawMap
        std::vector<Room> rooms = floor.getRooms();

        for (const auto& room : rooms) {
            switch (room.roomType) {
                case RoomType::Large: {
                    map[room.roomNumber] = std::make_unique<LargeRoom>(room, player);
                    break;
                }
                case RoomType::Treasure: {
                    map[room.roomNumber] = std::make_unique<TreasureRoom>(room);
                    break;
                }
                case RoomType::Trap: {
                    map[room.roomNumber] = std::make_unique<TrapRoom>(room);
                    break;
                }
                case RoomType::Boss: {
                    map[room.roomNumber] = std::make_unique<BossRoom>(room);
                    break;
                }
                case RoomType::Spawn: {
                    break;
                }
            }
        }

        std::cout << "Hashmap created" << std::endl;

        while (player.isAlive()) {
            SetCursorAtXY(0, MAX_MAP_Y + 1);
            std::cout << "Current Room: " << currentRoom.roomNumber << std::endl;
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

            auto it = map.find(currentRoom.roomNumber);
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