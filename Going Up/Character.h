#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <variant>
#include <iostream>
#include <algorithm> 
#include "Room.h"

class Character;

class AttackCard {
protected:
    std::string name;
    int damage;
public:
    AttackCard(std::string name, int damage) : name(name), damage(damage) {}
    virtual void use(Character& character) = 0;
    virtual bool operator==(const AttackCard& other) const {
        return name == other.name && damage == other.damage;
    }
    virtual ~AttackCard() = default;
};

class DoubleSlash : public AttackCard {
public:
    DoubleSlash() : AttackCard("Double Slash", 20) {}

    void use(Character& character);
};

class Bow : public AttackCard {
public:
    Bow() : AttackCard("Bow", 15) {}

    void use(Character& character);
};

using CardVariant = std::variant<DoubleSlash, Bow>;

class Character {
protected:
    int health;
    int maxHealth;
    Room currentRoom;
    std::vector<CardVariant> cards;

public:
    Character(int health, int maxHealth, Room currentRoom)
        : health(health), maxHealth(maxHealth), currentRoom(currentRoom) {}

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

    void useCard(size_t index, Character& target) {
        if (index < cards.size()) {
            std::visit([&target](auto& card) { card.use(target); }, cards[index]);
        }
    }

    virtual ~Character() = default;
};

void DoubleSlash::use(Character& character) {
    character.takeDamage(damage);
}

void Bow::use(Character& character) {
    character.takeDamage(damage);
}

#endif // CHARACTER_H
