#ifndef I2C_LINUX_HPP_
#define I2C_LINUX_HPP_

#include "i2c-device/exception.hpp"
#include "i2c-device/concept.hpp"
#include "i2c-device/util.hpp"

#include <linux/i2c-dev.h>
extern "C" {
#include <i2c/smbus.h>
}
#include <unistd.h>

#include <bit>
#include <iterator>
#include <array>
#include <cstdint>
#include <system_error>

namespace i2c::driver
{

    struct SMBus_Block {
        std::uint8_t length;
        std::array<std::uint8_t, I2C_SMBUS_BLOCK_MAX> data;
    };

    template<std::endian Endian = std::endian::big>
    struct Linux {
        using handle_type = int;

        handle_type open_i2c_device(const AdapterNumber adapter_nr, const DeviceID deviceid){
            const auto filehandle = ::open(fmt::format("/dev/i2c-{}", adapter_nr()).c_str(), O_RDWR);
            if (filehandle < 0) {
                //FIXME function to handle open errors
            }

            const auto res = ioctl(filehandle, I2C_SLAVE, deviceid());
            handle_i2c_error(res);

            return filehandle;
        }

        void close_i2c_device(const handle_type& handle)
        {
            ::fclose(handle);
        }

        void handle_i2c_error(const int ret_val)
        {
            if (ret_val > 0) {
                return;
            }

            switch (errno) {
            case EAGAIN:
                throw exception::again {};
            case EBADMSG:
                throw exception::bad_message {};
            case EBUSY:
                throw exception::busy {};
            case EINVAL:
                throw exception::invalid_parameter {};
            case EIO:
                throw exception::io_error {};
            case ENODEV:
                throw exception::no_device {};
            case ENOMEM:
                throw std::bad_alloc {};
            case ENXIO:
                throw exception::no_device_or_address {};
            case EOPNOTSUPP:
                throw exception::operation_not_supported {};
            case EPROTO:
                throw exception::protocol_error {};
            case ETIMEDOUT:
                throw exception::timedout {};
            default:
                throw exception::i2c_error { std::system_category().default_error_condition(errno) };
            }
        }

        void write(const handle_type& handle, std::contiguous_iterator auto buffer_begin, std::contiguous_iterator auto buffer_end)
        {
            const auto bytes_to_write = std::distance(buffer_begin, buffer_end);
            const auto res = ::write(handle, &(*buffer_begin), bytes_to_write);

            handle_i2c_error(res);
        }

        template<byte T>
        void read(const handle_type& handle, contiguous_output_iterator<T> auto buffer_begin, contiguous_output_iterator<T> auto buffer_end)
        {
            const auto bytes_to_read = std::distance(buffer_begin, buffer_end);

            const auto res = ::read(handle, &(*buffer_begin), bytes_to_read);
            
            handle_i2c_error(res);
        }

        void i2c_smbus_write_quick(const handle_type handle, const std::uint8_t value)
        {
            const auto res = ::i2c_smbus_write_quick(handle, value);
            handle_i2c_error(res);
        }

        std::uint8_t read_byte(const handle_type& handle)
        {
            const auto res = ::i2c_smbus_read_byte(handle);
            handle_i2c_error(res);
            return static_cast<std::uint8_t>(res);
        }

        void write_byte(const handle_type& handle, const std::uint8_t value)
        {
            const auto res = ::i2c_smbus_write_byte(handle, value);
            handle_i2c_error(res);
        }

        std::uint8_t read_byte_data(const handle_type& handle, const std::uint8_t reg)
        {
            const auto res = ::i2c_smbus_read_byte_data(handle, reg);
            handle_i2c_error(res);
            return static_cast<std::uint8_t>(res);
        }

        void write_byte_data(const handle_type handle, const std::uint8_t reg, const std::uint8_t value)
        {
            const auto res = ::i2c_smbus_write_byte_data(handle, reg, value);
            handle_i2c_error(res);
        }

        std::uint16_t read_word_data(const handle_type& handle, const std::uint8_t reg)
        {
            const auto res = ::i2c_smbus_read_word_data(handle, reg);
            handle_i2c_error(res);
            return util::from_device_repr<std::uint16_t, Endian>(static_cast<std::uint16_t>(res));
        }

        void write_word_data(const handle_type handle, const std::uint8_t reg, const std::uint16_t value)
        {
            const auto res = ::i2c_smbus_write_word_data(handle, reg, util::to_device_repr<std::uint16_t, Endian>(value));
            handle_i2c_error(res);
        }

        template<byte T>
        void read_data(
            const handle_type& handle,
            const std::uint8_t reg,
            contiguous_output_iterator<T> auto buffer_begin,
            contiguous_output_iterator<T> auto buffer_end)
        {
            i2c_smbus_read_i2c_block_data(handle, reg, buffer_begin, buffer_end);
        }

        SMBus_Block i2c_smbus_read_block_data(const handle_type handle, const std::uint8_t reg)
        {
            std::array<std::uint8_t, I2C_SMBUS_BLOCK_MAX> buffer {};
            const auto res = ::i2c_smbus_read_block_data(handle, reg, buffer.data());
            handle_i2c_error(res);
            return { static_cast<std::uint8_t>(res), buffer };
        }

        void i2c_smbus_write_block_data(
            const handle_type handle,
            const std::uint8_t reg,
            std::contiguous_iterator auto buffer_begin,
            std::contiguous_iterator auto buffer_end)
        {
            const auto bytes_to_write = std::distance(buffer_begin, buffer_end);

            const auto res
                = ::i2c_smbus_write_block_data(handle, reg, bytes_to_write, &(*buffer_begin));
            handle_i2c_error(res);
        }

        std::uint16_t i2c_smbus_process_call(
            const handle_type handle,
            const std::uint8_t reg,
            const std::uint16_t value)
        {
            const auto res = ::i2c_smbus_process_call(handle, reg, value);
            handle_i2c_error(res);
            return static_cast<std::uint8_t>(res);
        }

        template<byte T>
        void i2c_smbus_block_process_call(
            const handle_type handle,
            const std::uint8_t reg,
            contiguous_output_iterator<T> auto buffer_begin,
            contiguous_output_iterator<T> auto buffer_end)
        {
            const auto bytes_to_write = std::distance(buffer_begin, buffer_end);

            const auto res
                = ::i2c_smbus_block_process_call(handle, reg, bytes_to_write, &(*buffer_begin));
            handle_i2c_error(res);
        }
        
        template<byte T>
        void i2c_smbus_read_i2c_block_data(
            const handle_type handle,
            std::uint8_t reg,
            contiguous_output_iterator<T> auto buffer_begin,
            contiguous_output_iterator<T> auto buffer_end)
        {
            const auto bytes_to_read = std::distance(buffer_begin, buffer_end);

            const auto res
                = ::i2c_smbus_read_i2c_block_data(handle, reg, bytes_to_read, &(*buffer_begin));
            handle_i2c_error(res);
        }

        void i2c_smbus_write_i2c_block_data(
            const handle_type handle,
            std::uint8_t reg,
            std::contiguous_iterator auto buffer_begin,
            std::contiguous_iterator auto buffer_end)
        {
            const auto bytes_to_write = std::distance(buffer_begin, buffer_end);

            const auto res
                = ::i2c_smbus_write_i2c_block_data(handle, reg, bytes_to_write, &(*buffer_begin));
            handle_i2c_error(res);
        }

    };
}

#endif