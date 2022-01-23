#include "i2c/exception.hpp"

#include <system_error>
#include <string>

namespace i2c::exception
{
    i2c_error::i2c_error(std::error_code ec) : std::system_error(ec) {}
    i2c_error::i2c_error(std::error_code ec, const std::string& what_arg)
        : std::system_error(ec, what_arg) {}
    i2c_error::i2c_error(std::error_code ec, const char* what_arg)
        : std::system_error(ec, what_arg) {}
    i2c_error::i2c_error(int ev, const std::error_category& ecat) : std::system_error(ev, ecat) {}
    i2c_error::i2c_error(int ev, const std::error_category& ecat, const std::string& what_arg)
        : std::system_error(ev, ecat, what_arg) {}
    i2c_error::i2c_error(int ev, const std::error_category& ecat, const char* what_arg)
        : std::system_error(ev, ecat, what_arg) {}

    again::again(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    bad_message::bad_message(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    busy::busy(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    invalid_parameter::invalid_parameter(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    io_error::io_error(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    no_device::no_device(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    no_device_or_address::no_device_or_address(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    operation_not_supported::operation_not_supported(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    protocol_error::protocol_error(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

    timedout::timedout(int ev, const std::error_category& ecat) : i2c_error(ev, ecat) {}

}