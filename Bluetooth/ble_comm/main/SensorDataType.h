#include "mpu/types.hpp"
#define SENSORS_QUANTITY 5
typedef struct sensor_data{
    mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
    mpud::raw_axes_t gyroRaw;  
} SensorData;
