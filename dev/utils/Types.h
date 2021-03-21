/*
 * \file Types.h
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 14:28
 */

#pragma once

namespace space_shooter {

struct Stats
{
    int32_t totalBulletFired = 0;
    int32_t bulletsMissed = 0;
    int32_t asteroidsDestroyed = 0;
    int32_t score = 0;

    inline void clear()
    {
        totalBulletFired = bulletsMissed = asteroidsDestroyed = score = 0;
    }
};

namespace textures {

constexpr const auto BACKGROUND = "assets/textures/space.jpg";
constexpr const auto CURSOR = "assets/textures/cursor3.png";
constexpr const auto SHIP = "assets/textures/ship.png";
constexpr const auto BULLET = "assets/textures/bullet.png";
constexpr const auto ASTEROID = "assets/textures/asteroid.png";
constexpr const auto GAME_OVER = "assets/textures/gameOver.png";

}

namespace fonts {

constexpr const auto DOSIS_LIGHT = "assets/fonts/Dosis-Light.ttf";
constexpr const auto PIXEL_LETTERS = "assets/fonts/PixellettersFull.ttf";

}

}
