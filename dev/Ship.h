/*
 * \file Ship.h
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 12:13
 */

#pragma once

#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Bullet.h"
#include "Asteroid.h"

namespace space_shooter {

class Ship
{
public:
    using Ptr = std::unique_ptr< Ship >;

public:
    explicit Ship( sf::Vector2u windowSize );

public:
    void update( float dt, sf::Vector2< int > mousePos );
    void render( sf::RenderTarget & window );

    void restart( sf::Vector2u windowSize );

    void move( float dirX, float dirY );
    void fire();
    Asteroid::Collisions checkCollisions( const Asteroid & asteroid );

    bool isAlive() const noexcept { return hp > 0; }
    sf::FloatRect getBounds() const;
    constexpr int32_t getDamage() const noexcept { return Bullet::getDamage(); }
    int32_t getTotalBulletsFired() const noexcept { return  bulletsFired; }
    int32_t getBulletsMissed() const noexcept { return bulletsMissed; }

private:
    void decreaseHP( int32_t amount );
    bool isBulletOutsideOfTheScreen( const Bullet & bullet ) const noexcept;

private:
    sf::Sprite sprite;

    std::list< Bullet > bullets;

    sf::RectangleShape hpBarBackground;
    sf::RectangleShape hpBarInner;
    const float maxHpBarWidth;

    const float movementSpeed;
    const int32_t maxHp;

    int32_t hp;
    int32_t bulletsFired = 0;
    int32_t bulletsMissed = 0;
};

}
