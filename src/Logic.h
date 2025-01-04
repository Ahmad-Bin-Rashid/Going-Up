#pragma once

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
#include <chrono>
#include <thread>
#include "../Utilities/CustomNamespace.h"
#include "../Utilities/Utility.h"
#include "Room.h"
#include "../Utilities/Enums.h"
#include "../DataStructures/DoublyLinkedList.h"
#include "../DataStructures/Stack.h"
#include "../DataStructures/Queue.h"
#include "UI.h"

#define MAX_MAP_X 80
#define MAX_MAP_Y 28
bool IS_ANIMATION_SHOWN = false;
int CONSOLE_X = 130;
int CONSOLE_Y = 35;
void RESET_CONSOLE_XY() {CONSOLE_X = 119; CONSOLE_Y = 35;}
ScreenBuffer screen(158,43);

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
    ShadowStep() : AttackCard("Shadow Step", 3) {}

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

    std::string getName() const {
        std::string name = typeid(*this).name();
        size_t pos = name.find("class ");
        if (pos != std::string::npos) { 
            name.replace(pos, 6, "");
        }
        return name;
    }

    virtual void takeDamage(int damage) {
        health -= damage;
    }

    virtual void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    int getHealth() const {
        return health;
    }

    int getMaxHealth() const {
        return maxHealth;
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

    std::vector<CardVariant> getCards() const {
        return cards;
    }

    int getDirection() const { return direction; }
    void changeDirection() { direction = -direction; }

    void addCardToAttackStack(const CardVariant& card) {
        attackCardsStack.push(card);
    }

    void removeCardFromAttackStack() {
        if (!attackCardsStack.empty()) {
            attackCardsStack.pop();
        }
    }

    CardVariant topCardFromAttackStack() {
        return attackCardsStack.top();
    }

    bool attackCardsStackEmpty() const {
        return attackCardsStack.empty();
    }

    std::vector<CardVariant> getAttackStack() const {
        std::vector<CardVariant> attackStack;
        Stack<CardVariant> tempStack(attackCardsStack);
        while (!tempStack.empty()) {
            attackStack.push_back(tempStack.top());
            tempStack.pop();
        }
        return attackStack;
    }

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

    void damage(int amount) {
        health -= amount;
        if (health <= 0) {
            health = 0;
        }
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

    bool operator==(const Enemy& other) const {
        return health == other.health && maxHealth == other.maxHealth;
    }

    ~Enemy() = default;
};







class Goblin : public Enemy {
public:
    Goblin() : Enemy(4, 4) {
        addCard(DoubleSlash());
        addCard(SingleSlash());
    }

    ~Goblin() = default;
};

class SkeletonWarrior : public Enemy {
public:
    SkeletonWarrior() : Enemy(5, 5) {
        addCard(SingleSlash());
        addCard(Dash());
    }

    ~SkeletonWarrior() = default;
};

class CaveSpider : public Enemy {
public:
    CaveSpider() : Enemy(3, 3) {
        addCard(DoubleSlash());
        addCard(Bow());
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






class InfernalWarlord : public Enemy {
public:
    InfernalWarlord() : Enemy(10, 10) {
        addCard(DoubleSlash());
        addCard(Dash());
        addCard(SingleSlash());
        addCard(Bow());
        addCard(ShadowStep());
    }

    ~InfernalWarlord() = default;
};

using EnemyVariant = std::variant<Goblin, SkeletonWarrior, CaveSpider, FireImp, InfernalWarlord>;

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

// In Shadow Step Player or Enemy Moves the tiles until they reach then damage the character and then jump from enemy, goes to the opposite side and damage the character.
void ShadowStep::use(Character& character) const {
    character.takeDamage(damage);
}


// Define the possible player moves
enum class Moves {
    AddCard,
    ExecuteStack,
    ChangeDirection,
    MoveAway,
    MoveTowards,
    Pass,
    Cooldown
};

// Define a struct to associate moves with probabilities
struct MoveProbability {
    Moves move;
    double probability;
};

// Define a struct to represent the game state
struct GameState {
    int playerHealth;
    int enemyHealth;
    int playerDirection; // 1 for right, -1 for left
    int enemyDirection;  // 1 for right, -1 for left
    int distance;
    bool playerCooldown;
    bool enemyCooldown;
    int numPlayerCardsAvailable;
    std::vector<CardVariant> playerAttackStack;
};

// Function to predict player's move
Moves predictPlayerMove(const GameState& state) {
    // Initialize probabilities for actions
    double addCardProb = 0.0;
    double executeStackProb = 0.0;
    double changeDirectionProb = 0.0;
    double moveTowardsProb = 0.0;
    double moveAwayProb = 0.0;

    // Determine action probabilities based on state
    bool hasDistanceCard = std::any_of(state.playerAttackStack.begin(), state.playerAttackStack.end(), [](const CardVariant &card) { 
        return std::visit([](const auto &c) { return c.getName() != "Single Slash" && c.getName() != "Double Slash"; }, card); 
    });
    bool hasCloseCard = std::any_of(state.playerAttackStack.begin(), state.playerAttackStack.end(), [](const CardVariant &card) { 
        return std::visit([](const auto &c) { return c.getName() == "Single Slash" || c.getName() == "Double Slash"; }, card); 
    });

    if (state.playerAttackStack.empty() && state.numPlayerCardsAvailable != 0) {
        if (state.playerHealth < state.enemyHealth) {
            // Aggressive behavior
            if (hasDistanceCard) {
                addCardProb = 0.5;
                moveTowardsProb = 0.5; // Move intelligently to use distance cards
            } else if (hasCloseCard) {
                addCardProb = 0.7;
                moveTowardsProb = 0.3;
            }
        } else {
            if (hasDistanceCard) {
                addCardProb = 0.4;
                moveTowardsProb = 0.3;
            }
            if (hasCloseCard) {
                addCardProb += 0.3;
                executeStackProb = 0.3;
            }
        }
    } else if (state.distance <= 2 && hasCloseCard) {
        // Favor close-range attacks
        executeStackProb = 0.7;
        moveTowardsProb = 0.3;
    }

    if (state.enemyCooldown > 0) {
        // Enemy is on cooldown
        if (state.playerCooldown == 0) {
            moveTowardsProb = 0.6; // Player moves toward enemy
        } else {
            moveAwayProb = 0.8; // Player moves away to gain distance
        }
    }

    if (state.numPlayerCardsAvailable <= 2) {
        moveTowardsProb = 0.7; // Player moves toward enemy for attack
        addCardProb = 0.3;
    }

    // Normalize probabilities
    double total = addCardProb + executeStackProb + changeDirectionProb + moveTowardsProb + moveAwayProb;
    if (total > 0) {
        addCardProb /= total;
        executeStackProb /= total;
        changeDirectionProb /= total;
        moveTowardsProb /= total;
        moveAwayProb /= total;
    }

    std::vector<double> probabilities = {addCardProb, executeStackProb, moveTowardsProb, moveAwayProb};
    std::vector<Moves> moves = {Moves::AddCard, Moves::ExecuteStack, Moves::MoveTowards, Moves::MoveAway};

    std::vector<std::pair<double, Moves>> probMoves;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        probMoves.push_back({probabilities[i], moves[i]});
    }
    std::sort(probMoves.rbegin(), probMoves.rend(), [](const std::pair<double, Moves>& a, const std::pair<double, Moves>& b) {
        return a.first > b.first;
    });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist({
        probMoves[0].first, 
        probMoves[1].first, 
        probMoves[2].first
    });

    // Return the randomly selected move
    int choice = dist(gen);
    return probMoves[choice].second;
}



class Combat {
private:
    Player& player;
    EnemyVariant& enemy;
    DoublyLinkedList<std::unordered_map<int, std::string>> dll;

    int playerTurnCounter = 0;
    int enemyTurnCounter = 0; 

public:
    Combat(Player& player, EnemyVariant& enemy)
        : player(player),
          enemy(enemy) {}

    void makeFloor() {
        dll.pushBack({{1, "P"}});
        for (int i = 2; i < 4; ++i) dll.pushBack({{i, "X"}});
        dll.pushBack({{4, "E"}});
        dll.pushBack({{5, "X"}}); 
    }

    bool startCombat() {
        int playerPos;
        int enemyPos;
        while (player.isAlive() && std::visit([](auto& e) { return e.isAlive(); }, enemy)) {
            std::vector<std::string> cardNames;
            for (const auto& card : player.getCards()) {
                cardNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
            }
            screen.updateAvailableCards(cardNames);
            screen.updateEnemyHP(std::visit([](auto& e) { return e.getHealth(); }, enemy), std::visit([](auto& e) { return e.getMaxHealth(); }, enemy));
            screen.updatePlayerHP(player.getHealth(), player.getMaxHealth());
            printTiles(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));

            playerPos = dll.getPositionFromString("P");
            enemyPos = dll.getPositionFromString("E");

            screen.EnemyTurn();

            // Enemy's turn
            if (std::visit([](auto& e) { return e.getCards().empty(); }, enemy)) {
                enemyTurnCounter++;
                Sleep(1000);
                if (enemyTurnCounter >= 5) {
                    enemyTurnCounter = 0;
                    int numCards = rand() % 2 + 1;
                    for (int i = 0; i < numCards; ++i) {
                        std::visit([&](auto& e) {
                            int cardType = rand() % 5;
                            CardVariant newCard;
                            switch (cardType) {
                                case 0: newCard = DoubleSlash(); break;
                                case 1: newCard = Bow(); break;
                                case 2: newCard = SingleSlash(); break;
                                case 3: newCard = Dash(); break;
                                case 4: newCard = ShadowStep(); break;
                            }
                            e.addCard(newCard);
                        }, enemy);
                    }
                }
            }
            enemyTurn(playerPos, enemyPos);
            {
                std::vector<std::string> attackStackNames;
                for (const auto& card : std::visit([](auto& e) { return e.getAttackStack(); }, enemy)) {
                    attackStackNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
                }
                screen.updateEnemyAttackStack(attackStackNames);
            }
            screen.updatePlayerHP(player.getHealth(), player.getMaxHealth());
            printTiles(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));
            if (!player.isAlive()) {
                ct::p | "Player defeated!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                return false;
            }
            playerPos = dll.getPositionFromString("P");
            enemyPos = dll.getPositionFromString("E");
            
            // Player's turn
            playerTurn(playerPos, enemyPos);
            playerTurnCounter++;
            if (playerTurnCounter >= 5 && player.getCards().empty()) {
                playerTurnCounter = 0; 
                int numCards = rand() % 4 + 1;
                for (int i = 0; i < numCards; ++i) {
                    int cardType = rand() % 5;
                    CardVariant newCard;
                    switch (cardType) {
                        case 0: newCard = DoubleSlash(); break;
                        case 1: newCard = Bow(); break;
                        case 2: newCard = SingleSlash(); break;
                        case 3: newCard = Dash(); break;
                        case 4: newCard = ShadowStep(); break;
                    }
                    player.addCard(newCard);
                }
            }
            {
                std::vector<std::string> attackStackNames;
                for (const auto& card : player.getAttackStack()) {
                    attackStackNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
                }
                screen.updatePlayerAttackStack(attackStackNames);
            }
            if (!std::visit([](auto& e) { return e.isAlive(); }, enemy)) {
                printTiles(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));
                ct::p | "Enemy defeated!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                return true;
            }
            screen.updateEnemyHP(std::visit([](auto& e) { return e.getHealth(); }, enemy), std::visit([](auto& e) { return e.getMaxHealth(); }, enemy));
        }
        return false;
    }

    void playerTurn(int playerPos, int enemyPos) {
        bool turnEnded = false;
        std::vector<std::wstring> tips = {
            L" ≈ Use cards Wisely!",
            L" ≈ Monitor your Health!",
            L" ≈ Plan moves Ahead!",
            L" ≈ Monitor enemy Health!",
            L" ≈ Monitor enemy Stack!",
            L" ≈ Manage attack Stack!"
        };

        std::shuffle(tips.begin(), tips.end(), std::default_random_engine(std::random_device{}()));

        while (!turnEnded) {
            int choice;
            screen.clearConsole();
            screen.setString(130, 36, L"-> Your Turn: ", WBOLD_MAGENTA);
            screen.setString(130, 38, L"-> Tips: ", WBOLD_MAGENTA);
            screen.setString(130, 39, std::wstring(tips[0].begin(), tips[0].end()), WBOLD_MAGENTA);
            screen.setString(130, 40, std::wstring(tips[1].begin(), tips[1].end()), WBOLD_MAGENTA);
            screen.setString(130, 41, std::wstring(tips[2].begin(), tips[2].end()), WBOLD_MAGENTA);
            screen.print();
            choice = screen.TakeLargeRoomTurn();

            switch (choice) {
            case 1:
                movePlayer(player.getDirection());
                turnEnded = true;
                break; 
            case 2:
                movePlayer(-player.getDirection());
                turnEnded = true;
                break;
            case 3:
                addCardToAttackStack();
                turnEnded = true;
                break;
            case 4:
                if (!player.attackCardsStackEmpty()) {
                    executeAttackStack(playerPos, enemyPos);
                } else {
                }
                turnEnded = true;
                break; 
            case 5:
                player.changeDirection();
                turnEnded = true;
                break;
            case 6:
                turnEnded = true;
                break;
            default:
                ct::p | "Invalid choice!\n";
                break;
            }
        }
    }

    void addCardToAttackStack() {
        std::string consoleText = "";
        auto cards = player.getCards();
        screen.clearConsole();
        screen.setString(130, 36, L"-> Choose Card: ", WBOLD_MAGENTA);
        screen.print();

        int cardChoice;
        while (true) {
            cardChoice = screen.TakeLargeRoomTurn();
            if (cardChoice == -1) break; 
            cardChoice--;
            if (cardChoice < 0 || cardChoice >= cards.size() || cardChoice == -2) {
                consoleText = "Invalid card choice!";
                continue;
            } else {
                auto selectedCard = cards[cardChoice];
                player.addCardToAttackStack(selectedCard);
                player.removeCard(cards[cardChoice]);
                consoleText = "Player added " + std::visit([](const auto& card) { return card.getName(); }, selectedCard) + " to the attack stack.";
                break; 
            }
        }
    }

    void executeAttackStack(int playerPos, int enemyPos) {
        while (!player.attackCardsStackEmpty()) {
            auto attackCard = player.topCardFromAttackStack();
            player.removeCardFromAttackStack();

            std::visit([&](auto& card) {
                if (canAttack(player, enemyPos, playerPos, card)) {
                    performAttack(card, enemy);
                    updateTilesOnScreen(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));
                }
            }, attackCard);
        }
    }

    Moves decideEnemyMove(const GameState& state) {
        Moves predictedMoves = predictPlayerMove(state);

        // Counter the predicted player move
        switch (predictedMoves) {
            case Moves::MoveTowards:
                if (state.distance < 2) {
                    return Moves::ExecuteStack;
                } else {
                    return Moves::MoveAway;
                }
            case Moves::MoveAway:
                return Moves::MoveTowards;
            case Moves::AddCard:
                if (!std::visit([](auto& e) { return e.getAttackStack().empty(); }, enemy)) {
                    return Moves::ExecuteStack;
                } else if (!std::visit([](auto& e) { return e.getCards().empty(); }, enemy)) {
                    return Moves::AddCard;
                } else if (state.enemyCooldown) {
                    return Moves::Cooldown;
                } else {
                    return Moves::MoveTowards;
                }
            case Moves::ExecuteStack:
                if (!std::visit([](auto& e) { return e.getAttackStack().empty(); }, enemy)) {
                    return Moves::ExecuteStack;
                } else if (!std::visit([](auto& e) { return e.getCards().empty(); }, enemy)) {
                    return Moves::AddCard;
                } else if (state.enemyCooldown) {
                    return Moves::Cooldown;
                } else {
                    return Moves::MoveTowards;
                }
            case Moves::ChangeDirection:
                return Moves::MoveAway;
            default:
                return Moves::MoveTowards;
        }
    }

    void enemyTurn(int playerPos, int enemyPos) {
        bool turnEnded = false;
        std::string enemyTurn = "";

        while (!turnEnded) {
            std::visit([&](auto& enemyCharacter) {
                GameState gameState = {
                    player.getHealth(),
                    enemyCharacter.getHealth(),
                    player.getDirection(),
                    enemyCharacter.getDirection(),
                    std::abs(playerPos - enemyPos),
                    player.getCards().empty(),
                    enemyCharacter.getCards().empty(),
                    static_cast<int>(player.getCards().size()),
                    player.getAttackStack()
                };

                Moves enemyMove = decideEnemyMove(gameState);

                switch (enemyMove) {
                    case Moves::MoveTowards:
                        moveEnemyToward(playerPos, enemyPos);
                        enemyTurn = "-> Enemy moved!";
                        break;
                    case Moves::MoveAway:
                        moveEnemyAway(playerPos, enemyPos);
                        enemyTurn = "-> Enemy moved!";
                        break;
                    case Moves::AddCard:
                        if (!enemyCharacter.getCards().empty()) {
                            int cardIndex = rand() % enemyCharacter.getCards().size();
                            auto selectedCard = enemyCharacter.getCards()[cardIndex];
                            enemyCharacter.addCardToAttackStack(selectedCard);
                            enemyCharacter.removeCard(selectedCard);
                            enemyTurn = "-> Enemy adds " + std::visit([](const auto& card) { return card.getName(); }, selectedCard);
                        }
                        break;
                    case Moves::ExecuteStack:
                        if (!enemyCharacter.attackCardsStackEmpty()) {
                            auto attackCard = enemyCharacter.topCardFromAttackStack();
                            std::visit([&](auto& card) {
                                if ((playerPos > enemyPos && enemyCharacter.getDirection() != 1) ||
                                    (playerPos < enemyPos && enemyCharacter.getDirection() != -1)) {
                                    enemyCharacter.changeDirection();
                                }
                                else if (canAttack(enemyCharacter, playerPos, enemyPos, card)) {
                                    performAttack(card, player);
                                    enemyCharacter.removeCardFromAttackStack();
                                    enemyTurn = "-> Enemy used " + card.getName() + "!";
                                } else {
                                    moveEnemyToward(playerPos, enemyPos);
                                    enemyTurn = "-> Enemy's Moved!";
                                }
                            }, attackCard);
                        }
                        break;
                    case Moves::ChangeDirection:
                        enemyCharacter.changeDirection();
                        enemyTurn = "-> Enemy changed direction!";
                        break;
                    case Moves::Cooldown:
                        moveEnemyAway(playerPos, enemyPos);
                        enemyTurn = "Cooldown";
                    default:
                        break;
                }
            }, enemy);
            turnEnded = true;
            screen.EnemyTurnUpdate(enemyTurn);

            // Update the enemy attack stack on the screen
            std::vector<std::string> attackStackNames;
            for (const auto& card : std::visit([](auto& e) { return e.getAttackStack(); }, enemy)) {
                attackStackNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
            }
            screen.updateEnemyAttackStack(attackStackNames);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
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
        updateTilesOnScreen(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));
    }

    void movePlayer(int direction) {
        int position = dll.getPositionFromString("P");
        int targetPos = position + direction;

        if (position != -1 && targetPos > 0 && targetPos <= dll.getSize() &&
            dll.getDataFromPosition(targetPos).begin()->second == "X") {
            dll.setNodeAt(targetPos, {{targetPos, "P"}});
            dll.setNodeAt(position, {{position, "X"}});
        }
        updateTilesOnScreen(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));
    }

    void printTiles(int playerDirection, int enemyDirection) {
        system("cls");
        std::vector<std::string> tiles = dll.getValues();
        screen.updateTiles(tiles, playerDirection, enemyDirection);
        screen.updatePlayerHP(player.getHealth(), player.getMaxHealth());
        screen.updateEnemyHP(std::visit([](auto& e) { return e.getHealth(); }, enemy), std::visit([](auto& e) { return e.getMaxHealth(); }, enemy));
        {
            std::vector<std::string> attackStackNames;
            for (const auto& card : player.getAttackStack()) {
                attackStackNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
            }
            screen.updatePlayerAttackStack(attackStackNames);
        }
        {
        std::vector<std::string> attackStackNames;
        for (const auto& card : std::visit([](auto& e) { return e.getAttackStack(); }, enemy)) {
            attackStackNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
        }
        screen.updateEnemyAttackStack(attackStackNames);
        }
        std::vector<std::string> cardNames;
        for (const auto& card : player.getCards()) {
            cardNames.push_back(std::visit([](const auto& c) { return c.getName(); }, card));
        }
        screen.updateAvailableCards(cardNames);
        screen.updateTiles(tiles, playerDirection, enemyDirection);
        screen.print();
    }

    void updateTilesOnScreen(int playerDirection, int enemyDirection) {
        std::vector<std::string> tiles = dll.getValues();
        screen.updateTiles(tiles, playerDirection, enemyDirection);
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
            // Bow can attack if the target is in the direction the character is facing
            if (character.getDirection() == 1) {
                return targetPos > attackerPos;
            } else {
                return targetPos < attackerPos;
            }
        } else if constexpr (std::is_same_v<CardType, Dash>) {
            // Dash can attack if the target is in the direction the character is facing
            if (character.getDirection() == 1) {
                return targetPos > attackerPos;
            } else {
                return targetPos < attackerPos;
            }
        } else if constexpr (std::is_same_v<CardType, ShadowStep>) {
            // ShadowStep can attack if the enemy is not at the first or last tile
            return targetPos != 1 && targetPos != dll.getSize();
        } else {
            // Default case for other card types
            return false;
        }
    }

    template <typename T, typename CardType>
    void performAttack(const CardType& card, T& target) {
        std::string attackText = "";

        bool isInRange = false;

        if constexpr (std::is_same_v<CardType, SingleSlash>) {
            if constexpr (std::is_same_v<T, Player>) {
                screen.SingleSlashAnimation("E");
            } else {
                screen.SingleSlashAnimation("P");
            }
        } else if constexpr (std::is_same_v<CardType, DoubleSlash>) {
            if constexpr (std::is_same_v<T, Player>) {
                screen.DoubleSlashAnimation("E");
            } else {
                screen.DoubleSlashAnimation("P");
            }
        } else if constexpr (std::is_same_v<CardType, Bow>) {
            if constexpr (std::is_same_v<T, Player>) {
                screen.BowAnimation("E");
            } else {
                screen.BowAnimation("P");
            }
        } else if constexpr (std::is_same_v<CardType, Dash>) {
            if constexpr (std::is_same_v<T, Player>) {
                screen.DashAnimation("E");
            } else {
                screen.DashAnimation("P");
            }
        } else if constexpr (std::is_same_v<CardType, ShadowStep>) {
            if constexpr (std::is_same_v<T, Player>) {
                screen.ShadowStepAnimation("E");
            } else {
                screen.ShadowStepAnimation("P");
            }
        }

        if constexpr (std::is_same_v<CardType, Dash> || std::is_same_v<CardType, ShadowStep>) {
            int attackerPos = dll.getPositionFromString("P");
            int targetPos = dll.getPositionFromString("E");

            while (std::abs(attackerPos - targetPos) > 1) {
                int direction = (targetPos > attackerPos) ? 1 : -1;
                int newPos = attackerPos + direction;

                if (dll.getDataFromPosition(newPos).begin()->second == "X") {
                    dll.setNodeAt(newPos, {{newPos, "P"}});
                    dll.setNodeAt(attackerPos, {{attackerPos, "X"}});
                    attackerPos = newPos;
                } else {
                    break;
                }
            }

            if (std::abs(attackerPos - targetPos) == 1) {
                isInRange = true; 
            }

            if constexpr (std::is_same_v<CardType, ShadowStep>) {
                if (targetPos != 1 && targetPos != dll.getSize()) {
                    int jumpPos = targetPos + 1;
                    if (dll.getDataFromPosition(jumpPos).begin()->second == "X") {
                        dll.setNodeAt(jumpPos, {{jumpPos, "P"}});
                        dll.setNodeAt(attackerPos, {{attackerPos, "X"}});
                        attackText = "Player used ShadowStep and jumped to position " + std::to_string(jumpPos) + ".";
                        updateTilesOnScreen(player.getDirection(), std::visit([](auto& e) { return e.getDirection(); }, enemy));
                    } else {
                        attackText = "ShadowStep cannot be used! Target position is occupied.";
                    }
                } else {
                    attackText = "ShadowStep cannot be used! Enemy is at the edge.";
                }
            }
        } else {
            int attackerPos = dll.getPositionFromString("P");
            int targetPos = dll.getPositionFromString("E");

            if (canAttack(player, targetPos, attackerPos, card)) {
                isInRange = true;
            }
        }

        if (isInRange) {

            if constexpr (std::is_same_v<T, std::variant<Goblin, SkeletonWarrior, CaveSpider, FireImp, InfernalWarlord>>) {
                std::visit([&](auto& actualTarget) {
                    card.use(actualTarget);
                }, target);
                attackText = card.getName() + " was used by " + (std::is_same_v<T, Player> ? "Player" : "Enemy") + "!";
            } else {
                card.use(target);
                attackText = card.getName() + " was used by " + (std::is_same_v<T, Player> ? "Player" : "Enemy") + "!";
            }
        } else {
            attackText = card.getName() + " was ineffective (enemy out of range).";
        }
        screen.updateOutputConsole(attackText);
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

            // Ensure that at least 2 large rooms will be generated
            Room large(2, RoomType::Large, RandomCoord());
            Room large2(3, RoomType::Large, RandomCoord());
            rooms.push_back(spawn);
            rooms.push_back(boss);
            rooms.push_back(large);
            rooms.push_back(large2);

            for (int i = 4; i < roomCount; ++i) {
                RoomType randomType = static_cast<RoomType>(1 + rand() % 3);
                rooms.emplace_back(Room(i, randomType, RandomCoord()));
            }

            connectRooms();
            validMap = validateMap();

            if (!validMap) {
                ct::p | "Map generation failed, regenerating...\n";
            }
        }

        ct::p | "Valid map generated with " | rooms.size() | " rooms.\n";
    }

    void connectRooms() {
        for (auto& room : rooms) {
            if (room.roomType == RoomType::Boss || room.roomType == RoomType::Spawn) continue;

            int minConnections = 2;
            int maxConnections = 2;

            std::vector<Room*> potentialConnections;
            for (auto& other : rooms) {
                if (&room != &other && other.roomType != RoomType::Boss) {
                    potentialConnections.push_back(&other);
                }
            }

            std::sort(potentialConnections.begin(), potentialConnections.end(), [&room](const Room* a, const Room* b) {
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

        Room& bossRoom = rooms[1];
        if (bossRoom.connectedRooms.empty()) {
            Room* nearestRoom = nullptr;
            double minDistance = std::numeric_limits<double>::infinity();

            for (auto& room : rooms) {
                if (room.roomType != RoomType::Boss && room.roomType != RoomType::Spawn) {
                    double distance = bossRoom.distanceTo(room);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestRoom = &room;
                    }
                }
            }

            if (nearestRoom) {
                bossRoom.connectedRooms.push_back(nearestRoom->roomNumber);
                nearestRoom->connectedRooms.push_back(bossRoom.roomNumber);
            }
        }

        ensureGraphConnectivity();
    }

    void ensureGraphConnectivity() {
        std::vector<bool> visited(rooms.size(), false);
        Queue<int> toVisit;
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

        Room& bossRoom = rooms[1]; 
        if (bossRoom.connectedRooms.empty()) {
            Room* nearestRoom = nullptr;
            double minDistance = std::numeric_limits<double>::infinity();

            for (auto& room : rooms) {
                if (room.roomType != RoomType::Boss && room.roomType != RoomType::Spawn) {
                    double distance = bossRoom.distanceTo(room);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestRoom = &room;
                    }
                }
            }

            if (nearestRoom) {
                bossRoom.connectedRooms.push_back(nearestRoom->roomNumber);
                nearestRoom->connectedRooms.push_back(bossRoom.roomNumber);
            }
        }
    }

    bool validateMap() {
        std::vector<bool> visited(rooms.size(), false);
        Queue<int> toVisit;
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
                return false;
            }
        }

        return true;
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
        drawConnections(visitedRooms);
        for (const auto& room : rooms) {
            char symbol = '?';
            if (visitedRooms.find(room.roomNumber) != visitedRooms.end()) {
                switch (room.roomType) {
                    case RoomType::Spawn:    symbol = 'S'; break;
                    case RoomType::Treasure: symbol = 'T'; break;
                    case RoomType::Trap:     symbol = 'X'; break;
                    case RoomType::Large:    symbol = 'L'; break;
                    case RoomType::Boss:     symbol = 'B'; break;
                }
            }
            std::string color = visitedRooms.find(room.roomNumber) != visitedRooms.end() ? BOLD_WHITE : GREY;
            screen.setChar(room.position.first, room.position.second, symbol, color);
        }
        screen.print();
    }

    void drawConnections(const std::set<int>& visitedRooms) const {
        for (const auto& room : rooms) {
            for (int connectedRoomNumber : room.connectedRooms) {
                const Room& connectedRoom = rooms[connectedRoomNumber];
                drawLine(room.position.first, room.position.second, connectedRoom.position.first, connectedRoom.position.second);
            }
        }
    }

    void drawLine(int x1, int y1, int x2, int y2) const {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        int sleepTime = 10;

        while (true) {
            screen.setChar(x1, y1, '.');
            if (!IS_ANIMATION_SHOWN) {
                screen.print();
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
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

            if (sleepTime < 50 && !IS_ANIMATION_SHOWN) {
                sleepTime = static_cast<int>(sleepTime * 0.9);
            }
        }
    }
};





std::set<std::pair<int, int>> Floor::occupiedPositions;

class RoomBase {
public:
    bool isCleared = false;
    virtual ~RoomBase() = default;
    virtual void enterRoom(Player& player) = 0;
    virtual bool hasEnemy() const = 0;
    virtual const EnemyVariant& getEnemy() const {
        throw std::runtime_error("No enemy in this room");
    }
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
        : room(room), combat(player, enemy), enemy(generateRandomEnemy()) {}

    bool hasEnemy() const override {
        return true;
    }

    const EnemyVariant& getEnemy() const override {
        return enemy;
    }

    void enterRoom(Player& player) override {
        if (isCleared) {
            return;
        }
        screen.LargeRoomAnimation();
        screen.initializeFight();
        combat.makeFloor();
        bool combatWon = combat.startCombat();

        if (combatWon) {
            isCleared = true; 
            ct::p | "Room is cleared!\n";
        } else {
            ct::p | "You were defeated in this room!\n";
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
            ct::p | "You have already cleared this room!\n";
            return;
        } else {
            screen.TreasureRoomAnimation();
            int reward = rand() % 2;

            if (reward == 0) {
                std::vector<CardVariant> possibleCards = {
                    DoubleSlash(),
                    SingleSlash(),
                    Bow(),
                    Dash()
                };
                int cardIndex = rand() % possibleCards.size();
                player.addCard(possibleCards[cardIndex]);
            } else {
                // Give a healing potion
                player.heal(2);
            }
            screen.print();
            // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

            isCleared = true; 
        }
    }

    bool hasEnemy() const override {
        return false; 
    }
};





class TrapRoom : public RoomBase {
private:
    Room room;
public:
    TrapRoom(const Room& room) : room(room) {}
    void enterRoom(Player& player) override {
        if (isCleared) {
            return;
        }
        int trapEffect = rand() % 2; 

        if (trapEffect == 0) {
            std::vector<CardVariant> possibleCards = {
                DoubleSlash(),
                SingleSlash(),
                Bow(),
                Dash()
            };
            int cardIndex = rand() % possibleCards.size();
            auto& attackCard = possibleCards[cardIndex];
            std::visit([&](auto& card) { card.use(player); }, attackCard); // Apply the card's effect on the player
        } else {
            player.damage(1);
        }

        screen.TrapRoomAnimation();
        isCleared = true;
    }

    bool hasEnemy() const override {
        return false;
    }
};





class BossRoom : public RoomBase {
private:
    Room room;
    Combat combat;
    EnemyVariant enemy;

public:
    BossRoom(const Room& room, Player& player) : room(room), enemy(std::in_place_type<InfernalWarlord>), combat(player, enemy) {}
    void enterRoom(Player& player) override {
        if (isCleared) {
            ct::p | "You have already cleared this room!\n";
            getchar();
            return;
        } else {
            screen.LargeRoomAnimation();
            screen.initializeFight();
            combat.makeFloor();
            bool combatWon = combat.startCombat();

            if (combatWon) {
                isCleared = true;
                ct::p | "Boss defeated! Room is cleared!\n";
            } else {
                ct::p | "You were defeated by the boss!\n";
            }
        }
    }
    bool hasEnemy() const override {
        return true;
    }
};



using RoomVariant = std::variant<std::unique_ptr<LargeRoom>, std::unique_ptr<TreasureRoom>, std::unique_ptr<TrapRoom>, std::unique_ptr<BossRoom>>;

class GameLogic {
private:
    Floor floor;
    Room currentRoom;
    Player player;
    std::set<int> visitedRooms;
    std::unordered_map<int, RoomVariant> map;

public:
    GameLogic(int floorNumber, int minRooms, int maxRooms)
        : floor(floorNumber, minRooms, maxRooms), currentRoom(floor.getSpawnRoom()), player() {
        visitedRooms.insert(currentRoom.roomNumber); // Mark the spawn room as visited
    }

    void MainLoop() {
        hideCursor();
        system("cls");
        bool StartGame = false;
        while(!StartGame) {
            int selectedOption = screen.MenuWindow();
            switch (selectedOption) {
                case 0:
                    StartGame = true;
                    break;
                case 1:
                    screen.InitializeInstructionsWindow();
                    break;
                case 2:
                    exit(0);
                    break;
            }
        }        
        screen.clear();
        screen.StartingAnimation(20);
        std::vector<Room> rooms = floor.getRooms();

        for (const auto& room : rooms) {
            switch (room.roomType) {
                case RoomType::Large:
                    map[room.roomNumber] = std::make_unique<LargeRoom>(room, player);
                    break;
                case RoomType::Treasure:
                    map[room.roomNumber] = std::make_unique<TreasureRoom>(room);
                    break;
                case RoomType::Trap:
                    map[room.roomNumber] = std::make_unique<TrapRoom>(room);
                    break;
                case RoomType::Boss:
                    map[room.roomNumber] = std::make_unique<BossRoom>(room, player);
                    break;
                case RoomType::Spawn:
                    break;
            }
        }

        while (player.isAlive()) {
            screen.InitializeMap();
            screen.MapOptions(currentRoom, rooms, visitedRooms);
            floor.DrawMap(visitedRooms);
            IS_ANIMATION_SHOWN = true;

            int choice;
            choice = screen.takeRoomChoice();

            if (choice == -1) {
                screen.setString(100, 20, L"Exiting game...");
                screen.print();
                break;
            }

            if (choice < 0 || choice >= currentRoom.connectedRooms.size() || choice == -2) {
                continue;
            }

            int nextRoomNumber = currentRoom.connectedRooms[choice];
            currentRoom = rooms[nextRoomNumber];
            visitedRooms.insert(currentRoom.roomNumber);

            auto it = map.find(currentRoom.roomNumber);
            if (it != map.end()) {
                std::visit([&](auto& roomPtr) {
                    roomPtr->enterRoom(player);
                    if (!player.isAlive()) {
                        ct::p | "You died! Game Over.\n";
                        exit(0);
                    }
                }, it->second);
            }
        }

        if (!player.isAlive()) {
            ct::p | "Game Over! You have been defeated.\n";
        }
    }

};