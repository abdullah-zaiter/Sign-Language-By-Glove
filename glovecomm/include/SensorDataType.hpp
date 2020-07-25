#include "mpu/types.hpp"

#define SENSORS_QUANTITY 5
#define AXIS_QUANTITY 3
#define X 0
#define Y 1
#define Z 2

typedef struct imu{
    __int8_t accel[AXIS_QUANTITY];
    __int8_t gyro[AXIS_QUANTITY];  
} Imu;

typedef struct imuReading {
    Imu imu;
    uint32_t timestamp; 
} ImuReading;
