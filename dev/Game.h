/*
 * \file Game.h
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 11:45
 */

#pragma once

#include <memory>
#include <string>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Ship.h"
#include "Asteroid.h"
#include "HUD.h"
#include "utils/Types.h"

namespace space_shooter {

class Game
{
public:
    enum GameState : uint8_t
    {
        Play = 0,
        GameOver
    };

public:
    Game();

public:
    void run();
    void update( float dt );
    void render();

private:
    void loadAssets();
    void initCustomCursor();

    void processEvents();
    void handleShipMovements( float dt );

    void spawnAsteroid();
    void updateAsteroids( float dt );

    void restartGame();

private:
    std::unique_ptr< sf::RenderWindow > window;
    sf::Sprite background;
    sf::Sprite cursor;

    HUD hud;
    Stats stats;

    GameState gameState = GameState::Play;
    sf::Clock asteroidsRespawnTimer;

    Ship::Ptr ship;
    std::list< Asteroid > asteroids;
};

}

