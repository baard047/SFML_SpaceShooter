/*
 * \file Ship.cpp
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 12:13
 */

#include "Ship.h"
#include "utils/AssetsStorage.h"

#include <cmath>

namespace {
constexpr const float PI = 3.14159265;
}

namespace space_shooter {

Ship::Ship( sf::Vector2u windowSize )
    : movementSpeed( 5.f ),
      maxHp( 100 ),
      hp( maxHp )
{
    sprite.setTexture( AssetsStorage::instance().get< sf::Texture >( textures::SHIP ) );
    sprite.scale( 0.1f, 0.1f );
//    sprite.setOrigin( (float)sprite.getTextureRect().width / 2, (float)sprite.getTextureRect().height / 2 );

    maxHpBarWidth = sprite.getGlobalBounds().width;
    hpBarBackground.setSize({ maxHpBarWidth, 10.f });
    hpBarBackground.setFillColor( sf::Color{ 50, 50, 50, 80 } );
    hpBarInner.setFillColor( sf::Color{ 250, 20, 20, 100 } );

    restart( windowSize );
}

void Ship::restart( sf::Vector2u windowSize )
{
    hp = maxHp;
    bulletsMissed = 0;
    bulletsFired = 0;

    float x = (float)windowSize.x / 2 - sprite.getGlobalBounds().width / 2;
    float y = windowSize.y - 200.f;

    sprite.setPosition( x , y );

    float centerOfShipX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
    float hpBarX = centerOfShipX - hpBarBackground.getSize().x / 2;
    float hpBarY = sprite.getPosition().y + sprite.getGlobalBounds().height + 5.f;

    hpBarBackground.setPosition( hpBarX, hpBarY );
    hpBarInner.setPosition( hpBarBackground.getPosition() );
    hpBarInner.setSize( hpBarBackground.getSize() );

    bullets.clear();
}

void Ship::move( float dirX, float dirY )
{
    const sf::Vector2f dir { movementSpeed * dirX, movementSpeed * dirY };

    sprite.move( dir );
    hpBarBackground.move( dir );
    hpBarInner.move( dir );
}

void Ship::fire()
{
    //bullet positioning on center of the ship
    float xPos = ( sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f ) - 5.f;
    sf::Vector2f pos { xPos, sprite.getPosition().y };

    bullets.emplace_back( pos, sf::Vector2f{ 0.f, -1.f }, movementSpeed * 1.5f );
    ++bulletsFired;
}

void Ship::update( float dt, sf::Vector2< int > mousePos )
{
//    float dx = static_cast<float>( mousePos.x ) - sprite.getPosition().x;
//    float dy = static_cast<float>( mousePos.y ) - sprite.getPosition().y;
//    float angle = std::atan2( dy, dx ) * ( 180 / PI );
//    sprite.setRotation( 90 + angle );

    for( auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
    {
        bulletIt->update( dt );

        if( isBulletOutsideOfTheScreen( *bulletIt ) )
        {
            bulletIt = bullets.erase( bulletIt );
            ++bulletsMissed;
        }
        else { ++bulletIt; }
    }
}

void Ship::render( sf::RenderTarget & target )
{
    target.draw( sprite );
    target.draw( hpBarBackground );
    target.draw( hpBarInner );
    for( auto &bullet : bullets ) { bullet.render( target ); }
}

Asteroid::Collisions Ship::checkCollisions( const Asteroid & asteroid )
{
    if( sprite.getGlobalBounds().intersects( asteroid.getBounds() ) )
    {
        decreaseHP( asteroid.getDamage() );
        return Asteroid::Collisions::Damaged_Ship;
    }

    for( auto bullet = bullets.begin(); bullet != bullets.end(); ++bullet )
    {
        if( bullet->getBounds().intersects( asteroid.getBounds() ) )
        {
            bullets.erase( bullet );
            return Asteroid::Collisions::Got_Hit_By_A_Bullet;
        }
    }

    return Asteroid::Collisions::None;
}

void Ship::decreaseHP( int32_t amount )
{
    if( hp <= 0 ) { throw std::logic_error( "Ship already destroyed!" ); }

    hp -= amount;
    float hpPercent = static_cast< float >( hp ) / static_cast< float >( maxHp );
    hpBarInner.setSize( { std::floor( maxHpBarWidth * hpPercent ), hpBarInner.getSize().y } );
}

bool Ship::isBulletOutsideOfTheScreen( const Bullet & bullet ) const noexcept
{
    //TODO
    return bullet.getBounds().top + bullet.getBounds().height < 0.f;
}

sf::FloatRect Ship::getBounds() const
{
    sf::Rect< float > bounds = sprite.getGlobalBounds();
    bounds.height += hpBarInner.getGlobalBounds().height * 2;
    return bounds;
}

}