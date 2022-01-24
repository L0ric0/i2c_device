#ifndef I2C_WINDOWS_HPP_
#define I2C_WINDOWS_HPP_

#include "i2c_device/concept.hpp"
#include "i2c_device/util.hpp"
#include "i2c_device/bit.hpp"
#include "i2c_device/exception.hpp"

#include <winrt/windows.devices.i2c.h>
#include <winrt/windows.devices.enumeration.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/base.h>

#include <bit>
#include <iterator>
#include <array>
#include <cstdint>

namespace i2c::driver
{
    template <std::endian Endian = std::endian::big>
    struct Windows {
        using handle_type = winrt::Windows::Devices::I2c::I2cDevice;

        handle_type open_i2c_device(const AdapterNumber adapter_nr, const DeviceID deviceid)
        {
            auto i2cDeviceSelector = winrt::Windows::Devices::I2c::I2cDevice::GetDeviceSelector();
            winrt::Windows::Devices::Enumeration::DeviceInformationCollection devices
                = winrt::Windows::Devices::Enumeration::DeviceInformation::FindAllAsync(
                      i2cDeviceSelector)
                      .get();
            auto settings = winrt::Windows::Devices::I2c::I2cConnectionSettings(deviceid());

            return winrt::Windows::Devices::I2c::I2cDevice::FromIdAsync(
                       devices.GetAt(adapter_nr()).Id(),
                       settings)
                .get();
        }

        void close_i2c_device(const handle_type& handle)
        {
            handle.Close();
        }

        void handle_i2c_error(const winrt::Windows::Devices::I2c::I2cTransferResult res) const {
            using namespace winrt::Windows::Devices::I2c;
            if (res.Status == I2cTransferStatus::FullTransfer) {
                return;
            }

            switch (res.Status) {
            case I2cTransferStatus::ClockStretchTimeout:
                throw exception::timeout {};
            case I2cTransferStatus::SlaveAddressNotAcknowledged:
                throw exception::no_device_or_address {};
            default:
                throw exception::i2c_error { -1 };
            }
        }

        void write(
            const handle_type& handle,
            std::contiguous_iterator auto buffer_begin,
            std::contiguous_iterator auto buffer_end) const
        {
            const auto res = handle.WritePartial(winrt::array_view(buffer_begin, buffer_end));
            handle_i2c_error(res);
        }

        template<byte T>
        void read(
            const handle_type& handle,
            contiguous_output_iterator<T> auto buffer_begin,
            contiguous_output_iterator<T> auto buffer_end) const
        {
            const auto res = handle.ReadPartial(winrt::array_view(buffer_begin, buffer_end));
            handle_i2c_error(res);
        }

        std::uint8_t read_byte(const handle_type& handle) const {
            std::array<std::uint8_t, 1> buffer {};
            const auto res = handle.ReadPartial(buffer);
            handle_i2c_error(res);
            return buffer[0];
        }

        void write_byte(const handle_type& handle, const std::uint8_t value) const {
            winrt::array_view buffer { value };
            const auto res = handle.WritePartial(buffer);
            handle_i2c_error(res);
        }

        std::uint8_t read_byte_data(const handle_type& handle, const std::uint8_t reg) const {
            winrt::array_view addr_buffer { reg };
            std::array<std::uint8_t, 1> read_buffer {};
            const auto res = handle.WriteReadPartial(addr_buffer, read_buffer);
            handle_i2c_error(res);
            return read_buffer[0];
        }

        void write_byte_data(const handle_type& handle, const std::uint8_t reg, const std::uint8_t value) const {
            winrt::array_view buffer { reg, value };
            const auto res = handle.WritePartial(buffer);
            handle_i2c_error(res);
        }

        std::uint16_t read_word_data(const handle_type& handle, const std::uint8_t reg) const {
            std::array<std::uint8_t, 1> addr_buffer { reg };
            std::array<std::uint8_t, 2> read_buffer {};
            const auto res = handle.WriteReadPartial(addr_buffer, read_buffer);
            handle_i2c_error(res);

            return util::from_device_repr<std::uint16_t, Endian>(std::bit_cast<std::uint16_t>(read_buffer));
        }

        void write_word_data(const handle_type& handle, const std::uint8_t reg, const std::uint16_t value) const {
            auto value_buffer = std::bit_cast<std::array<std::uint8_t, 2>>(util::to_device_repr<std::uint16_t, Endian>(value));
            std::array<std::uint8_t, 3> buffer {};
            buffer[0] = reg;
            auto const res = handle.WritePartial(buffer);
            handle_i2c_error(res);
        }

        template<byte T>
        void read_data(
            const handle_type& handle,
            const std::uint8_t reg,
            contiguous_output_iterator<T> auto buffer_begin,
            contiguous_output_iterator<T> auto buffer_end) const
        {
            winrt::array_view addr_buffer { reg };
            auto read_buffer = winrt::array_view(buffer_begin, buffer_end);
            auto const res = handle.WriteReadPartial(addr_buffer, read_buffer);
        }
    };
}

#endif