#ifndef I2C_DEVICE_HPP_
#define I2C_DEVICE_HPP_

#include "i2c-device/concept.hpp"
#include "i2c-device/util.hpp"

#ifdef _WIN32
#include "i2c-device/driver/Windows.hpp"

using default_driver = i2c::driver::Windows<>;
#elif __linux__
#include "i2c-device/driver/Linux.hpp"

using default_driver = i2c::driver::Linux<>;
#endif

#include <bit>
#include <optional>
#include <cstdint>
#include <iterator>

namespace i2c
{
    template<class Driver = default_driver>
    struct i2c_device
    {
        Driver driver;

        private:
        std::optional<typename Driver::handle_type> m_handle;

        public:
        i2c_device(const AdapterNumber adapter_nr, const DeviceID deviceid)
        : m_handle(driver.open_i2c_device(adapter_nr, deviceid))
          {
          }

        ~i2c_device() {
            if (m_handle) {
                driver.close_i2c_device(m_handle.value());
            }
        }

        i2c_device(const i2c_device& other) = delete;
        i2c_device& operator=(const i2c_device& other) = delete;

        i2c_device(i2c_device&& other) noexcept : m_handle(std::move(other.m_handle)) {
            other.m_handle.reset();
        }
        i2c_device& operator=(i2c_device&& other) noexcept {
            m_handle = std::move(other.m_handle);
            other.m_handle.reset();
            return *this;
        }

        void write(std::contiguous_iterator auto buffer_begin, std::contiguous_iterator auto buffer_end) const {
            driver.write(m_handle.value(), buffer_begin, buffer_end);
        }

        template<byte T>
        void read(contiguous_output_iterator<T> auto buffer_begin, contiguous_output_iterator<T> auto buffer_end) const
        {
            driver.read(m_handle.value(), buffer_begin, buffer_end);
        }

        std::uint8_t read_byte() const {
            return driver.read_byte(m_handle.value());
        }

        void write_byte(const std::uint8_t value) const {
            driver.write_byte(m_handle.value(), value);
        }

        std::uint8_t read_byte_data(const std::uint8_t reg) const {
            return driver.read_byte_data(m_handle.value(), reg);
        }

        void write_byte_data(const std::uint8_t reg, const std::uint8_t value) const {
            driver.write_byte_data(m_handle.value(), reg, value);
        }

        std::uint16_t read_word_data(const std::uint8_t reg) const {
            return driver.read_word_data(m_handle.value(), reg);
        }

        void write_word_data(const std::uint8_t reg, const std::uint16_t value) const {
            driver.write_word_data(m_handle.value(), reg, value);
        }
    };
}

#endif