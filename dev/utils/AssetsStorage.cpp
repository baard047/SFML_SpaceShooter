/*
 * \file AssetsStorage.cpp
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 14:14
 */

#include "AssetsStorage.h"

#include <stdexcept>

namespace space_shooter {

template<>
void AssetsStorage::load< sf::Texture >( const std::string & path )
{
    load< sf::Texture >( m_textures, path );
}

template<>
void AssetsStorage::load< sf::Font >( const std::string & path )
{
    load< sf::Font >( m_fonts, path );
}

template<>
const sf::Texture & AssetsStorage::get( const std::string & key ) const
{
    return get< sf::Texture >( m_textures, key );
}

template<>
const sf::Font & AssetsStorage::get( const std::string & key ) const
{
    return get< sf::Font >( m_fonts, key );
}

AssetsStorage & AssetsStorage::instance()
{
    if( !m_instance ) { m_instance.reset( new AssetsStorage() ); }
    return *m_instance;
}

std::unique_ptr< AssetsStorage > AssetsStorage::m_instance;

}