/*
 * \file Ship.cpp
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 12:13
 */

#include "Ship.h"
#include "utils/AssetsStorage.h"
#include "utils/utils.h"

#include <cmath>

namespace {
constexpr const float PI = 3.14159265;
}

namespace space_shooter {

Ship::Ship( sf::Vector2u windowSize )
        : sprite( []()
                  {
                      sf::Sprite sprite{ AssetsStorage::instance().get< sf::Texture >( textures::SHIP ) };
                      sprite.setOrigin( utils::getCenterOfObject( sprite ) );
                      sprite.scale( 0.1f, 0.1f );
                      return sprite;
                  }() ),
          maxHpBarWidth( sprite.getGlobalBounds().width ),
          movementSpeed( 5.f ),
          maxHp( 5 ),
          hp( maxHp )
{
    hpBarBackground.setSize({ maxHpBarWidth, 10.f });
    hpBarBackground.setFillColor( sf::Color{ 50, 50, 50, 80 } );
    hpBarInner.setFillColor( sf::Color{ 250, 20, 20, 100 } );
    hpBarBackground.setOrigin( utils::getCenterOfObject( hpBarBackground ) );
    hpBarInner.setOrigin( hpBarBackground.getOrigin() );

    restart( windowSize );
}

void Ship::restart( sf::Vector2u windowSize )
{
    hp = maxHp;
    bulletsMissed = 0;
    bulletsFired = 0;
    sprite.setPosition(0., 0.);

    auto centerOfShip = utils::getCenterOfObject( sprite );
    sprite.setPosition( (float)windowSize.x / 2.f, (float)windowSize.y + sprite.getGlobalBounds().height - 200.f );

    hpBarBackground.setPosition( sprite.getPosition().x, sprite.getPosition().y + centerOfShip.y + 5.f );
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
    float y = sprite.getPosition().y - sprite.getGlobalBounds().height / 2;
    sf::Vector2f pos { sprite.getPosition().x - 5.f, y };

    bullets.emplace_back( pos, sf::Vector2f{ 0.f, -1.f }, movementSpeed * 1.5f );
    ++bulletsFired;
}

void Ship::update( float dt, sf::Vector2< int > mousePos )
{
//    float dx = static_cast<float>( mousePos.x ) - sprite.getPosition().x;
//    float dy = static_cast<float>( mousePos.y ) - sprite.getPosition().y;

//    float angle = std::atan2( dy, dx ) * ( 180 / PI );
//    sprite.setRotation( 90 + angle );
//    hpBarBackground.setRotation( sprite.getRotation() );
//    hpBarInner.setRotation( sprite.getRotation() );
//    hpBarInner.setPosition( hpBarBackground.getPosition() );

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

//    auto b = sprite.getGlobalBounds();
//    auto pos = sprite.getPosition();
}

void Ship::render( sf::RenderTarget & window )
{
    window.draw( sprite );
    window.draw( hpBarBackground );
    window.draw( hpBarInner );
    for( auto &bullet : bullets ) { bullet.render( window ); }
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
    if( hp <= 0 ) { return; } //TODO log warning

    hp -= amount;
    float hpPercent = static_cast< float >( hp ) / static_cast< float >( maxHp );
    hpBarInner.setSize( { std::floor( maxHpBarWidth * hpPercent ), hpBarInner.getSize().y } );
}

bool Ship::isBulletOutsideOfTheScreen( const Bullet & bullet ) const noexcept
{
    //TODO update condition, than ship can rotate and bullets will fly in others directions
    return bullet.getBounds().top + bullet.getBounds().height < 0.f;
}

sf::FloatRect Ship::getBounds() const
{
    sf::Rect< float > bounds = sprite.getGlobalBounds();
    bounds.height += hpBarInner.getGlobalBounds().height * 2;
    return bounds;
}

}