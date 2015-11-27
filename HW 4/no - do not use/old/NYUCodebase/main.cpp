/*

CS-UY 3113 - Intro to Game Programming
Prof. Ivan Safrin

Sergey Smirnov
sas1030 / N15186921
smirnov@nyu.edu

Assignment 4 - Platformer

*/

#include "Platformer.h"

int main(int argc, char *argv[])
{
    Platformer game;
    while (!game.update_and_render())
    {
        // run the game
    }
    return 0;
}
