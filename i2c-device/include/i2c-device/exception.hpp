#ifndef I2C_EXCEPTION_HPP_
#define I2C_EXCEPTION_HPP_

#include "i2c-device/detail/i2c-device_export.h"


#include <system_error>
#include <string>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4275)
#endif

namespace i2c::exception
{
    // suppress C4275 (non dll-interface class 'std::system_error' used as base for dll-interface struct 'i2c::exception::i2c_error')
    // to not end in a compiler error as the common consensus seems to be to just ignore it
    // https://stackoverflow.com/questions/24511376/how-to-dllexport-a-class-derived-from-stdruntime-error
    // https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-2-c4275?view=msvc-170
	struct I2C_DEVICE_EXPORT i2c_error : std::system_error
    {
        i2c_error(std::error_code ec);
        i2c_error(std::error_code ec, const std::string& what_arg);
        i2c_error(std::error_code ec, const char* what_arg);
        i2c_error(int ev, const std::error_category& ecat);
        i2c_error(int ev, const std::error_category& ecat, const std::string& what_arg);
        i2c_error(int ev, const std::error_category& ecat, const char* what_arg);
    };

    struct I2C_DEVICE_EXPORT again : i2c_error {
        again(int ev = EAGAIN, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT bad_message : i2c_error {
        bad_message(int ev = EBADMSG, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT busy : i2c_error {
        busy(int ev = EBUSY, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT invalid_parameter : i2c_error {
        invalid_parameter(int ev = EINVAL, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT io_error : i2c_error {
        io_error(int ev = EIO, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT no_device : i2c_error {
        no_device(int ev = ENODEV, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT no_device_or_address : i2c_error {
        no_device_or_address(int ev = ENXIO, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT operation_not_supported : i2c_error {
        operation_not_supported(int ev = EOPNOTSUPP, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT protocol_error : i2c_error {
        protocol_error(int ev = EPROTO, const std::error_category& ecat = std::system_category());
    };

    struct I2C_DEVICE_EXPORT timedout : i2c_error {
        timedout(int ev = ETIMEDOUT, const std::error_category& ecat = std::system_category());
    };
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif