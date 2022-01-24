set(I2CDEVICE_HEADER 
    "i2c-device/include/i2c-device/exception.hpp" 
    "i2c-device/include/i2c-device/concept.hpp"
    "i2c-device/include/i2c-device/device.hpp"
    "i2c-device/include/i2c-device/bit.hpp"  
    "i2c-device/include/i2c-device/util.hpp"
)

add_library(i2c-device
    "src/exception.cpp"
    "include/i2c-device/exception.hpp" 
    "include/i2c-device/concept.hpp"
    "include/i2c-device/device.hpp"
    "include/i2c-device/bit.hpp"  
    "include/i2c-device/util.hpp"
)

if(WIN32)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/i2c-device/driver/Windows.hpp"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/i2c-device/driver"
)
elseif(LINUX)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/i2c-device/driver/Linux.hpp"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/i2c-device/driver"
)
endif()

set_target_properties(i2c-device PROPERTIES PUBLIC_HEADER
    "${I2CDEVICE_HEADER}"
)

add_library(i2c::i2c-device ALIAS i2c-device)

target_link_libraries(i2c-device PRIVATE project_options
                                         project_warnings)

target_include_directories(i2c-device PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include/>  # <prefix>/include
)

if(WIN32)
message(STATUS ">>> WINDOWS")

elseif(LINUX)
message(STATUS ">>> LINUX")

find_package(fmt CONFIG REQUIRED)
target_link_libraries(i2c-device PRIVATE fmt::fmt)
target_link_libraries(i2c-device PRIVATE i2c)
endif()
