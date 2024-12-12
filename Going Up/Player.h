#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

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

#endif // PLAYER_H
