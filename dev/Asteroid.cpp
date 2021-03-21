/*
 * \file Asteroid.cpp
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 19:09
 */

#include "Asteroid.h"

#include "utils/AssetsStorage.h"

#include <utils/RandomGenerator.h>

namespace space_shooter {

namespace {
constexpr const float SPEED = 1.6f;
}

Asteroid::Asteroid( sf::Vector2f position )
        : hpMax( 2 ),
          points( 5 ),
          hp( hpMax )
{
    sprite.setTexture( AssetsStorage::instance().get< sf::Texture >( textures::ASTEROID ) );
    auto scale = utils::RandomGenerator::get( 0.2f, 0.45f );
    sprite.setScale( sf::Vector2f{ scale, scale } );
    sprite.setPosition( position );

    damage = scale > 0.4 ? 2 : 1;
    //TODO make points, hp dynamic. Based on scale
}

void Asteroid::update( float dt )
{
    sprite.move( 0.f, dt * SPEED );
}

void Asteroid::render( sf::RenderTarget & window )
{
    window.draw( sprite );
}

sf::FloatRect Asteroid::getBounds() const
{
    return sprite.getGlobalBounds();
}

void Asteroid::takeDamage( int32_t damage )
{
    if( hp <= 0 )
    {
        return;
    } //TODO log warning

    hp -= damage;
    sprite.setColor( sf::Color::Red );
}

int32_t Asteroid::getHp() const noexcept
{
    return hp;
}

int32_t Asteroid::getPoints() const noexcept
{
    return points;
}

int32_t Asteroid::getDamage() const noexcept
{
    return damage;
}

}