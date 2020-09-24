/*
 * \file utils.h
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 24.09.2020
 * \time : 9:49
 */

#pragma once

#include <type_traits>

#include <SFML/Graphics/Sprite.hpp>

namespace space_shooter::utils {

template< class T, class = typename std::enable_if_t< std::is_base_of_v< sf::Transformable, T>> >
sf::Vector2< float > getCenterOfObject( const T & object )
{
    float x = object.getPosition().x + object.getGlobalBounds().width / 2;
    float y = object.getPosition().y + object.getGlobalBounds().height / 2;
    return { x, y };
}

}