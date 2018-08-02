#include "Game.h"
#include <iostream>

int main()
{
    MazeCrisis::Game *game;
    try
    {
        game = new MazeCrisis::Game(1920, 1080);
        game->update();

        delete game;
        return 0;
    }
    catch (std::exception e)
    {
        std::cout << e.what();
        getchar();
        glfwTerminate();
    }
}