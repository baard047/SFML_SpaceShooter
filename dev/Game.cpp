/*
 * \file Game.cpp
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 11:45
 */

#include "Game.h"

#include "utils/AssetsStorage.h"
#include <utils/RandomGenerator.h>

namespace {
constexpr const float dtMultiplier = 144.f;
constexpr const float ASTEROIDS_RESPAWN_COOLDOWN = 0.6f;

}

namespace space_shooter {

Game::Game()
        : window( std::make_unique< sf::RenderWindow >( sf::VideoMode( 800, 600 ), "Space Shooter",
                                                        sf::Style::Close | sf::Style::Titlebar ) )
{
    window->setFramerateLimit( 1500 );
    window->setVerticalSyncEnabled( false );

    loadAssets();

    background.setTexture( AssetsStorage::instance().get< sf::Texture >( textures::BACKGROUND ) );
    background.setScale( (float) window->getSize().x / background.getGlobalBounds().width,
                         (float) window->getSize().y / background.getGlobalBounds().height );

    initCustomCursor();

    hud.init( window->getSize() );
    ship = std::make_unique< Ship >( window->getSize() );
}

void Game::loadAssets()
{
    AssetsStorage::instance().load< sf::Texture >( textures::BACKGROUND );
    AssetsStorage::instance().load< sf::Texture >( textures::CURSOR );
    AssetsStorage::instance().load< sf::Texture >( textures::SHIP );
    AssetsStorage::instance().load< sf::Texture >( textures::BULLET );
    AssetsStorage::instance().load< sf::Texture >( textures::ASTEROID );
    AssetsStorage::instance().load< sf::Texture >( textures::GAME_OVER );
    AssetsStorage::instance().load< sf::Font >( fonts::DOSIS_LIGHT );
    AssetsStorage::instance().load< sf::Font >( fonts::PIXEL_LETTERS );
}

void Game::run()
{
    sf::Clock clock;
    while( window->isOpen() )
    {
        float dt = clock.restart().asSeconds() * dtMultiplier;
        update( dt );

        render();
    }
}

void Game::update( float dt )
{
    processEvents();

    auto mousePos = sf::Mouse::getPosition( *window );
    if( gameState == Play )
    {
        handleShipMovements( dt );
        ship->update( dt, mousePos );
        spawnAsteroid();
        updateAsteroids( dt );

        stats.totalBulletFired = ship->getTotalBulletsFired();
        stats.bulletsMissed = ship->getBulletsMissed();
        hud.update( stats );
    }

    sf::Vector2f cursorCenter{ cursor.getGlobalBounds().width / 2, cursor.getGlobalBounds().height / 2 };
    cursor.setPosition( (float)mousePos.x - cursorCenter.x, (float)mousePos.y - cursorCenter.y );
}

void Game::render()
{
    window->clear();

    window->draw( background );
    for( auto &asteroid : asteroids ) { asteroid.render( *window ); }
    ship->render( *window );
    hud.render( *window );
    window->draw( cursor );

    if( gameState == GameOver )
    {
        hud.showGameOverBanner( *window );
    }

    window->display();
}

void Game::processEvents()
{
    //TODO if it'll be more complex do Finite State Machine

    sf::Event event{};
    while( window->pollEvent( event ) )
    {
        if( event.type == sf::Event::Closed ||
            ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape ) )
        {
            window->close();
        }

        if( gameState == Play )
        {
            if( event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left )
            {
                ship->fire();
            }
        }
        else if( gameState == GameOver )
        {
            if( event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Space )
            {
                restartGame();
            }
        }
    }
}

void Game::handleShipMovements( float dt )
{
    sf::FloatRect shipBounds = ship->getBounds();

    if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
    {
        if( shipBounds.left > 0.f ) { ship->move( -1.f * dt, 0.f ); }
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        if( shipBounds.left + shipBounds.width < window->getSize().x )
        {
            ship->move( 1.f * dt, 0.f );
        }
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) )
    {
        if( shipBounds.top > 0.f ) { ship->move( 0.f, -1.f * dt ); }
    }
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        if( shipBounds.top + shipBounds.height < window->getSize().y )
        {
            ship->move( 0.f, 1.f * dt );
        }
    }
}

void Game::updateAsteroids( float dt )
{
    for( auto asteroid = asteroids.begin(); asteroid != asteroids.end(); )
    {
        asteroid->update( dt );
        auto collision = ship->checkCollisions( *asteroid );

        if( !ship->isAlive() )
        {
            gameState = GameOver;
            break;
        }

        if( collision == Asteroid::Damaged_Ship )
        {
            asteroid = asteroids.erase( asteroid );
        }
        else if( collision == Asteroid::Got_Hit_By_A_Bullet )
        {
            asteroid->takeDamage( ship->getDamage() );

            if( asteroid->getHp() <= 0 )
            {
                ++stats.asteroidsDestroyed;
                stats.score += asteroid->getPoints();

                asteroid = asteroids.erase( asteroid );
            }
        }
        else if( asteroid->getBounds().top > window->getSize().y )
        {
            stats.score -= asteroid->getPoints();

            asteroid = asteroids.erase( asteroid );
        }
        else { ++asteroid; }
    }
}

void Game::spawnAsteroid()
{
    if( asteroidsRespawnTimer.getElapsedTime().asSeconds() > ASTEROIDS_RESPAWN_COOLDOWN )
    {
        asteroids.emplace_back( sf::Vector2f{ utils::RandomGenerator::get( 0.f, window->getSize().x - 100.f), -100.f } );
        asteroidsRespawnTimer.restart();
    }
}

void Game::restartGame()
{
    ship->restart( window->getSize() );
    asteroids.clear();
    asteroidsRespawnTimer.restart();
    stats.clear();

    gameState = Play;
}

void Game::initCustomCursor()
{
    window->setMouseCursorVisible( false );
    cursor.setTexture( AssetsStorage::instance().get< sf::Texture >( textures::CURSOR ) );
    cursor.setScale( 0.50f, 0.50f );
    sf::Vector2f windowCenter{ (float) window->getSize().x / 2, (float)window->getSize().y / 2 };
    cursor.setPosition( windowCenter.x - cursor.getGlobalBounds().width / 2,
                        windowCenter.y - cursor.getGlobalBounds().height / 2 );
}

}