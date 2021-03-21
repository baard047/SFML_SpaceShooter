/*
 * \file HUD.h
 * \author : Bardashevsky A.K.
 * \date : 17.09.2020
 * \time : 9:11
 */

#pragma once

#include <array>

#include <SFML/Graphics.hpp>
#include "utils/Types.h"

namespace space_shooter {

class HUD
{
public:
    HUD() = default;

    void init( sf::Vector2u windowSize );

    void update( const Stats & stats );
    void render( sf::RenderTarget & window );

    void showGameOverBanner( sf::RenderTarget & window );

private:
    void updateFps();

private:
    enum Indexes : size_t
    {
        TotalBullets = 0,
        BulletsMissed,
        AsteroidDestroyed,
        Score
    };

private:
    std::array< sf::Text, 4 > statistics;
    sf::Sprite gameOverBanner;
    sf::Text gameRestartText;

    sf::Text fpsText;

    sf::Clock clock;
    int32_t fps = 0;
};

}