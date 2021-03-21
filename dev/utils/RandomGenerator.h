/*
 * \file random_generator.h
 * \author : Bardashevsky A.K.
 * \date : 26.06.2020
 * \time : 18:12
 */

#pragma once

#include <random>
#include <type_traits>
#include <stdexcept>

namespace space_shooter::utils
{
/**
 * @brief Class to generate random numbers in a given range
 */
class RandomGenerator
{
private:
    using Generator = std::mt19937;

public:
    template <class ValueT, typename std::enable_if_t<std::is_integral<ValueT>::value
                                                      && !std::is_floating_point<ValueT>::value> * = nullptr>
    static ValueT get(ValueT min, ValueT max)
    {
        if( min >= max ) { throw std::invalid_argument("Invalid range!"); }
        std::random_device device;
        Generator mt(device());
        std::uniform_int_distribution<ValueT> seed(min, max);
        return seed(mt);
    }

    template <class ValueT, typename std::enable_if_t<std::is_floating_point<ValueT>::value> * = nullptr>
    static ValueT get(ValueT min, ValueT max)
    {
        if( min >= max ) { throw std::invalid_argument("Invalid range!"); }
        std::random_device device;
        Generator mt(device());
        std::uniform_real_distribution<ValueT> seed(min, max);
        return seed(mt);
    }
};

}