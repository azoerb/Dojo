#include "Controller.h"

int main() {
    Controller* game = new Controller();
    
    game->mainLoop();
    
    if (game) { delete game; }
    return EXIT_SUCCESS;
}
