/*
 * \file AssetsStorage.h
 * \copyright (C) 2020 Special Technological Center Ltd
 * \author : Bardashevsky A.K.
 * \date : 13.09.2020
 * \time : 14:14
 */

#pragma once

#include <memory>
#include <map>
#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Types.h"

namespace space_shooter {

class AssetsStorage
{
private:
    using Textures = std::map< std::string, sf::Texture >;
    using Fonts = std::map< std::string, sf::Font >;

public:
    static AssetsStorage & instance();

    template< class AssetT >
    void load( const std::string & path );

    template< class AssetT >
    const AssetT & get( const std::string & key ) const;

private:
    template< class AssetT, class ContainerT >
    void load( ContainerT & storage, const std::string & path );

    template< class AssetT, class ContainerT >
    const AssetT & get( const ContainerT & storage, const std::string & key ) const;

public:
    AssetsStorage( const AssetsStorage &) = delete;
    AssetsStorage &operator=( const AssetsStorage &) = delete;

private:
    AssetsStorage() = default;

    Textures m_textures;
    Fonts m_fonts;
    static std::unique_ptr< AssetsStorage > m_instance;
};



///////////// definitions /////////////

template< class AssetT, class ContainerT >
void AssetsStorage::load( ContainerT & storage, const std::string & path )
{
    if( storage.find( path ) != storage.end() )
    {
        throw std::logic_error("Asset " + path + " already loaded!");
    }

    AssetT asset;
    if( !asset.loadFromFile( path ) )
    {
        throw std::runtime_error( "Couldn't load asset at path " + path );
    }

    storage.emplace( path, std::move( asset ) );
}

template< class AssetT, class ContainerT >
const AssetT & AssetsStorage::get( const ContainerT & storage, const std::string & key ) const
{
    if( auto it = storage.find( key ); it != storage.end() )
    {
        return it->second;
    }
    throw std::runtime_error("Couldn't find asset with key " + key);
}

}