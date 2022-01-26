
#ifndef I2C_DEVICE_EXPORT_H
#define I2C_DEVICE_EXPORT_H

#ifdef I2C_DEVICE_STATIC_DEFINE
#  define I2C_DEVICE_EXPORT
#  define I2C_DEVICE_NO_EXPORT
#else
#  ifndef I2C_DEVICE_EXPORT
#    ifdef i2c_device_EXPORTS
        /* We are building this library */
#      define I2C_DEVICE_EXPORT 
#    else
        /* We are using this library */
#      define I2C_DEVICE_EXPORT 
#    endif
#  endif

#  ifndef I2C_DEVICE_NO_EXPORT
#    define I2C_DEVICE_NO_EXPORT 
#  endif
#endif

#ifndef I2C_DEVICE_DEPRECATED
#  define I2C_DEVICE_DEPRECATED __declspec(deprecated)
#endif

#ifndef I2C_DEVICE_DEPRECATED_EXPORT
#  define I2C_DEVICE_DEPRECATED_EXPORT I2C_DEVICE_EXPORT I2C_DEVICE_DEPRECATED
#endif

#ifndef I2C_DEVICE_DEPRECATED_NO_EXPORT
#  define I2C_DEVICE_DEPRECATED_NO_EXPORT I2C_DEVICE_NO_EXPORT I2C_DEVICE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef I2C_DEVICE_NO_DEPRECATED
#    define I2C_DEVICE_NO_DEPRECATED
#  endif
#endif

#endif /* I2C_DEVICE_EXPORT_H */
