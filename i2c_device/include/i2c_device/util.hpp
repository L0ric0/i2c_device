#ifndef I2C_UTIL_HPP_
#define I2C_UTIL_HPP_

namespace i2c
{
    struct AdapterNumber {
        int adapter_nr;

        explicit AdapterNumber(const int value) : adapter_nr(value) {}

        int operator()() const {
            return adapter_nr;
        }
    };

    struct DeviceID {
        int device_id;
    
        explicit DeviceID(const int value) : device_id(value) {}

        int operator()() const {
            return device_id;
        }
    };
}

#endif