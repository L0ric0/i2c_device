#ifndef I2C_EXCEPTION_HPP_
#define I2C_EXCEPTION_HPP_

#include <system_error>
#include <string>

namespace i2c::exception
{

	struct i2c_error : std::system_error {
        i2c_error(std::error_code ec);
        i2c_error(std::error_code ec, const std::string& what_arg);
        i2c_error(std::error_code ec, const char* what_arg);
        i2c_error(int ev, const std::error_category& ecat);
        i2c_error(int ev, const std::error_category& ecat, const std::string& what_arg);
        i2c_error(int ev, const std::error_category& ecat, const char* what_arg);
    };

    struct again : i2c_error {
        again(int ev = EAGAIN, const std::error_category& ecat = std::system_category());
    };

    struct bad_message : i2c_error {
        bad_message(int ev = EBADMSG, const std::error_category& ecat = std::system_category());
    };

    struct busy : i2c_error {
        busy(int ev = EBUSY, const std::error_category& ecat = std::system_category());
    };

    struct invalid_parameter : i2c_error {
        invalid_parameter(int ev = EINVAL, const std::error_category& ecat = std::system_category());
    };

    struct io_error : i2c_error {
        io_error(int ev = EIO, const std::error_category& ecat = std::system_category());
    };

    struct no_device : i2c_error {
        no_device(int ev = ENODEV, const std::error_category& ecat = std::system_category());
    };

    struct no_device_or_address : i2c_error {
        no_device_or_address(int ev = ENXIO, const std::error_category& ecat = std::system_category());
    };

    struct operation_not_supported : i2c_error {
        operation_not_supported(int ev = EOPNOTSUPP, const std::error_category& ecat = std::system_category());
    };

    struct protocol_error : i2c_error {
        protocol_error(int ev = EPROTO, const std::error_category& ecat = std::system_category());
    };

    struct timedout : i2c_error {
        timedout(int ev = ETIMEDOUT, const std::error_category& ecat = std::system_category());
    };
}

#endif