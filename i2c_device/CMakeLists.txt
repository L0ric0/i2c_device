add_library(i2c_device
    "include/i2c/exception.hpp" "src/exception.cpp"
    "include/i2c/concept.hpp"
    "include/i2c/device.hpp"
    "include/i2c/bit.hpp"
    "include/i2c/util.hpp"
)

if(WIN32)
target_sources(i2c_device
    PUBLIC "include/i2c/driver/Windows.hpp"
)
elseif(LINUX)
target_sources(i2c_device
    PUBLIC "include/i2c/driver/Linux.hpp"
)
endif()

add_library(i2c_device::i2c_device ALIAS i2c_device)

target_link_libraries(i2c_device PRIVATE project_options
                                         project_warnings)

target_include_directories(i2c_device PUBLIC include)

if(WIN32)

elseif(LINUX)
target_link_libraries(i2c_device PUBLIC i2c)
endif()
