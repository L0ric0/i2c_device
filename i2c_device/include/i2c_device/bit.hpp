#ifndef I2C_BIT_HPP_
#define I2C_BIT_HPP_

#include "i2c_device/concept.hpp"

#include <bit>
#include <iterator>
#include <type_traits>


namespace i2c::util
{
    template<word T>
    T byte_swap(const T value)
    {
        return static_cast<T>(((value & 0xff) << 8) | ((value & 0xff00) >> 8));
    }

    template<word T, std::endian Endian = std::endian::big>
    T to_device_repr(const T value)
    {
        if constexpr (std::endian::native != Endian) {
            return byte_swap(value);
        } else {
            return value;
        }
    }

    template<word T, std::endian Endian = std::endian::big>
    T from_device_repr(const T value)
    {
        return to_device_repr<T, Endian>(value);
    }
}

#endif
