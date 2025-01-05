#include "Src/Logic.h"
#include "Src/UI.h"

int main() {
    fullsc();
    GameLogic game(1, 6, 8);
    game.MainLoop();
    return 0;
}
