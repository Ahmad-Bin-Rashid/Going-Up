#pragma once

#include "Logic.h"
#include <vector>

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
        void print() const 
        {
            system("cls");
            for (const auto &row : buffer) 
                {
                    std::cout << row << '\n';
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
            int StartX = 90;
            int StartY = 2;

            setString(StartX, StartY, "--------------------------------------------------------------");
            setString(StartX + 5, StartY + 2, "-> Current Room: " + roomTypeToString(currentRoom.roomType));
            setString(StartX + 5, StartY + 3, "-> Room Position: (" + std::to_string(currentRoom.position.first) + ", " + std::to_string(currentRoom.position.second) + ")");

            setString(StartX + 5, StartY + 5, "-> Connected Rooms:");
            int count = 0;
            for (int connectedRoomNumber : currentRoom.connectedRooms) {
                setString(StartX + 5, StartY + 6 + count, std::to_string(count) + ". ");
                setString(StartX + 5 + 3, StartY + 6 + count, roomTypeToString(rooms[connectedRoomNumber].roomType) + "  (" + std::to_string(rooms[connectedRoomNumber].position.first) + ", " + std::to_string(rooms[connectedRoomNumber].position.second) + ")");
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
            setString(17, 4, "20/20");
            setString(56, 4, "5/5");
            setString(0, 6, "----------------------------------------------------------------------------------");
            setString(0, 7, "  PLAYER STACK:                           ENEMY STACK:");
            setString(0, 8, "  [Card 1]  [Card 2]                      [Enemy Card 1]  [Enemy Card 2]");
            setString(0, 10, "----------------------------------------------------------------------------------");
            setString(0, 11, "AVAILABLE CARDS :");
            setString(0, 12, "  [Slash (0)]  [Guard (1)]  [Heavy Strike (2)]  [Dodge (0)]  [Fireball (3)]");
            setString(0, 14, "----------------------------------------------------------------------------------");
            setString(0, 15, "AVAILABLE MOVES:");
            setString(0, 16, "  1) Move Left");
            setString(0, 17, "  2) Move Right");
            setString(0, 18, "  3) Add Card To Attack Stack");
            setString(0, 19, "  4) Attack with Current Attack Stack");
            setString(0, 20, "  5) Change Direction");
            setString(0, 21, "  6) Pass Turn");
            setString(0, 23, "----------------------------------------------------------------------------------");
            setString(0, 24, ">>");
            setString(0, 25, ">>");
            setString(0, 26, ">>");
            setString(0, 28, "----------------------------------------------------------------------------------");
            setString(0, 29, "Next Move:");
            setString(0, 30, ">>");
        }
        void updatePlayerAttackStack(std::vector<std::string> playerAttackStack) {
            // clear the previous cards
            for (int i = 1; i < 85; i+=15) {
                setString(i, 8, "               ");
            }
            for (int i = 0; i < playerAttackStack.size(); ++i) {
                if (i < playerAttackStack.size()) {
                    setString(2 + 20 * i, 8, playerAttackStack[i]);
                } else {
                    setString(2 + 20 * i, 8, "          ");
                }
            }
        }
        void updateEnemyAttackStack(std::vector<std::string> enemyAttackStack) {
            // clear the previous cards
            for (int i = 1; i < 85; i+=15) {
                setString(i, 8, "               ");
            }
            for (int i = 0; i < enemyAttackStack.size(); ++i) {
                if (i < enemyAttackStack.size()) {
                    setString(2 + 20 * i, 8, enemyAttackStack[i]);
                } else {
                    setString(2 + 20 * i, 8, "               ");
                }
            }
        }
        void updateAvailableCards(std::vector<std::string> cards) {
            // clear the previous cards
            for (int i = 1; i < 85; i++) {
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
            for (int i = 1; i < 85; i+=15) {
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
            if (consoleOutputs.getSize() == 3) {
                consoleOutputs.dequeue();
            }
            consoleOutputs.enqueue(consoleText);
        }
        void printOutputConsole() {
            if (consoleOutputs.getSize() == 0) {
                return;
            }
            for (int i = 0; i < 3; ++i) {
                setString(4, 24 + i, consoleOutputs.front());
                consoleOutputs.enqueue(consoleOutputs.dequeue());
            }
        }
        void updatePlayerHP(int hp,int maxHP) 
        {
            std::string hpStr = std::to_string(hp) + "/" + std::to_string(maxHP);
            setString(17, 4, hpStr);
        }
        void updateEnemyHP(int hp,int maxHP) 
        {
            std::string hpStr = std::to_string(hp) + "/" + std::to_string(maxHP);
            setString(56, 4, hpStr);
        }
        void updatePlayerAttackCardsStack() {

        }
        void updateEnemyAttackCardsStack() {

        }
        void updatePrintAvailableCards(const std::vector<std::string> &cards) 
        {
            for (size_t i = 0; i < cards.size(); ++i) 
            {
                setString(2 + 15 * i, 12, cards[i]);
            }
        }


        void InitializeTreasureRoom() {
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
            setString(2, 25,"|>> You Got a Potion .Healed for 3 Hp .                                         |");
            setString(2, 26,"|   Your Journey Will Be Continued ...                                               |");
            setString(2, 27,"|_______________________________________________________________________________|");
        }


// |*******************************************************************************|                                                                                  
// |                    ___                              _...__                    |                                                              
// |                 .-'   '--._                       .'_     '-.                 |                                                                  
// |               .'     .--._ '-._                 .'.' \       '.               |                                                                  
// |              /      /__   `'.  '.              / /    `\       \              |                                                                      
// |             /      /   '-.    `\ \           /'/'       \      |              |                                                                      
// |             |     |       '.    `\`\        / /     _.-'|      |              |                                                                      
// |             |     |         \     \ \      / /    .'    |      /              |                                                                      
// |              \    /          '.   | |,-~-,/ /   .'      \     /               |                                                                  
// |               '--'           __\               /__       '._.'                |                                                                  
// |                            ."  '.             .'  ".                          |                                                          
// |                            |      '.       .'      |                          |                                                          
// |                             \ ',    '.   .'    .' /                           |                                                      
// |                             /'-.""-._ \ / _.-"".-'\                           |                                                      
// |                  _.=='''--,/ '/_.--._\'V'/_.--._\' \,--'''--._                |                                                                  
// |                .'            _'-.__0_;\ /;_0__.-' _'          '.              |                                                                      
// |               /     ,____..-'\'.      """      -'/'--..____,    \             |                                                                      
// |              /     '          \       .=.       /          `     \            |                                                                      
// |              |             __  '-.   .-=-.   .-'  __             |            |                                                                      
// |               \      _.--''  ''---'. .-=-. .'---''  ''--._      /             |                                                                      
// |                '----'            .-'  ___  '-.            `----'              |                                                                      
// |                                 (    '   '    )                               |                                                  
// |                                  '.    _    .'                                |                                                  
// |                                    '--/ \--'                                  |                                                  
// |                                       |#|                                     |                                              
// |                                       \_/                                     |                                              
// |                                                                               |  
// |*******************************************************************************|
// |_______________________________________________________________________________|
// |                                                                               |
// |                       Congrats You entered a Trap Room                        |
// |_______________________________________________________________________________|
// |                                                                               |
// |>> You Got Hit for  3 Damage.                                                  |
// |   Press any key to continue ...                                               |
// |_______________________________________________________________________________|

        void InitializeTrapRoom() {
            clear();

            setString(2, 0,"|*******************************************************************************|");
            setString(2, 1,"|                    ___                              _...__                    |");
            setString(2, 2,"|                 .-'   '--._                       .'_     '-.                 |");
            setString(2, 3,"|               .'     .--._ '-._                 .'.' \\       '.               |");
            setString(2, 4,"|              /      /__   `'.  '.              / /    '\\       \\              |");
            setString(2, 5,"|             /      /   '-.    '\\ \\           /'/\\'       \\      |              |");
            setString(2, 6,"|             |     |       '.    '\\\\        / /     _.-'|      |              |");
            setString(2, 7,"|             |     |         \\     \\ \\      / /    .'    |      /              |");
            setString(2, 8,"|              \\    /          '.   | |,-~-,/ /   .'      \\     /              |");
            setString(2, 9,"|               '--'           __\\               /__       '._.'                |");
            setString(2, 10,"|                            .\"  '.             .'  \".                          |");
            setString(2, 11,"|                            |      '.       .'      |                          |");
            setString(2, 12,"|                             \\ ',    '.   .'    .' /                           |");
            setString(2, 13,"|                             /'-.\"\"-._ \\ / _.-\"\".-'\\                           |");
            setString(2, 14,"|                  _.=='''--,/ '/_.--._\\'V'/_.--._\\' \\,--'''--._                |");
            setString(2, 15,"|                .'            _'-.__0_;\\ /;_0__.-' _'          '.              |");
            setString(2, 16,"|               /     ,____..-'\\'.      \"\"\"      -'/--..____,    \\             |");
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
            setString(2, 34,"|   Your Journey Will Be Continued ...                                               |");
            setString(2, 35,"|_______________________________________________________________________________|");
        }
        
};