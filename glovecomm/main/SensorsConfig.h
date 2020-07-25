#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"
#include "inttypes.h"

static constexpr gpio_num_t SCL = GPIO_NUM_19;//VERDE 19
static constexpr gpio_num_t SDA = GPIO_NUM_18;//AZUL 18
//static constexpr gpio_num_t AD0 = GPIO_NUM_5;
static constexpr uint32_t CLOCK = 400000;  // range from 100 KHz ~ 400Hz

static constexpr gpio_num_t GY1 = GPIO_NUM_17;//17//thumb
static constexpr gpio_num_t GY2 = GPIO_NUM_16;//16
static constexpr gpio_num_t GY3 = GPIO_NUM_4;//4
static constexpr gpio_num_t GY4 = GPIO_NUM_0;//0
static constexpr gpio_num_t GY5 = GPIO_NUM_2;//2

MPU_t* initSensors(void) {
    MPU_t* MPU = (MPU_t*) malloc(sizeof(MPU_t));
    
    i2c0.begin(SDA, SCL, CLOCK);  // initialize the I2C bus
    MPU->setBus(i2c0);  // set communication bus, for SPI -> pass 'hspi'
    MPU->setAddr(mpud::MPU_I2CADDRESS_AD0_LOW);  // set address or handle, for SPI -> pass 'mpu_spi_handle'
    MPU->testConnection();  // test connection with the chip, return is a error code
    MPU->initialize();  // this will initialize the chip and set default configurations
    MPU->setSampleRate(1000);  // in (Hz)
    MPU->setAccelFullScale(mpud::ACCEL_FS_4G);
    MPU->setGyroFullScale(mpud::GYRO_FS_500DPS);
    MPU->setDigitalLowPassFilter(mpud::DLPF_42HZ);  // smoother data
    MPU->setInterruptEnabled(mpud::INT_EN_RAWDATA_READY);  // enable I	

    //gpio_pad_select_gpio(AD0);
	//gpio_set_direction(AD0, GPIO_MODE_OUTPUT);
	//gpio_set_level(AD0,false);

	gpio_pad_select_gpio(GY1);
    gpio_pad_select_gpio(GY2);
    gpio_pad_select_gpio(GY3);
    gpio_pad_select_gpio(GY4);
    gpio_pad_select_gpio(GY5);

    gpio_set_direction(GY1, GPIO_MODE_OUTPUT);
    gpio_set_direction(GY2, GPIO_MODE_OUTPUT);
    gpio_set_direction(GY3, GPIO_MODE_OUTPUT);
    gpio_set_direction(GY4, GPIO_MODE_OUTPUT);
    gpio_set_direction(GY5, GPIO_MODE_OUTPUT);

	gpio_set_level(GY1,true);
	gpio_set_level(GY2,true);
	gpio_set_level(GY3,true);
	gpio_set_level(GY4,true);
	gpio_set_level(GY5,true);
    return MPU;
}

void switchGyro(int gyroNum){
    gpio_set_level(GY1, true);
    gpio_set_level(GY2, true);
    gpio_set_level(GY3, true);
    gpio_set_level(GY4, true);
    gpio_set_level(GY5, true);
    switch(gyroNum)
    {
        case 1:
            gpio_set_level(GY1, false);
            break;
        case 2:
            gpio_set_level(GY2, false);
            break;
        case 3:
            gpio_set_level(GY3, false);
            break;
        case 4:
            gpio_set_level(GY4, false);
            break;
        case 5:
            gpio_set_level(GY5, false);
            break;
        default:
            gpio_set_level(GY1, false);
            break;
    }
}