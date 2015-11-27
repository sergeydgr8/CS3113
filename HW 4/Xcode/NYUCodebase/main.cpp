//
// Platformer Assignment - CS 3113
// Sergey Smirnov - Fall 2015
// main.cpp
//

#include "Platformer.hpp"

int main(int argc, char *argv[])
{
    Platformer game;
    while (!game.run_game())
    {
        // run the game
    }
    SDL_Quit();
    return 0;
}
