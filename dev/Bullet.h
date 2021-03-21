/*
 * \file Bullet.h
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 13:49
 */

#pragma once

#include <SFML/Graphics.hpp>

namespace space_shooter {

class Bullet
{
public:
    Bullet( const sf::Vector2f &position, sf::Vector2f direction, float ms);

    void update( float dt );
    void render( sf::RenderTarget & window );

    constexpr static int32_t getDamage() noexcept { return damage; }

    sf::FloatRect getBounds() const;

private:
    sf::Vector2f direction;
    float movementSpeed;
    constexpr const static int32_t damage = 1;

    sf::Sprite sprite;
};

}