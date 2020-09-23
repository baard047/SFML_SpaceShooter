/*
 * \file HUD.cpp
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 17.09.2020
 * \time : 9:11
 */

#include "HUD.h"

#include <utils/AssetsStorage.h>

#include <iostream>

namespace space_shooter {

namespace {
constexpr const float FPS_TEXT_W = 100.0f;
constexpr const float MARGINS = 15.0f;

namespace literals {
constexpr const auto FPS = "FPS: ";
constexpr const auto BULLETS_FIRED = "Bullets Fired: ";
constexpr const auto BULLETS_MISSED = "Bullets Missed: ";
constexpr const auto ASTEROIDS_DESTROYED = "Asteroids Destroyed: ";
constexpr const auto SCORE = "SCORE: ";
constexpr const auto GAME_RESTART_TEXT = "Press space to restart";
}

}

void HUD::init( sf::Vector2u windowSize )
{
    const auto &font = AssetsStorage::instance().get< sf::Font >( fonts::DOSIS_LIGHT );

    fpsText.setFont( font );
    fpsText.setFillColor( sf::Color{ 102, 255, 102, 200 } );
    fpsText.setPosition( { windowSize.x - FPS_TEXT_W - MARGINS, MARGINS } );

    const sf::Color hudColor { 255, 255, 102, 150 };

    float hudYPos = 0.f + MARGINS;
    for( auto & stat : statistics )
    {
        stat.setFont( font );
        stat.setFillColor( hudColor );
        stat.setCharacterSize( 20 );
        stat.setPosition( 0.f + MARGINS, hudYPos );
        hudYPos += MARGINS + stat.getGlobalBounds().height + (float)stat.getCharacterSize();
    }

    gameOverBanner.setTexture( AssetsStorage::instance().get< sf::Texture >( textures::GAME_OVER ) );

    sf::Vector2f windowCenter{ (float) windowSize.x / 2, (float)windowSize.y / 2 };
    gameOverBanner.setPosition( windowCenter.x - gameOverBanner.getGlobalBounds().width / 2,
                                windowCenter.y - gameOverBanner.getGlobalBounds().height / 2 );
    gameOverBanner.setColor( sf::Color::Red );

    gameRestartText.setFont( font );
    gameRestartText.setFillColor( sf::Color::Red );
    gameRestartText.setString( literals::GAME_RESTART_TEXT );
    gameRestartText.setPosition( windowCenter.x - gameRestartText.getGlobalBounds().width / 2,
                                 gameOverBanner.getGlobalBounds().height + gameOverBanner.getGlobalBounds().top);

}

void HUD::update( const Stats & stats )
{
    updateFps();

    statistics[ TotalBullets ].setString( literals::BULLETS_FIRED + std::to_string( stats.totalBulletFired ) );
    statistics[ BulletsMissed ].setString( literals::BULLETS_MISSED + std::to_string( stats.bulletsMissed ) );
    statistics[ AsteroidDestroyed ].setString( literals::ASTEROIDS_DESTROYED + std::to_string( stats.asteroidsDestroyed ) );
    statistics[ Score ].setString( literals::SCORE + std::to_string( stats.score ) );
}

void HUD::render( sf::RenderTarget & window )
{
    window.draw( fpsText );
    for( const auto & stat : statistics ) { window.draw( stat ); }
}

void HUD::updateFps()
{
    fps = static_cast< int32_t >( 1.f / clock.getElapsedTime().asSeconds() );
    fpsText.setString( literals::FPS + std::to_string( fps ) );
    clock.restart();
}

void HUD::showGameOverBanner( sf::RenderTarget & window )
{
    window.draw( gameOverBanner );
    window.draw( gameRestartText );
}

}