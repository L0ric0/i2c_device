#ifndef I2C_CONCEPT_HPP_
#define I2C_CONCEPT_HPP_

#include <iterator>

namespace i2c
{
	template <class T>
    concept byte = requires(T t)
    {
        sizeof(T) == 1;
    };

    template <class T>
    concept word = requires(T t)
    {
        sizeof(T) == 2;
    };

    template<class I, class T>
    concept contiguous_output_iterator = std::contiguous_iterator<I> && std::output_iterator<I, T>;
    
}

#endif