/*
 * \file Asteroid.h
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 19:09
 */

#pragma once

#include <SFML/Graphics.hpp>

namespace space_shooter {

class Asteroid
{
public:
    enum Collisions : uint_least8_t
    {
        None = 0,
        Got_Hit_By_A_Bullet,
        Damaged_Ship
    };

public:
    explicit Asteroid( sf::Vector2f position );

    void update( float dt );
    void render( sf::RenderTarget & window );

    void takeDamage( int32_t damage );

    int32_t getHp() const noexcept;
    int32_t getDamage() const noexcept;
    int32_t getPoints() const noexcept;
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;

    const int32_t hpMax;
    const int32_t points;
    int32_t hp;
    int32_t damage;
};

}