#pragma once

#include <vector>
#include <sstream>
#include "../Utilities/CustomNamespace.h"
#include "Logic.h"

#define SCREEN_WIDTH 140
#define SCREEN_HEIGHT 36

class ScreenBuffer
{
    private:
        std::vector<std::string> buffer;
        int width, height;
        Queue<std::string> consoleOutputs;
    
    public:
        ScreenBuffer(int w, int h) : width(w), height(h), buffer(h, std::string(w, ' ')) {}

        void setChar(int x, int y, char c) 
        {
            if (y >= 0 && y < height && x >= 0 && x < width) 
                {
                    buffer[y][x] = c;
                }
        }

        void setString(int x, int y, const std::string &str) 
        {
            if (y >= 0 && y < height) 
            {
                for (size_t i = 0; i < str.size() && x + i < width; ++i) 
                {
                    buffer[y][x + i] = str[i];
                }
            }
        }
        void clear() 
        {
            for (auto &row : buffer) 
                {
                    row = std::string(width, ' ');
                }
        }
        void print() 
        {
            gotoxy(0, 0);
            ct::p | "\n";
            for (const auto &row : buffer) 
            {
                ct::p | "  " + row | '\n';
            }
        }
        void InitializeMap() 
        {
            clear();
            for (int i = 0; i < 34; ++i) {
                if (i%5 == 0) {
                    setString(83, i, std::to_string(i));
                    setString(85, i, "-");
                } else {
                    setString(85, i, "|");
                }
            }
            for (int i = 0; i < 85; ++i) {
                if (i%10 == 0) {
                    setString(i, 32, std::to_string(i));
                    setString(i, 33, "|");
                } else {
                    setString(i, 33, "_");
                }
            }
        }

        void MapOptions(Room currentRoom, std::vector<Room> rooms, std::set<int>& occupiedPositions) {
            system("cls");
            int StartX = 90;
            int StartY = 2;

            setString(StartX, StartY, "--------------------------------------------------------------");
            setString(StartX + 5, StartY + 2, "-> Current Room: " + roomTypeToString(currentRoom.roomType));
            setString(StartX + 5, StartY + 3, "-> Room Position: (" + std::to_string(currentRoom.position.first) + ", " + std::to_string(currentRoom.position.second) + ")");

            setString(StartX + 5, StartY + 5, "-> Connected Rooms:");
            int count = 0;
            for (int connectedRoomNumber : currentRoom.connectedRooms) {
            std::string roomInfo = occupiedPositions.count(connectedRoomNumber) ? roomTypeToString(rooms[connectedRoomNumber].roomType) : "???";
            setString(StartX + 5, StartY + 6 + count, std::to_string(count) + ". ");
            setString(StartX + 5 + 3, StartY + 6 + count, roomInfo + "  (" + std::to_string(rooms[connectedRoomNumber].position.first) + ", " + std::to_string(rooms[connectedRoomNumber].position.second) + ")");
            count++;
            }

            setString(StartX, StartY + 6 + count + 1, "--------------------------------------------------------------");
        }


        void initializeFight()
        {
            system("cls");
            clear();
            setString(0, 2, " |---------------|---------------|---------------|---------------|---------------|");
            setString(0, 4, "  PLAYER HP: [          ]                 ENEMY HP: [           ]    ");
            setString(17, 4, "?/?");
            setString(56, 4, "?/?");
            setString(0, 6, "--------------------------------------------------------------------------------------------------");
            setString(0, 7, "  PLAYER STACK:                                               ENEMY STACK:");
            setString(0, 8, "  [Card 1]  [Card 2]                                          [Enemy Card 1]  [Enemy Card 2]");
            setString(0, 10, "--------------------------------------------------------------------------------------------------");
            setString(0, 11, "AVAILABLE CARDS :");
            setString(0, 12, "  [Slash ???]  [Guard ???]  [Heavy Strike ???]  [Dodge ???]  [Fireball ???]");
            setString(0, 14, "--------------------------------------------------------------------------------------------------");
            setString(0, 15, "AVAILABLE MOVES:");
            setString(0, 16, "  1) Move Right");
            setString(0, 17, "  2) Move Left");
            setString(0, 18, "  3) Add Card To Attack Stack");
            setString(0, 19, "  4) Attack with Current Attack Stack");
            setString(0, 20, "  5) Change Direction");
            setString(0, 21, "  6) Pass Turn");
            setString(0, 23, "--------------------------------------------------------------------------------------------------");
            setString(0, 24, ">>");
            setString(0, 25, ">>");
            setString(0, 28, "--------------------------------------------------------------------------------------------------");
        }
        void updatePlayerAttackStack(std::vector<std::string> playerAttackStack) {
            for (int i = 2; i < 42; i+=15) {
                setString(i, 8, "               ");
            }
            for (int i = 0; i < playerAttackStack.size(); ++i) {
                if (i < playerAttackStack.size()) {
                    setString(2 + 15 * i, 8, playerAttackStack[i]);
                } else {
                    setString(2 + 15 * i, 8, "          ");
                }
            }
        }
        void updateEnemyAttackStack(std::vector<std::string> enemyAttackStack) {
            for (int i = 62; i < SCREEN_WIDTH; i+=15) {
                setString(i, 8, "               ");
            }
            for (int i = 0; i < enemyAttackStack.size(); ++i) {
                if (i < enemyAttackStack.size()) {
                    setString(62 + 10 * i, 8, enemyAttackStack[i]);
                } else {
                    setString(62 + 10 * i, 8, "          ");
                }
            }
        }
        void updateAvailableCards(std::vector<std::string> cards) {
            // clear the previous cards
            for (int i = 1; i < SCREEN_WIDTH; i++) {
                setString(i, 12, " ");
            }
            int count = 0;
            for (int i = 0; i < cards.size(); ++i) {
                setString(2 + 20 * i, 12, std::to_string(count) + ". " + cards[i]);
                count++;
            }
        }
        void updateTiles(std::vector<std::string> tiles, int playerDirection, int enemyDirection) {
            int count = 0;
            // clear the previous tiles
            for (int i = 1; i < SCREEN_WIDTH; i+=15) {
                setString(i, 1, "               ");
            }
            for (int i = 1; i < 85; i+=15) {
                std::string tile = tiles[count];
                if (tile == "X") {
                    setString(i+count, 1, " ");
                } else {
                    if (tile == "P") {
                        playerDirection == 1 ? setString(i+count, 1, "P->") : setString(i+count, 1, "<-P");
                    } else {
                        enemyDirection == 1 ? setString(i+count, 1, "E->") : setString(i+count-2, 1, "<-E");
                    }
                }
                count++;
            }
        }
        void updateOutputConsole(std::string consoleText) {
            setString(4, 24, "                                                                                                  ");
            setString(4, 24, consoleText);
        }
        void updatePlayerHP(int hp,int maxHP) 
        {
            std::string hpStr;
            if (hp < 10) hpStr = " " + std::string(std::to_string(hp) + "/" + std::to_string(maxHP));
            else hpStr = std::to_string(hp) + "/" + std::to_string(maxHP);
            setString(17, 4, hpStr);
        }
        void updateEnemyHP(int hp,int maxHP) 
        {
            std::string hpStr;
            if (hp < 10) hpStr = " " + std::string(std::to_string(hp) + "/" + std::to_string(maxHP));
            else hpStr = std::to_string(hp) + "/" + std::to_string(maxHP);
            setString(56, 4, hpStr);
        }
        void updatePrintAvailableCards(const std::vector<std::string> &cards) 
        {
            for (size_t i = 0; i < cards.size(); ++i) 
            {
                setString(2 + 15 * i, 12, cards[i]);
            }
        }


        void InitializeTreasureRoom() {
            system("cls");
            clear();

            setString(2, 0,"|*******************************************************************************|");
            setString(2, 1,"|          |                   |                  |                     |       |");
            setString(2, 2,"| _________|________________.=""_;=.______________|_____________________|_______|");
            setString(2, 3,"||                   |  ,-\"_,=\"\"     `\"=.|                  |                   |");
            setString(2, 4,"||___________________|__\"=._o`\"-._        `\"=.______________|___________________|");
            setString(2, 5,"|          |                `\"=._o`\"=._      _`\"=._                     |       |");
            setString(2, 6,"| _________|_____________________:=._o \"=._.\"_.-=\"'\"=.__________________|_______|");
            setString(2, 7,"||                   |    __.--\" , ; `\"=._o.\" ,-\"\"\"-._ \".   |                   |");
            setString(2, 8,"||___________________|_._\"  ,. .` ` `` ,  `\"-._\"-._   \". '__|___________________|");
            setString(2, 9,"|          |           |o`\"=._` , \"` `; .\". ,  \"-._\"-._; ;              |       |");
            setString(2, 10,"| _________|___________| ;`-.o`\"=._; .\" ` '`.\"\\` . \"-._ /_______________|_______|");
            setString(2, 11,"||                   | |o;    `\"-.o`\"=._``  '` \" ,__.--o;   |                   |");
            setString(2, 12,"||___________________|_| ;     (#) `-.o `\"=.`_.--\"_o.-; ;___|___________________|");
            setString(2, 13,"|____/______/______/___|o;._    \"      `\".o|o_.--\"    ;o;____/______/______/____|");
            setString(2, 14,"|/______/______/______/\"=._o--._        ; | ;        ; ;/______/______/______/_|");
            setString(2, 15,"|____/______/______/______/__\"=._o--._   ;o|o;     _._;o;____/______/______/____|");
            setString(2, 16,"|/______/______/______/______/____\"=._o._; | ;_.--\"o.--\"_/______/______/______/_|");
            setString(2, 17,"|____/______/______/______/______/_____\"=.o|o_.--\"\"___/______/______/______/____|");
            setString(2, 18,"|/______/______/______/______/______/______/______/______/______/______/_______/|");
            setString(2, 19,"|*******************************************************************************|");
            setString(2, 20,"|_______________________________________________________________________________|");
            setString(2, 21,"|                                                                               |");
            setString(2, 22,"|                     Congrats You entered a Treasure Room                      |");
            setString(2, 23,"|_______________________________________________________________________________|");
            setString(2, 24,"|                                                                               |");
            setString(2, 25,"|>> You Got a Potion. Healed for 3 Hp.                                          |");
            setString(2, 26,"|   Your Journey Will Be Continued ...                                          |");
            setString(2, 27,"|_______________________________________________________________________________|");
        }

        void InitializeTrapRoom() {
            system("cls");
            clear();

            setString(2, 0,"|*******************************************************************************|");
            setString(2, 1,"|                    ___                              _...__                    |");
            setString(2, 2,"|                 .-'   '--._                       .'_     '-.                 |");
            setString(2, 3,"|               .'     .--._ '-._                 .'.' \\       '.               |");
            setString(2, 4,"|              /      /__   `'.  '.              / /    '\\       \\              |");
            setString(2, 5,"|             /      /   '-.    '\\ \\           /'/\\'       \\      |             |");
            setString(2, 6,"|             |     |       '.    '\\\\        / /     _.-'|      |               |");
            setString(2, 7,"|             |     |         \\     \\ \\      / /    .'    |      /              |");
            setString(2, 8,"|              \\    /          '.   | |,-~-,/ /   .'      \\     /               |");
            setString(2, 9,"|               '--'           __\\               /__       '._.'                |");
            setString(2, 10,"|                            .\"  '.             .'  \".                          |");
            setString(2, 11,"|                            |      '.       .'      |                          |");
            setString(2, 12,"|                             \\ ',    '.   .'    .' /                           |");
            setString(2, 13,"|                             /'-.\"\"-._ \\ / _.-\"\".-'\\                           |");
            setString(2, 14,"|                  _.=='''--,/ '/_.--._\\'V'/_.--._\\' \\,--'''--._                |");
            setString(2, 15,"|                .'            _'-.__0_;\\ /;_0__.-' _'          '.              |");
            setString(2, 16,"|               /     ,____..-'\\'.      \"\"\"      -'/--..____,    \\              |");
            setString(2, 17,"|              /     '          \\       .=.       /          `     \\            |");
            setString(2, 18,"|              |             __  '-.   .-=-.   .-'  __             |            |");
            setString(2, 19,"|               \\      _.--''  ''---'. .-=-. .'---''  ''--._      /             |");
            setString(2, 20,"|                '----'            .-'  ___  '-.            `----'              |");
            setString(2, 21,"|                                 (    '   '    )                               |");
            setString(2, 22,"|                                  '.    _    .'                                |");
            setString(2, 23,"|                                    '--/ \\--'                                  |");
            setString(2, 24,"|                                       |#|                                     |");
            setString(2, 25,"|                                       \\_/                                     |");
            setString(2, 26,"|                                                                               |");
            setString(2, 27,"|*******************************************************************************|");
            setString(2, 28,"|_______________________________________________________________________________|");
            setString(2, 29,"|                                                                               |");
            setString(2, 30,"|                       Congrats You entered a Trap Room                        |");
            setString(2, 31,"|_______________________________________________________________________________|");
            setString(2, 32,"|                                                                               |");
            setString(2, 33,"|>> You Got Hit for  3 Damage.                                                  |");
            setString(2, 34,"|   Your Journey Will Be Continued ...                                          |");
            setString(2, 35,"|_______________________________________________________________________________|");
        }

    void InitializeInstructionsWindow() {
        clear();
        setString(2, 0, " ____                                                                                                            ____ ");
        setString(2, 1, "( __ )                                                                                                          ( __ )");
        setString(2, 2, " |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  | ");
        setString(2, 3, " |  |                                                                                                            |  | ");
        setString(2, 4, " |  |                                                                                                            |  | ");
        setString(2, 5, " |  |                                                                                                            |  | ");
        setString(2, 6, " |  |                                                                                                            |  | ");
        setString(2, 7, " |  |                                                                                                            |  | ");
        setString(2, 8, " |  |                                                                                                            |  | ");
        setString(2, 9, " |  |                                                                                                            |  | ");
        setString(2, 10, " |  |                                                                                                            |  | ");
        setString(2, 11, " |  |                                                                                                            |  | ");
        setString(2, 12, " |  |                                                                                                            |  | ");
        setString(2, 13, " |  |                                                                                                            |  | ");
        setString(2, 14, " |  |                                                                                                            |  | ");
        setString(2, 15, " |  |                                                                                                            |  | ");
        setString(2, 16, " |  |                                                                                                            |  | ");
        setString(2, 17, " |  |                                                                                                            |  | ");
        setString(2, 18, " |  |                                                                                                            |  | ");
        setString(2, 19, " |  |                                                                                                            |  | ");
        setString(2, 20, " |  |                                                                                                            |  | ");
        setString(2, 21, " |  |                                                                                                            |  | ");
        setString(2, 22, " |  |                                                                                                            |  | ");
        setString(2, 23, " |  |                                                                                                            |  | ");
        setString(2, 24, " |  |                                                                                                            |  | ");
        setString(2, 25, " |  |                                                                                                            |  | ");
        setString(2, 26, " |  |                                                                                                            |  | ");
        setString(2, 27, " |  |                                                                                                            |  | ");
        setString(2, 28, " |  |                                                                                                            |  | ");
        setString(2, 29, " |  |                                                                                                            |  | ");
        setString(2, 30, " |  |                                                                                                            |  | ");
        setString(2, 31, " |  |                                                                                                            |  | ");
        setString(2, 32, " |  |                                                                                                            |  | ");
        setString(2, 33, " |__|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|__| ");
        setString(2, 34, "(____)                                                                                                          (____)");
        printTextInsideInstructionsOneWordAtATime();
    }

    void printTextInsideInstructionsOneWordAtATime() {
        std::vector<std::string> paragraphs = {
            "Welcome to 'Going Up'! You find yourself trapped in an ancient dungeon after a violent typhoon struck your town. The storm caused a massive power outage, plunging the entire area into darkness. Seeking shelter, you stumble upon a hidden entrance to a mysterious underground labyrinth. With no other options, you decide to venture inside.",
            "The dungeon is filled with treacherous traps, fearsome enemies, and valuable treasures. Your goal is to navigate through the dungeon's many rooms, each with its own unique challenges, and reach the top floor to find a way back to the surface. Along the way, you must gather resources, fight enemies, and solve puzzles to survive.",
            "Can you overcome the dangers of the dungeon and escape to safety? Good luck!"
        };

        int x = 10;
        int y = 6;
        for (const auto& paragraph : paragraphs) {
            std::istringstream iss(paragraph);
            std::string word;
            std::string line;
            while (iss >> word) {
                if (line.size() + word.size() + 1 > 100) {
                    setString(x, y++, line);
                    line.clear();
                }
                if (!line.empty()) {
                    line += " ";
                }
                line += word;
                setString(x, y, line);
                print();
                std::this_thread::sleep_for(std::chrono::milliseconds(0));
            }
            if (!line.empty()) {
                setString(x, y++, line);
                print();
            }
            y++;
        }
    }
};