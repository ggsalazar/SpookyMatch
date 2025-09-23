#include "Engine/Engine.h"

int main() {

    //Initialize
    Engine game("SpookyMatch!", 60);

    //Game loop
    while (game.running)
        game.Run();

    return 0;
}