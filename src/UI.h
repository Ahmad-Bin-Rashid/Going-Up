#pragma once

#include <vector>
#include <sstream>
#include <io.h>
#include <fcntl.h>
#include <random>
#include "../Utilities/CustomNamespace.h"
#include "Logic.h"

#define SCREEN_WIDTH 158
#define SCREEN_HEIGHT 43
int TILE_START = 30;
int TILE_END = 120;
int PLAYER_X = 38;
int ENEMY_X = 56;
int TILE_DIFF_X = 18;
int P_POSITION = 0;
int E_POSITION = 0;
int P_DIRECTION = 1;
int E_DIRECTION = -1;
int CONNECTED_ROOMS = 0;

// Define ANSI color codes
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define GREY "\033[90m"

#define BOLD_BLACK "\033[1m\033[30m"
#define BOLD_RED "\033[1m\033[31m"
#define BOLD_GREEN "\033[1m\033[32m"
#define BOLD_YELLOW "\033[1m\033[33m"
#define BOLD_BLUE "\033[1m\033[34m"
#define BOLD_MAGENTA "\033[1m\033[35m"
#define BOLD_CYAN "\033[1m\033[36m"
#define BOLD_WHITE "\033[1m\033[37m"

#define UNDERLINE_BLACK "\033[4m\033[30m"
#define UNDERLINE_RED "\033[4m\033[31m"
#define UNDERLINE_GREEN "\033[4m\033[32m"
#define UNDERLINE_YELLOW "\033[4m\033[33m"
#define UNDERLINE_BLUE "\033[4m\033[34m"
#define UNDERLINE_MAGENTA "\033[4m\033[35m"
#define UNDERLINE_CYAN "\033[4m\033[36m"
#define UNDERLINE_WHITE "\033[4m\033[37m"

#define WRESET L"\033[0m"
#define WBLACK L"\033[30m"
#define WRED L"\033[31m"
#define WGREEN L"\033[32m"
#define WYELLOW L"\033[33m"
#define WBLUE L"\033[34m"
#define WMAGENTA L"\033[35m"
#define WCYAN L"\033[36m"
#define WWHITE L"\033[37m"
#define WGREY L"\033[90m"

#define WBOLD_BLACK L"\033[1m\033[30m"
#define WBOLD_RED L"\033[1m\033[31m"
#define WBOLD_GREEN L"\033[1m\033[32m"
#define WBOLD_YELLOW L"\033[1m\033[33m"
#define WBOLD_BLUE L"\033[1m\033[34m"
#define WBOLD_MAGENTA L"\033[1m\033[35m"
#define WBOLD_CYAN L"\033[1m\033[36m"
#define WBOLD_WHITE L"\033[1m\033[37m"

#define WUNDERLINE_BLACK L"\033[4m\033[30m"
#define WUNDERLINE_RED L"\033[4m\033[31m"
#define WUNDERLINE_GREEN L"\033[4m\033[32m"
#define WUNDERLINE_YELLOW L"\033[4m\033[33m"
#define WUNDERLINE_BLUE L"\033[4m\033[34m"
#define WUNDERLINE_MAGENTA L"\033[4m\033[35m"
#define WUNDERLINE_CYAN L"\033[4m\033[36m"
#define WUNDERLINE_WHITE L"\033[4m\033[37m"
#define WUNDERLINE_GREY L"\033[4m\033[90m"

class ScreenBuffer
{
private:
    std::vector<std::string> buffer;
    std::vector<std::wstring> wbuffer;
    std::vector<std::vector<std::string>> colorBuffer;
    std::vector<bool> isUnicodeRow;
    int width, height;

public:
    ScreenBuffer(int w, int h) : width(w), height(h),
                                 buffer(h, std::string(w, ' ')),
                                 wbuffer(h, std::wstring(w, L' ')),
                                 colorBuffer(h, std::vector<std::string>(w, RESET)),
                                 isUnicodeRow(h, false) {}

    void setChar(int x, int y, char c, const std::string &color = RESET)
    {
        if (y >= 0 && y < height && x >= 0 && x < width)
        {
            buffer[y][x] = c;
            colorBuffer[y][x] = color;
            isUnicodeRow[y] = false;
        }
    }

    void setChar(int x, int y, wchar_t c, const std::wstring &color = L"\033[0m")
    {
        if (y >= 0 && y < height && x >= 0 && x < width)
        {
            wbuffer[y][x] = c;
            colorBuffer[y][x] = std::string(color.begin(), color.end());
            isUnicodeRow[y] = true;
        }
    }

    void setString(int x, int y, const std::string &str, const std::string &color = RESET)
    {
        if (y >= 0 && y < height)
        {
            for (size_t i = 0; i < str.size() && x + i < width; ++i)
            {
                buffer[y][x + i] = str[i];
                colorBuffer[y][x + i] = color;
            }
            isUnicodeRow[y] = false;
        }
    }

    void setString(int x, int y, const std::wstring &wstr, const std::wstring &color = L"\033[0m")
    {
        if (y >= 0 && y < height)
        {
            for (size_t i = 0; i < wstr.size() && x + i < width; ++i)
            {
                wbuffer[y][x + i] = wstr[i];
                colorBuffer[y][x + i] = std::string(color.begin(), color.end());
            }
            isUnicodeRow[y] = true;
        }
    }

    void clear()
    {
        for (int y = 0; y < height; ++y)
        {
            buffer[y] = std::string(width, ' ');
            wbuffer[y] = std::wstring(width, L' ');
            std::fill(colorBuffer[y].begin(), colorBuffer[y].end(), RESET);
            isUnicodeRow[y] = false;
        }
    }

    void clearPlayerEnemy()
    {
        for (int y = 4; y < 15; ++y)
        {
            for (int x = TILE_START; x < TILE_END; ++x)
            {
                buffer[y][x] = ' ';
                wbuffer[y][x] = L' ';
                colorBuffer[y][x] = RESET;
            }
        }
    }

    void print()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE)
        {
            return;
        }

        // Console character buffer
        std::vector<CHAR_INFO> screenBuffer(width * height);

        // Fill the CHAR_INFO array
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                CHAR_INFO &charInfo = screenBuffer[y * width + x];
                if (isUnicodeRow[y])
                {
                    // Unicode characters
                    charInfo.Char.UnicodeChar = wbuffer[y][x];
                }
                else
                {
                    // Non-Unicode characters
                    charInfo.Char.AsciiChar = buffer[y][x];
                }
                // Convert ANSI color codes to console attributes
                if (colorBuffer[y][x] == RESET)
                {
                    charInfo.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                }
                else if (colorBuffer[y][x] == BLACK)
                {
                    charInfo.Attributes = 0;
                }
                else if (colorBuffer[y][x] == RED)
                {
                    charInfo.Attributes = FOREGROUND_RED;
                }
                else if (colorBuffer[y][x] == GREEN)
                {
                    charInfo.Attributes = FOREGROUND_GREEN;
                }
                else if (colorBuffer[y][x] == YELLOW)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_GREEN;
                }
                else if (colorBuffer[y][x] == BLUE)
                {
                    charInfo.Attributes = FOREGROUND_BLUE;
                }
                else if (colorBuffer[y][x] == MAGENTA)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_BLUE;
                }
                else if (colorBuffer[y][x] == CYAN)
                {
                    charInfo.Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE;
                }
                else if (colorBuffer[y][x] == WHITE)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                }
                else if (colorBuffer[y][x] == GREY)
                {
                    charInfo.Attributes = FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_BLACK)
                {
                    charInfo.Attributes = 0 | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_RED)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_GREEN)
                {
                    charInfo.Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_YELLOW)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_BLUE)
                {
                    charInfo.Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_MAGENTA)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_CYAN)
                {
                    charInfo.Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == BOLD_WHITE)
                {
                    charInfo.Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                }
                else if (colorBuffer[y][x] == UNDERLINE_BLACK || colorBuffer[y][x] == UNDERLINE_RED || colorBuffer[y][x] == UNDERLINE_GREEN || colorBuffer[y][x] == UNDERLINE_YELLOW || colorBuffer[y][x] == UNDERLINE_BLUE || colorBuffer[y][x] == UNDERLINE_MAGENTA || colorBuffer[y][x] == UNDERLINE_CYAN || colorBuffer[y][x] == UNDERLINE_WHITE)
                {
                    charInfo.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | COMMON_LVB_UNDERSCORE;
                }
            }
        }

        // Define the area to write to
        SMALL_RECT writeRegion = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};

        WriteConsoleOutputW(hConsole, screenBuffer.data(), {static_cast<SHORT>(width), static_cast<SHORT>(height)}, {0, 0}, &writeRegion);
    }

    int MenuWindow()
    {
        clear();
        int selectedOption = 0;
        const int totalOptions = 3;
        std::vector<std::string> options = {"-> New Game", "-> Story", "-> Exit"};

        while (true)
        {
            for (int i = 0; i < totalOptions; ++i)
            {
                if (i == selectedOption)
                {
                    setString(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2 + i * 2, options[i], BOLD_WHITE);
                }
                else
                {
                    setString(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 2 + i * 2, options[i], GREY);
                }
            }
            print();

            if (GetAsyncKeyState(VK_UP) & 0x8000)
            {
                selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
                Sleep(150);
            }
            else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
            {
                selectedOption = (selectedOption + 1) % totalOptions;
                Sleep(150);
            }
            else if (GetAsyncKeyState(VK_RETURN) & 0x8000)
            {
                break;
            }
        }
        return selectedOption;
    }

    void InitializeMap()
    {
        clear();
        for (int i = 0; i < 34; ++i)
        {
            if (i % 5 == 0)
            {
                setString(83, i, std::to_string(i));
                setString(85, i, "-");
            }
            else
            {
                setString(85, i, "|");
            }
        }
        for (int i = 0; i < 85; ++i)
        {
            if (i % 10 == 0)
            {
                setString(i, 32, std::to_string(i));
                setString(i, 33, "|");
            }
            else
            {
                setString(i, 33, "_");
            }
        }
    }

    void MapOptions(Room currentRoom, std::vector<Room> rooms, std::set<int> &occupiedPositions)
    {
        system("cls");
        int StartX = 95;
        int StartY = 2;

        setString(StartX, StartY, "______________________________________________________________", BOLD_YELLOW);
        setString(StartX + 5, StartY + 2, "-> Current Room: " + roomTypeToString(currentRoom.roomType), BOLD_GREEN);
        setString(StartX + 5, StartY + 3, "-> Room Position: (" + std::to_string(currentRoom.position.first) + ", " + std::to_string(currentRoom.position.second) + ")", BOLD_GREEN);

        setString(StartX + 5, StartY + 5, "-> Connected Rooms:", BOLD_BLUE);
        int count = 0;
        for (int connectedRoomNumber : currentRoom.connectedRooms)
        {
            std::string roomInfo = occupiedPositions.count(connectedRoomNumber) ? roomTypeToString(rooms[connectedRoomNumber].roomType) : "???";
            setString(StartX + 5, StartY + 6 + count, std::to_string(count) + ". ", BLUE);
            setString(StartX + 5 + 3, StartY + 6 + count, roomInfo + "  (" + std::to_string(rooms[connectedRoomNumber].position.first) + ", " + std::to_string(rooms[connectedRoomNumber].position.second) + ")", BLUE);
            count++;
        }
        CONNECTED_ROOMS = count;
        setString(StartX + 5, StartY + 6 + count + 1, "-> Select a Room No (0, 1, ..) & -1 to Exit: ", BOLD_GREEN);
        setString(StartX, StartY + 6 + count + 3, "______________________________________________________________", BOLD_YELLOW);
    }

    void initializeFight()
    {
        system("cls");
        clear();

        setString(40, 3, L"Player Health:                    Enemy Health:            ");
        setString(30, 15, "\\__________________\\__________________\\________________\\________________\\_________________\\", GREY);
        setString(40, 20, L" ╔                   ╗                     ╔                   ╗", WBOLD_BLUE);
        setString(40, 21, L" ║                   ║                     ║                   ║", WBOLD_BLUE);
        setString(40, 22, L" ║                   ║                     ║                   ║", WBOLD_BLUE);
        setString(40, 23, L" ║                   ║                     ║                   ║", WBOLD_BLUE);
        setString(40, 24, L" ║                   ║                     ║                   ║", WBOLD_BLUE);
        setString(40, 25, L" ║                   ║                     ║                   ║", WBOLD_BLUE);
        setString(40, 26, L" ║                   ║                     ║                   ║", WBOLD_BLUE);
        setString(40, 27, L" ╚═══════════════════╝                     ╚═══════════════════╝", WBOLD_BLUE);
        setString(40, 28, "  Player Attack Stack                        Enemy Attack Stack", BOLD_BLUE);
        setString(0, 32, "_________________________________________________________________________________________________________________________________________________", BOLD_BLUE);
        setString(26, 34, L"𝐀𝐕𝐀𝐋𝐀𝚰𝐁𝐋𝐄 𝐂𝐀𝐑𝐃𝐒                   |                   𝐌𝐄𝐍𝐔                  |            𝐂𝐎𝐍𝐒𝐎𝐋𝐄  ", WBOLD_BLUE);

        setString(87, 36, L"1. Move Forward", WGREEN);
        setString(87, 37, L"2. Move Backward", WGREEN);
        setString(87, 38, L"3. Add Card To Attack Stack", WGREEN);
        setString(87, 39, L"4. Attack With Current Stack", WGREEN);
        setString(87, 40, L"5. Change Direction", WGREEN);
        setString(87, 41, L"6. Pass Turn", WGREEN);
    }

    void clearConsole()
    {
        for (int i = 130; i < SCREEN_WIDTH; ++i)
        {
            for (int j = 36; j < SCREEN_HEIGHT; ++j)
            {
                setChar(i, j, ' ');
                setChar(i, j, L' ');
            }
        }
    }

    int takeRoomChoice()
    {
        int choice = -2;
        std::string input;
        gotoxy(95 + 50, 2 + 6 + CONNECTED_ROOMS + 1);
        std::getline(std::cin, input);
        std::stringstream stream(input);
        stream >> choice;
        return choice;
    }

    int TakeLargeRoomTurn()
    {
        int choice = -2;
        std::string input;
        std::cout << BOLD_MAGENTA;
        gotoxy(120, 36);
        std::getline(std::cin, input);
        std::cout << RESET;
        std::stringstream stream(input);
        stream >> choice;
        return choice;
    }

    void EnemyTurn()
    {
        int randomNum = rand() % 6 + 5;
        for (int i = 0; i < randomNum; ++i)
        {
            clearConsole();
            setString(130, 36, L"-> Enemy's Turn", WBOLD_RED);
            for (int j = 0; j < randomNum; ++j)
            {
                setChar(130 + 15 + i, 36, L'∙', WBOLD_RED);
            }
            print();
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
        }
    }

    void EnemyTurnUpdate(std::string message)
    {
        clearConsole();
        setString(130, 36, L"-> Enemy's Turn", WBOLD_RED);
        if (message == "Cooldown")
        {
            setString(130, 38, L"-> Enemy on CoolDown!", WBOLD_RED);
            setString(130, 39, L"-> MAKE YOUR MOVE!!", WBOLD_RED);
            setString(130, 40, L"-> HURRY UP!!!", WBOLD_RED);
        }
        else
        {
            setString(130, 38, std::wstring(message.begin(), message.end()), WBOLD_RED);
        }
        print();
    }

    void updatePlayerAttackStack(std::vector<std::string> playerAttackStack)
    {
        int y = 26;

        // Clear the previous attack stack
        for (int i = 0; i < 7; ++i)
        {
            if (i == 6)
            {
                setString(41, y - i, L"╔                   ╗", WBOLD_BLUE);
            }
            else
            {
                setString(41, y - i, L"║                   ║", WBOLD_BLUE);
            }
        }

        if (playerAttackStack.empty())
        {
            return;
        }

        std::reverse(playerAttackStack.begin(), playerAttackStack.end());

        for (int i = 0; i < playerAttackStack.size(); ++i)
        {
            if (playerAttackStack[i].size() <= 4)
            {
                for (int j = 0; j < playerAttackStack[i].size(); ++j)
                {
                    setChar(50 + j, y, static_cast<wchar_t>(playerAttackStack[i][j]));
                }
            }
            else
            {
                for (int j = 0; j < playerAttackStack[i].size(); ++j)
                {
                    setChar(46 + j, y, static_cast<wchar_t>(playerAttackStack[i][j]));
                }
            }
            y--;
        }
    }
    void updateEnemyAttackStack(std::vector<std::string> enemyAttackStack)
    {
        int y = 26;

        // Clear the previous attack stack
        for (int i = 0; i < 7; ++i)
        {
            if (i == 6)
            {
                setString(83, y - i, L"╔                   ╗", WBOLD_BLUE);
            }
            else
            {
                setString(83, y - i, L"║                   ║", WBOLD_BLUE);
            }
        }

        if (enemyAttackStack.empty())
        {
            return;
        }

        std::reverse(enemyAttackStack.begin(), enemyAttackStack.end());

        for (int i = 0; i < enemyAttackStack.size(); ++i)
        {
            if (enemyAttackStack[i].size() <= 4)
            {
                for (int j = 0; j < enemyAttackStack[i].size(); ++j)
                {
                    setChar(92 + j, y, static_cast<wchar_t>(enemyAttackStack[i][j]));
                }
            }
            else
            {
                for (int j = 0; j < enemyAttackStack[i].size(); ++j)
                {
                    setChar(88 + j, y, static_cast<wchar_t>(enemyAttackStack[i][j]));
                }
            }
            y--;
        }
    }
    void updateAvailableCards(std::vector<std::string> cards)
    {
        int x = 26;
        int y = 35;

        // Clear the previous available cards
        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                setString(x + j * 20, y + i, L"                 ");
            }
        }

        int padding = 12;
        // Print each card name
        for (size_t i = 0; i < cards.size(); ++i)
        {
            std::wstring cardName(cards[i].begin(), cards[i].end());
            std::wstring paddedCardName = cardName + std::wstring(padding - cardName.size(), L' ') + L"│";
            setString(x, y, L"┌─┬─────────────┐", WBOLD_YELLOW);
            setString(x, y + 1, L"│" + std::to_wstring(i + 1) + L"│ " + paddedCardName, WBOLD_YELLOW);
            setString(x, y + 2, L"└─┴─────────────┘", WBOLD_YELLOW);
            x += 20;
            if ((i + 1) % 3 == 0)
            {
                x = 26;
                y += 3;
            }
        }
        for (int i = 35; i < SCREEN_HEIGHT; ++i)
        {
            setString(85, i, L"|", WBOLD_BLUE);
            setString(127, i, L"|", WBOLD_BLUE);
        }
    }
    void updateTiles(std::vector<std::string> tiles, int playerDirection, int enemyDirection)
    {
        int playerPosition = -1;
        int enemyPosition = -1;

        // Determine the positions of the player and enemy based on the tiles
        for (int i = 0; i < tiles.size(); ++i)
        {
            if (tiles[i] == "P")
            {
                playerPosition = i;
            }
            else if (tiles[i] == "E")
            {
                enemyPosition = i;
            }
        }

        // Clear the previous tiles
        for (int i = TILE_START; i < TILE_END; i += TILE_DIFF_X)
        {
            setString(i, 11, L"                               ");
            setString(i, 12, L"                               ");
            setString(i, 13, L"                               ");
            setString(i, 14, L"                               ");
            setString(i, 16, L"                               ");
        }

        // Reset player and enemy positions
        int currentTileX = TILE_START;

        for (int count = 0; count < tiles.size(); ++count, currentTileX += TILE_DIFF_X)
        {
            // Render the player if the current tile matches the player's position
            if (count == playerPosition)
            {
                P_POSITION = currentTileX + 8;
                if (playerDirection == 1)
                { // Facing right
                    P_DIRECTION = 1;
                    setString(currentTileX + 8, 12, L" O ", WGREEN);
                    setString(currentTileX + 8, 13, L"/|\\", WGREEN);
                    setString(currentTileX + 8, 14, L"/ \\", WGREEN);
                    setChar(currentTileX + 8 + 1, 16, L'▶', WGREEN);
                }
                else
                { // Facing left
                    P_DIRECTION = -1;
                    setString(currentTileX + 8, 12, L" O ", WGREEN);
                    setString(currentTileX + 8, 13, L"/|\\", WGREEN);
                    setString(currentTileX + 8, 14, L"/ \\", WGREEN);
                    setChar(currentTileX + 8 + 1, 16, L'◀', WGREEN);
                }
            }

            // Render the enemy if the current tile matches the enemy's position
            if (count == enemyPosition)
            {
                E_POSITION = currentTileX + 8;
                if (enemyDirection == 1)
                { // Facing right
                    E_DIRECTION = 1;
                    setString(currentTileX + 8, 11, L" _)\\_∞", WRED);
                    setString(currentTileX + 8, 12, L"// }|", WRED);
                    setString(currentTileX + 8, 13, L"( `-|", WRED);
                    setString(currentTileX + 8, 14, L" / \\", WRED);
                    setChar(currentTileX + 8 + 2, 16, L'▶', WRED);
                }
                else
                { // Facing left
                    E_DIRECTION = -1;
                    setString(currentTileX + 8, 11, L"∞_/(_", WRED);
                    setString(currentTileX + 8, 12, L"|{ \\", WRED);
                    setString(currentTileX + 8, 13, L"|-´ )", WRED);
                    setString(currentTileX + 8, 14, L" /  \\", WRED);
                    setChar(currentTileX + 8 + 2, 16, L'◀', WRED);
                }
            }
        }
    }

    void updatePlayerHP(int hp, int maxHP)
    {
        int filledBlocks = hp / 2;
        for (int i = 0; i < maxHP / 2; ++i)
        {
            if (i < filledBlocks)
            {
                setChar(55 + i, 3, L'█', WGREEN);
            }
            else
            {
                setChar(55 + i, 3, L'█', WGREY);
            }
        }
    }

    void updateEnemyHP(int hp, int maxHP)
    {
        int filledBlocks = hp;
        for (int i = 0; i < maxHP; ++i)
        {
            if (i < filledBlocks)
            {
                setChar(88 + i, 3, L'█', WRED);
            }
            else
            {
                setChar(88 + i, 3, L'█', WGREY);
            }
        }
    }

    /****************************************ANIMATIONS***************************************** */

    void SingleSlashAnimation(std::string character)
    {
        int yAddition = 3;
        int frames = 6;
        int x, y;

        if (character == "P")
        {
            if (P_DIRECTION == 1)
            {
                x = P_POSITION + TILE_DIFF_X;
            }
            else
            {
                x = P_POSITION - TILE_DIFF_X;
            }
        }
        else
        {
            if (E_DIRECTION == 1)
            {
                x = E_POSITION + TILE_DIFF_X;
            }
            else
            {
                x = E_POSITION - TILE_DIFF_X;
            }
        }

        for (int i = 0; i < frames; ++i)
        {
            y = 4 + i;
            // Clear the previous frame
            if (i > 0)
            {
                setString(x, y - 1, L"   ");
                setString(x, y, L"    ");
                setString(x, y + 1, L"   ");
                setString(x, y + 2, L"   ");
                setString(x, y + 3, L"   ");
            }

            // Draw the new frame
            setString(x, y, L" )(", WGREEN);
            setString(x, y + 1, L"o==o", WGREEN);
            setString(x, y + 2, L" ||", WGREEN);
            setString(x, y + 3, L" ||", WGREEN);
            setString(x, y + 4, L" \\/", WGREEN);

            print();
            Sleep(100);
        }

        // Clear the final frame
        setString(x, y, L"   ");
        setString(x, y + 1, L"    ");
        setString(x, y + 2, L"   ");
        setString(x, y + 3, L"   ");
        setString(x, y + 4, L"   ");
    }

    void DoubleSlashAnimation(std::string character)
    {
        int yAddition = 3;
        int frames = 6;
        int leftX, rightX, y;

        if (character == "P")
        {
            if (P_DIRECTION == 1)
            {
                leftX = P_POSITION - TILE_DIFF_X;
                rightX = P_POSITION + TILE_DIFF_X;
            }
            else
            {
                leftX = P_POSITION + TILE_DIFF_X;
                rightX = P_POSITION - TILE_DIFF_X;
            }
        }
        else
        {
            if (E_DIRECTION == 1)
            {
                leftX = E_POSITION - TILE_DIFF_X;
                rightX = E_POSITION + TILE_DIFF_X;
            }
            else
            {
                leftX = E_POSITION + TILE_DIFF_X;
                rightX = E_POSITION - TILE_DIFF_X;
            }
        }

        for (int i = 0; i < frames; ++i)
        {
            y = 4 + i;
            // Clear the previous frame
            if (i > 0)
            {
                setString(leftX, y - 1, L"   ");
                setString(leftX, y, L"    ");
                setString(leftX, y + 1, L"    ");
                setString(leftX, y + 2, L"    ");
                setString(leftX, y + 3, L"    ");
                setString(leftX, y + 4, L"    ");

                setString(rightX, y - 1, L"   ");
                setString(rightX, y, L"    ");
                setString(rightX, y + 1, L"    ");
                setString(rightX, y + 2, L"    ");
                setString(rightX, y + 3, L"    ");
                setString(rightX, y + 4, L"    ");
            }

            // Draw the new frame
            setString(leftX, y, L" )(", WGREEN);
            setString(leftX, y + 1, L"o==o", WGREEN);
            setString(leftX, y + 2, L" ||", WGREEN);
            setString(leftX, y + 3, L" ||", WGREEN);
            setString(leftX, y + 4, L" \\/", WGREEN);

            setString(rightX, y, L" )(", WGREEN);
            setString(rightX, y + 1, L"o==o", WGREEN);
            setString(rightX, y + 2, L" ||", WGREEN);
            setString(rightX, y + 3, L" ||", WGREEN);
            setString(rightX, y + 4, L" \\/", WGREEN);

            print();
            Sleep(100);
        }

        // Clear the final frame
        setString(leftX, y, L"   ");
        setString(leftX, y + 1, L"    ");
        setString(leftX, y + 2, L"   ");
        setString(leftX, y + 3, L"   ");
        setString(leftX, y + 4, L"   ");

        setString(rightX, y, L"   ");
        setString(rightX, y + 1, L"    ");
        setString(rightX, y + 2, L"   ");
        setString(rightX, y + 3, L"   ");
        setString(rightX, y + 4, L"   ");
    }

    void BowAnimation(std::string character)
    {
        int fromX, toX, y;
        int direction;

        if (character == "P")
        {
            direction = P_DIRECTION;
            fromX = P_POSITION;
            toX = E_POSITION;
        }
        else
        {
            direction = E_DIRECTION;
            fromX = E_POSITION;
            toX = P_POSITION;
        }

        std::vector<std::vector<std::string>> frames = readFramesFromFile(direction == 1 ? "Sprites/bow_right.txt" : "Sprites/bow_left.txt");
        std::vector<std::string> arrow = readFramesFromFile(direction == 1 ? "Sprites/arrow_right.txt" : "Sprites/arrow_left.txt")[0];

        // Display the bow at the player's position
        int bowHeight = frames[0].size();
        y = 15 - bowHeight;
        // print the bow animation frames
        for (const auto &frame : frames)
        {
            // Clear the previous frame
            if (&frame != &frames[0])
            {
                for (int i = 0; i < frames[0].size(); ++i)
                {
                    setString(fromX, y + i, L"                    ");
                }
            }
            // Draw the new frame
            for (int i = 0; i < frame.size(); ++i)
            {
                setString(fromX, y + i, std::wstring(frame[i].begin(), frame[i].end()), WGREEN);
            }
            print();
            Sleep(500);
        }
        for (const auto &line : frames[0])
        {
            setString(fromX, y, std::wstring(line.begin(), line.end()), WGREEN);
            y++;
        }
        print();

        if (direction == 1)
        {
            int x = fromX + frames[0][3].size();
            y = 15 - (bowHeight / 2) - 1;

            // Move the arrow until it reaches the target position
            while (x < toX)
            {
                // Draw the arrow
                for (int i = 0; i < arrow.size(); ++i)
                {
                    setString(x, y + i, std::wstring(arrow[i].begin(), arrow[i].end()), WGREEN);
                }
                print();
                Sleep(50);

                // Clear the previous arrow frame
                for (int i = 0; i < arrow.size(); ++i)
                {
                    setString(x, y, L"   ");
                }
                print();

                x += 2; // Move the arrow to the right
            }
        }
        else
        {
            int x = fromX - 1;
            y = 15 - (bowHeight / 2) - 1;

            // Move the arrow until it reaches the target position
            while (x > toX)
            {
                // Draw the arrow
                for (int i = 0; i < arrow.size(); ++i)
                {
                    setString(x, y + i, std::wstring(arrow[i].begin(), arrow[i].end()), WGREEN);
                }
                print();
                Sleep(50);

                // Clear the previous arrow frame
                for (int i = 0; i < SCREEN_WIDTH; ++i)
                {
                    setString(i, y, L"   ");
                }
                print();

                x -= 2; // Move the arrow to the left
            }
        }

        clearPlayerEnemy();
        print();
    }

    bool isFacingEachOther(int playerPosition, int playerDirection, int enemyPosition, int enemyDirection)
    {
        if (playerDirection == 1 && enemyDirection == -1)
        {
            return playerPosition < enemyPosition;
        }
        else if (playerDirection == -1 && enemyDirection == 1)
        {
            return playerPosition > enemyPosition;
        }
        return false;
    }

    struct CharInfo
    {
        wchar_t ch;
        int x;
        int y;
    };
    void DashAnimation(std::string character)
    {
        if (!isFacingEachOther(P_POSITION, P_DIRECTION, E_POSITION, E_DIRECTION))
        {
            return;
        }
        int fromX, toX, y;
        int direction;

        std::vector<std::vector<std::string>> player;
        std::vector<std::vector<std::string>> enemy;
        std::vector<CharInfo> pattern;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        if (character == "P")
        {
            direction = P_DIRECTION;
            if (direction == 1)
            {
                player = readFramesFromFile("Sprites/player_right.txt");
            }
            else
            {
                player = readFramesFromFile("Sprites/player_left.txt");
            }
            fromX = P_POSITION;
            toX = E_POSITION - TILE_DIFF_X * direction;
        }
        else
        {
            direction = E_DIRECTION;
            if (direction == 1)
            {
                enemy = readFramesFromFile("Sprites/enemy_right.txt");
            }
            else
            {
                enemy = readFramesFromFile("Sprites/enemy_left.txt");
            }
            fromX = E_POSITION;
            toX = P_POSITION - TILE_DIFF_X * direction;
        }

        clearPlayerEnemy();
        print();

        if (character == "P")
        {
            int frameIndex = 0;
            for (int i = fromX; i != toX; i += direction)
            {
                clearPlayerEnemy();
                for (int j = 0; j < player[frameIndex].size(); ++j)
                {
                    setString(i, 15 - player[frameIndex].size() + j, std::wstring(player[frameIndex][j].begin(), player[frameIndex][j].end()), WGREEN);
                    int d = direction == 1 ? -3 : 3;
                    CharInfo charInfo;
                    charInfo.ch = (dis(gen) == 0) ? L' ' : L'.';
                    charInfo.x = i + d;
                    charInfo.y = 15 - player[frameIndex].size() + j;
                    pattern.push_back(charInfo);
                }
                // print the pattern
                for (const auto &charInfo : pattern)
                {
                    setChar(charInfo.x, charInfo.y, charInfo.ch, WGREEN);
                }
                print();
                Sleep(50);
                frameIndex = (frameIndex + 1) % player.size(); // Move to the next frame, loop back to the first frame if at the end
            }
            P_POSITION = toX; // Update player position
        }
        else
        {
            int frameIndex = 0;
            for (int i = fromX; i != toX; i += direction)
            {
                clearPlayerEnemy();
                for (int j = 0; j < enemy[frameIndex].size(); ++j)
                {
                    setString(i, 15 - enemy[frameIndex].size() + j, std::wstring(enemy[frameIndex][j].begin(), enemy[frameIndex][j].end()), WRED);
                    int d = direction == 1 ? -3 : 3;
                    CharInfo charInfo;
                    charInfo.ch = (dis(gen) == 0) ? L' ' : L'.';
                    charInfo.x = i + d;
                    charInfo.y = 15 - enemy[frameIndex].size() + j;
                    pattern.push_back(charInfo);
                }
                // print the pattern
                for (const auto &charInfo : pattern)
                {
                    setChar(charInfo.x, charInfo.y, charInfo.ch, WGREEN);
                }
                print();
                Sleep(50);
                frameIndex = (frameIndex + 1) % enemy.size(); // Move to the next frame, loop back to the first frame if at the end
            }
            E_POSITION = toX; // Update enemy position
        }
        clearPlayerEnemy();
        print();
        Sleep(100);
    }

    void ShadowStepAnimation(std::string character)
    {
        if (!isFacingEachOther(P_POSITION, P_DIRECTION, E_POSITION, E_DIRECTION))
        {
            return;
        }
        int fromX, toX, y;
        int direction;

        std::vector<std::vector<std::string>> player;
        std::vector<std::vector<std::string>> enemy;
        std::vector<std::string> player_jump = readFramesFromFile("Sprites/player_jump.txt")[0];
        std::vector<std::string> player_fall = readFramesFromFile("Sprites/player_fall.txt")[0];
        std::vector<std::string> player_jump_direction;
        std::vector<CharInfo> pattern(enemy.size());

        // Random number generation setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        if (character == "P")
        {
            direction = P_DIRECTION;
            if (direction == 1)
            {
                player = readFramesFromFile("Sprites/player_right.txt");
                player_jump_direction = readFramesFromFile("Sprites/player_jump_right.txt")[0];
            }
            else
            {
                player = readFramesFromFile("Sprites/player_left.txt");
                player_jump_direction = readFramesFromFile("Sprites/player_jump_left.txt")[0];
            }
            fromX = P_POSITION;
            toX = E_POSITION - TILE_DIFF_X * direction;
        }
        else
        {
            direction = E_DIRECTION;
            if (direction == 1)
            {
                enemy = readFramesFromFile("Sprites/enemy_right.txt");
            }
            else
            {
                enemy = readFramesFromFile("Sprites/enemy_left.txt");
            }
            fromX = E_POSITION;
            toX = P_POSITION - TILE_DIFF_X * direction;
        }

        clearPlayerEnemy();
        print();

        if (character == "P")
        {
            if (abs(fromX - toX) > TILE_DIFF_X)
            {
                int frameIndex = 0;
                for (int i = fromX; i != toX; i += direction)
                {
                    clearPlayerEnemy();
                    for (int j = 0; j < player[frameIndex].size(); ++j)
                    {
                        setString(i, 15 - player[frameIndex].size() + j, std::wstring(player[frameIndex][j].begin(), player[frameIndex][j].end()), WGREEN);
                        int d = direction == 1 ? -3 : 3;
                        CharInfo charInfo;
                        charInfo.ch = (dis(gen) == 0) ? L' ' : L'.';
                        charInfo.x = i + d;
                        charInfo.y = 15 - player[frameIndex].size() + j;
                        pattern.push_back(charInfo);
                    }
                    // print the pattern
                    for (const auto &charInfo : pattern)
                    {
                        setChar(charInfo.x, charInfo.y, charInfo.ch, WGREEN);
                    }
                    print();
                    Sleep(50);
                    frameIndex = (frameIndex + 1) % player.size(); // Move to the next frame, loop back to the first frame if at the end
                }
                P_POSITION = toX; // Update player position
            }
        }
        clearPlayerEnemy();
        print();
        Sleep(100);
    }

    void StartingAnimation(int startY)
    {
        std::vector<std::vector<std::string>> frames = readFramesFromFile("Sprites/starting.txt");

        for (const auto &frame : frames)
        {
            clear();
            int y = startY - frame.size() / 2;
            for (const auto &line : frame)
            {
                int x = 0;
                for (const auto &ch : line)
                {
                    if (ch == '\\')
                        setChar(x++, y, ch, WGREY);
                    else
                        setChar(x++, y, ch, WBOLD_WHITE);
                }
                y++;
            }
            print();
            Sleep(200);
        }
    }

    void LargeRoomAnimation()
    {
        std::vector<std::vector<std::string>> playerFrames = readFramesFromFile("Sprites/player_right.txt");

        const int groundLevel = 20;
        const int halfScreenWidth = SCREEN_WIDTH / 3;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        std::vector<std::wstring> groundPattern(SCREEN_WIDTH);
        for (int i = 0; i < SCREEN_WIDTH; ++i)
        {
            groundPattern[i] = (dis(gen) == 0) ? L"-" : L" ";
        }

        // Player running animation
        int frameIndex = 0;
        for (int x = 0; x < halfScreenWidth; x += 1)
        {
            // Flicker Effect
            if (x == halfScreenWidth - 4 || x == halfScreenWidth - 12 || x == halfScreenWidth - 20)
            {
                clear();
                initializeFight();
                print();
                Sleep(100);
                continue;
            }
            clear();

            // Print the ground pattern above the player
            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerFrames[0].size() - 2, groundPattern[i], WGREY);
            }

            // Set the buffer for player frames
            for (int i = 0; i < playerFrames[frameIndex].size(); ++i)
            {
                setString(x, groundLevel - playerFrames[frameIndex].size() + i, std::wstring(playerFrames[frameIndex][i].begin(), playerFrames[frameIndex][i].end()), WBOLD_WHITE);
            }

            // Print the ground pattern below the player
            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerFrames[0].size() + 5, groundPattern[i], WGREY);
            }

            // Print the buffer
            print();
            Sleep(50);

            // Move to the next frame, loop back to the first frame if at the end
            frameIndex = (frameIndex + 1) % playerFrames.size();
        }
    }
    void TrapRoomAnimation()
    {
        std::random_device rd1;
        std::mt19937 gen1(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<> dis1(SCREEN_WIDTH / 3, 2 * SCREEN_WIDTH / 3);

        std::vector<std::vector<std::string>> monsterFrames = readFramesFromFile("Sprites/monster.txt");
        std::vector<std::vector<std::string>> playerFrames = readFramesFromFile("Sprites/player_right.txt");

        const int groundLevel = 20;
        const int animationX = dis1(gen1);

        std::random_device rd2;
        std::mt19937 gen2(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<> dis2(0, 1);

        std::vector<std::wstring> groundPattern(SCREEN_WIDTH);
        for (int i = 0; i < SCREEN_WIDTH; ++i)
        {
            groundPattern[i] = (dis2(gen2) == 0) ? L"-" : L" ";
        }

        int frameIndex = 0;
        for (int x = 0; x < animationX; x += 1)
        {
            clear();

            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerFrames[0].size() - 2, groundPattern[i], WGREY);
            }

            for (int i = 0; i < playerFrames[frameIndex].size(); ++i)
            {
                setString(x, groundLevel - playerFrames[frameIndex].size() + i, std::wstring(playerFrames[frameIndex][i].begin(), playerFrames[frameIndex][i].end()), WBOLD_WHITE);
            }

            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerFrames[0].size() + 5, groundPattern[i], WGREY);
            }

            print();
            Sleep(50);

            frameIndex = (frameIndex + 1) % playerFrames.size();
        }

        // Monster animation
        for (const auto &frame : monsterFrames)
        {
            clear();

            // Print the ground pattern above the monster according to player frame size
            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerFrames[0].size() - 2, groundPattern[i]);
            }

            // Set the buffer for monster frames
            for (int i = 0; i < frame.size(); ++i)
            {
                setString(animationX - 10, groundLevel - frame.size() + i, std::wstring(frame[i].begin(), frame[i].end()), WBOLD_RED);
            }

            // Print the ground pattern below the monster according to player frame size
            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerFrames[0].size() + 5, groundPattern[i]);
            }

            print();
            Sleep(100);
        }
    }

    void TreasureRoomAnimation()
    {
        std::vector<std::vector<std::string>> playerRightFrames = readFramesFromFile("Sprites/player_right.txt");
        std::vector<std::vector<std::string>> playerLeftFrames = readFramesFromFile("Sprites/player_left.txt");
        std::vector<std::string> treasureFrames = readFramesFromFile("Sprites/treasure.txt")[0];
        std::vector<std::string> playerJumpFrames = readFramesFromFile("Sprites/player_jump.txt")[0];

        const int groundLevel = 20;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        std::uniform_int_distribution<> treasurePosDis(5, SCREEN_WIDTH / 3);

        std::vector<std::wstring> groundPattern(SCREEN_WIDTH);
        for (int i = 0; i < SCREEN_WIDTH; ++i)
        {
            groundPattern[i] = (dis(gen) == 0) ? L"-" : L" ";
        }

        // Determine the treasure position
        int treasureX = treasurePosDis(gen);

        int frameIndex = 0;
        for (int x = 0; x < SCREEN_WIDTH / 2; x += 1)
        {
            clear();

            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerRightFrames[0].size() - 2, groundPattern[i], WGREY);
            }

            for (int i = 0; i < playerRightFrames[frameIndex].size(); ++i)
            {
                setString(x, groundLevel - playerRightFrames[frameIndex].size() + i, std::wstring(playerRightFrames[frameIndex][i].begin(), playerRightFrames[frameIndex][i].end()), WBOLD_WHITE);
            }

            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerRightFrames[0].size() + 5, groundPattern[i], WGREY);
            }

            print();
            Sleep(50);

            frameIndex = (frameIndex + 1) % playerRightFrames.size();
        }

        // Print the treasure
        for (int i = 0; i < treasureFrames.size(); ++i)
        {
            setString(treasureX, groundLevel - treasureFrames.size() + i, std::wstring(treasureFrames[i].begin(), treasureFrames[i].end()), WBOLD_YELLOW);
        }
        print();
        Sleep(500);

        // Player running animation to the left towards the treasure
        frameIndex = 0;
        for (int x = SCREEN_WIDTH / 2; x > treasureX + 20; x -= 1)
        {
            clear();

            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerLeftFrames[0].size() - 2, groundPattern[i], WGREY);
            }

            for (int i = 0; i < playerLeftFrames[frameIndex].size(); ++i)
            {
                setString(x, groundLevel - playerLeftFrames[frameIndex].size() + i, std::wstring(playerLeftFrames[frameIndex][i].begin(), playerLeftFrames[frameIndex][i].end()), WBOLD_WHITE);
            }

            for (int i = 0; i < SCREEN_WIDTH; ++i)
            {
                setString(i, groundLevel - playerLeftFrames[0].size() + 5, groundPattern[i], WGREY);
            }

            for (int i = 0; i < treasureFrames.size(); ++i)
            {
                setString(treasureX, groundLevel - treasureFrames.size() + i, std::wstring(treasureFrames[i].begin(), treasureFrames[i].end()), WBOLD_YELLOW);
            }

            print();
            Sleep(50);

            frameIndex = (frameIndex + 1) % playerLeftFrames.size();
        }

        // print player at the treasure
        setString(treasureX + 30, groundLevel - playerLeftFrames[0].size(), std::wstring(playerLeftFrames[0][0].begin(), playerLeftFrames[0][0].end()), WBOLD_WHITE);
        Sleep(1000);
    }

    void InitializeInstructionsWindow()
    {
        clear();
        setString(18, 3, " ____                                                                                                            ____ ");
        setString(18, 4, "( __ )                                                                                                          ( __ )");
        setString(18, 5, " |  |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|  | ");
        setString(18, 6, " |  |                                                                                                            |  | ");
        setString(18, 7, " |  |                                                                                                            |  | ");
        setString(18, 8, " |  |                                                                                                            |  | ");
        setString(18, 9, " |  |                                                                                                            |  | ");
        setString(18, 10, " |  |                                                                                                            |  | ");
        setString(18, 11, " |  |                                                                                                            |  | ");
        setString(18, 12, " |  |                                                                                                            |  | ");
        setString(18, 13, " |  |                                                                                                            |  | ");
        setString(18, 14, " |  |                                                                                                            |  | ");
        setString(18, 15, " |  |                                                                                                            |  | ");
        setString(18, 16, " |  |                                                                                                            |  | ");
        setString(18, 17, " |  |                                                                                                            |  | ");
        setString(18, 18, " |  |                                                                                                            |  | ");
        setString(18, 19, " |  |                                                                                                            |  | ");
        setString(18, 20, " |  |                                                                                                            |  | ");
        setString(18, 21, " |  |                                                                                                            |  | ");
        setString(18, 22, " |  |                                                                                                            |  | ");
        setString(18, 23, " |  |                                                                                                            |  | ");
        setString(18, 24, " |  |                                                                                                            |  | ");
        setString(18, 25, " |  |                                                                                                            |  | ");
        setString(18, 26, " |  |                                                                                                            |  | ");
        setString(18, 27, " |  |                                                                                                            |  | ");
        setString(18, 28, " |  |                                                                                                            |  | ");
        setString(18, 29, " |  |                                                                                                            |  | ");
        setString(18, 30, " |  |                                                                                                            |  | ");
        setString(18, 31, " |  |                                                                                                            |  | ");
        setString(18, 32, " |  |                                                                                                            |  | ");
        setString(18, 33, " |  |                                                                                                            |  | ");
        setString(18, 34, " |  |                                                                                                            |  | ");
        setString(18, 35, " |  |                                                                                                            |  | ");
        setString(18, 36, " |__|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|__| ");
        setString(18, 37, "(____)                                                                                                          (____)");
        printTextInsideInstructionsOneWordAtATime();
        Sleep(3000);
    }

    void printTextInsideInstructionsOneWordAtATime()
    {
        std::vector<std::string> paragraphs = {
            "Welcome to 'Going Up'! You find yourself trapped in an ancient dungeon after a violent typhoon struck your town. The storm caused a massive power outage, plunging the entire area into darkness. Seeking shelter, you stumble upon a hidden entrance to a mysterious underground labyrinth. With no other options, you decide to venture inside.",
            "The dungeon is filled with treacherous traps, fearsome enemies, and valuable treasures. Your goal is to navigate through the dungeon's many rooms, each with its own unique challenges, and reach the top floor to find a way back to the surface. Along the way, you must gather resources, fight enemies, and solve puzzles to survive.",
            "Can you overcome the dangers of the dungeon and escape to safety? Good luck!"};

        int x = 25;
        int y = 8;
        for (const auto &paragraph : paragraphs)
        {
            std::istringstream iss(paragraph);
            std::string word;
            std::string line;
            while (iss >> word)
            {
                if (line.size() + word.size() + 1 > 100)
                {
                    setString(x, y++, line);
                    line.clear();
                }
                if (!line.empty())
                {
                    line += " ";
                }
                line += word;
                setString(x, y, line);
                print();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            if (!line.empty())
            {
                setString(x, y++, line);
                print();
            }
            y++;
        }
    }
};