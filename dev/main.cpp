#include "Game.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    try
    {
        space_shooter::Game game;
        game.run();
    }
    catch ( const std::exception &error )
    {
        std::cerr << error.what() << "\n";
    }

    return 0;
}
