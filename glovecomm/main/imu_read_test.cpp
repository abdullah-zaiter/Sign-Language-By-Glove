/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"
#include "SensorDataType.hpp"
#include "inttypes.h"

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

static void init(MPU_t* MPU) {

  esp_timer_init();
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
}

static void run(MPU_t* MPU) {
    mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
    mpud::raw_axes_t gyroRaw;      // holds x, y, z axes as int16
    ImuReading sensors[SENSORS_QUANTITY];
    for(int j = 0; j < SENSORS_QUANTITY; j++)
    {
        switchGyro(j);
        MPU->motion(&accelRaw, &gyroRaw);
		    uint32_t timeNow = (esp_timer_get_time());
        sensors[j].timestamp = timeNow;
        sensors[j].imu.accel[X] = (__int8_t)(accelRaw[X] >> 4);
        sensors[j].imu.accel[Y] = (__int8_t)(accelRaw[Y] >> 4);
        sensors[j].imu.accel[Z] = (__int8_t)(accelRaw[Z] >> 4);
        sensors[j].imu.gyro[X] = (__int8_t)(gyroRaw[X] >> 4);
        sensors[j].imu.gyro[Y] = (__int8_t)(gyroRaw[Y] >> 4);
        sensors[j].imu.gyro[Z] = (__int8_t)(gyroRaw[Z] >> 4);
    }
    // vTaskDelay(100 / portTICK_PERIOD_MS);

}

extern "C" void app_main()
{
	  MPU_t MPU;
    init(&MPU);
    int cnt = 0;
    while(cnt < 10) {
      run(&MPU);
      cnt++;
    }
}

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