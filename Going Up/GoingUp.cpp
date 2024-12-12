#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <memory>
#include <cmath>
#include <ctime>
#include "Map.h"
#include "Utility.h"
#include "Room.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"

#define MAX_MAP_X 50
#define MAX_MAP_Y 30

class LargeRoom {
    private:
        Room room;
        std::vector<Enemy> enemies;
    
    public:
        LargeRoom(Room room) : room(room) {}

        void spawnEnemies() {
            for (int i = 0; i < 5; i++) {
                enemies.push_back(Enemy(room));
            }
        }
};

class Combat {
private:
    Player& player;  // Use references to base class for polymorphism
    Enemy& enemy;

public:
    Combat(Player& player, Enemy& enemy) : player(player), enemy(enemy) {}

    void playerTurn(AttackCard& card) {
        card.use(enemy);
    }

    void enemyTurn() {
        DoubleSlash doubleSlash;
        doubleSlash.use(player);
    }

    void start() {
        while (player.isAlive() && enemy.isAlive()) {
            DoubleSlash doubleSlash;  // This line creates a new `DoubleSlash` object each time
            playerTurn(doubleSlash); 
            player.printStatus();
            enemy.printStatus();
        }
    }
    ~Combat() = default;
};

using CardVariant = std::variant<DoubleSlash, Bow>;


int main() {
    Floor floor(1, 5, 10);
    system("cls");

    Player player(Room(0, RoomType::Spawn, {0, 0}));
    Enemy enemy(Room(1, RoomType::Boss, {1, 1}));

    CardVariant doubleSlash = DoubleSlash();
    player.addCard(doubleSlash);
    CardVariant bow = Bow();
    player.addCard(bow);

    Combat combat(player, enemy);
    combat.start();

    return 0;
}