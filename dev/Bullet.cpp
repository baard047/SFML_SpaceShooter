/*
 * \file Bullet.cpp
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 13:49
 */

#include "Bullet.h"

#include "utils/AssetsStorage.h"

namespace space_shooter {

Bullet::Bullet( const sf::Vector2f &position, sf::Vector2f direction, float ms )
        : direction( direction ),
          movementSpeed( ms )
{
    sprite.setTexture( AssetsStorage::instance().get< sf::Texture >( textures::BULLET ) );
    sprite.setPosition(position);
}

void Bullet::update( float dt )
{
    sprite.move( direction * movementSpeed * dt );
}

void Bullet::render( sf::RenderTarget & target )
{
    target.draw( sprite );
}

sf::FloatRect Bullet::getBounds() const
{
    return sprite.getGlobalBounds();
}

}