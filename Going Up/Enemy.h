#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

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

    ~Enemy() = default;
};

#endif // ENEMY_H
