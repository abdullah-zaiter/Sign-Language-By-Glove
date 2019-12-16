/**
 * Main file for running the BLE samples.
 */
extern "C" {
	void app_main(void);
}

/**
 * Create a BLE Server such that when a client connects and requests a change to the characteristic
 * value, the callback associated with the server will be invoked such that the server can perform
 * some action based on the new value.  The action in this sample is merely to log the new value to
 * the console.
 */

#include <inttypes.h>
#include <cstdlib>
	//Seed Random srand()

#include <esp_log.h>
#include <string>
#include <sys/time.h>
#include <sstream>
#include "GeneralUtils.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"
#include "SensorDataType.h"



#define NUM_CHARAC 5

static constexpr gpio_num_t SCL = GPIO_NUM_19;//VERDE 19
static constexpr gpio_num_t SDA = GPIO_NUM_18;//AZUL 18
//static constexpr gpio_num_t AD0 = GPIO_NUM_5;
static constexpr uint32_t CLOCK = 400000;  // range from 100 KHz ~ 400Hz

static constexpr gpio_num_t GY1 = GPIO_NUM_17;//17//thumb
static constexpr gpio_num_t GY2 = GPIO_NUM_16;//16
static constexpr gpio_num_t GY3 = GPIO_NUM_4;//4
static constexpr gpio_num_t GY4 = GPIO_NUM_0;//0
static constexpr gpio_num_t GY5 = GPIO_NUM_2;//2


void switchGyro(int);


static void run() {
	i2c0.begin(SDA, SCL, CLOCK);  // initialize the I2C bus
	MPU_t MPU;         // create an object
	MPU.setBus(i2c0);  // set communication bus, for SPI -> pass 'hspi'
	MPU.setAddr(mpud::MPU_I2CADDRESS_AD0_LOW);  // set address or handle, for SPI -> pass 'mpu_spi_handle'
	MPU.testConnection();  // test connection with the chip, return is a error code
	MPU.initialize();  // this will initialize the chip and set default configurations
	MPU.setSampleRate(1000);  // in (Hz)
	MPU.setAccelFullScale(mpud::ACCEL_FS_4G);
	MPU.setGyroFullScale(mpud::GYRO_FS_500DPS);
	MPU.setDigitalLowPassFilter(mpud::DLPF_42HZ);  // smoother data
	MPU.setInterruptEnabled(mpud::INT_EN_RAWDATA_READY);  // enable I	
	//gpio_pad_select_gpio(AD0);
	//gpio_set_direction(AD0, GPIO_MODE_OUTPUT);
	//gpio_set_level(AD0,false);
	SensorData sensors[SENSORS_QUANTITY];
	//////////////////

	mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
	mpud::raw_axes_t gyroRaw;      // holds x, y, z axes as int16


	//////////////////
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

	int delay = 150000;
	uint8_t aux[30];
	while(true)
	{	
		int64_t startT = esp_timer_get_time();
		for(int j = 1; j < 6; j++)
		{
		printf("\n");
		for (int i = 0; i < 100; i++)
			{
				switchGyro(j);	
				MPU.motion(&accelRaw, &gyroRaw);
				mpud::float_axes_t accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_4G);  // raw data to gravity
				mpud::float_axes_t gyroDPS = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS);  // raw data to º/s
				
				// Test Sensors
				printf("Sensor[%d] \t", j);
				printf("accel: [%+6.2f %+6.2f %+6.2f]", accelG[0], accelG[1], accelG[2]);
				printf("gyro: [%+7.2f %+7.2f %+7.2f]\n", gyroDPS.x, gyroDPS.y, gyroDPS.z);
				
				vTaskDelay(100 / portTICK_PERIOD_MS);				
			}	
		}
	}
}


void SampleWrite(void)
{
	run();
} // app_main

void app_main(void) {
	SampleWrite();
} // app_main


//MPU6050 Functions
void switchGyro(int gyroNum)
{
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