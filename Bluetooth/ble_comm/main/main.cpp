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


#include "BLEUtils.h"
#include "BLEServer.h"
#include <esp_log.h>
#include <string>
#include <sys/time.h>
#include <sstream>
#include "BLEDevice.h"
#include "GeneralUtils.h"

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"
#include "SensorDataType.h"


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_RECEIVE_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_SEND_UUID "beb5483e-36e1-4688-b7f5-ea07361b26aa"

#define CHARACTERISTIC_SEND2_0_UUID "5b0c6f3f-4968-4851-8003-5ac7e184cd0e"
#define CHARACTERISTIC_SEND2_1_UUID "2afe407a-963d-4806-b293-f8c3abee4a95"
#define CHARACTERISTIC_SEND2_2_UUID "b6af070b-9c1c-4ad4-a5b8-2aaf7730047e"
#define CHARACTERISTIC_SEND2_3_UUID "3c6f02ec-4037-4ac1-afc8-ee407f5f130f"
#define CHARACTERISTIC_SEND2_4_UUID "6eed4e5b-7dbb-4a05-b1b1-b350ddd099a4"



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

bool start_transmission = false;

bool end_transmission = false;


class MyCallbacks: public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {
		start_transmission = !start_transmission;
	}
};

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


	//////////////////
	GeneralUtils::dumpInfo();
	BLEDevice::init("ESP32");
	BLEServer *pServer = BLEDevice::createServer();

	BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

	BLECharacteristic *pCharacteristic_send = pService->createCharacteristic(
		BLEUUID(CHARACTERISTIC_SEND_UUID),
		BLECharacteristic::PROPERTY_READ 
	);

	BLECharacteristic *pCharacteristic_receive = pService->createCharacteristic(
		BLEUUID(CHARACTERISTIC_RECEIVE_UUID),
		BLECharacteristic::PROPERTY_WRITE 
	);
	pCharacteristic_receive->setCallbacks(new MyCallbacks());
	pCharacteristic_send->setValue("null");
	pService->start();
	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();


	for (;!start_transmission;);
	printf("Start Transmission");
	int delay = 200000;
	int numpack = 0;
	uint8_t aux[30];
	while(delay > 200)
	{	
		int64_t startT = esp_timer_get_time();
		printf("\n");
		for (int i = 0; i < 100; i++)
		{				
			for(int j = 0; j < 6; j++)
			{
				switchGyro(j);
				MPU.motion(&accelRaw, &gyroRaw);
				mpud::float_axes_t accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_4G);  // raw data to gravity
				mpud::float_axes_t gyroDPS = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS);  // raw data to º/s
			}
			vTaskDelay(100 / portTICK_PERIOD_MS);

			aux[0] = (uint8_t)1;//gyroRaw[0]; //sensors[0].
			aux[1] = (uint8_t)rand();//gyroRaw[1]; //sensors[0].
			aux[2] = (uint8_t)2;//gyroRaw[2]; //sensors[0].
			aux[3] = (uint8_t)rand();//gyroRaw[0]; //sensors[0].
			aux[4] = (uint8_t)3;//gyroRaw[1]; //sensors[0].
			aux[5] = (uint8_t)rand();//gyroRaw[2]; //sensors[0].

			aux[6] = (uint8_t)4;//gyroRaw[0]; //sensors[0].
			aux[7] = (uint8_t)i;//gyroRaw[1]; //sensors[0].
			aux[8] = (uint8_t)5;//gyroRaw[2]; //sensors[0].
			aux[9] =  (uint8_t)rand();//gyroRaw[0]; //sensors[0].
			aux[10] = (uint8_t)6;//gyroRaw[1]; //sensors[0].
			aux[11] = (uint8_t)rand();//gyroRaw[2]; //sensors[0].
			
			aux[12] = (uint8_t)7;//gyroRaw[0]; //sensors[0].
			aux[13] = (uint8_t)rand();//gyroRaw[1]; //sensors[0].
			aux[14] = (uint8_t)8;//gyroRaw[2]; //sensors[0].
			aux[15] = (uint8_t)rand();//gyroRaw[0]; //sensors[0].
			aux[16] = (uint8_t)9;//gyroRaw[1]; //sensors[0].
			aux[17] = (uint8_t)rand();//gyroRaw[2]; //sensors[0].
			
			aux[18] = (uint8_t)10;//gyroRaw[0]; //sensors[0].
			aux[19] = (uint8_t)rand();//gyroRaw[1]; //sensors[0].
			aux[20] = (uint8_t)11;//gyroRaw[2]; //sensors[0].
			aux[21] =  (uint8_t)rand();//gyroRaw[0]; //sensors[0].
			aux[22] = (uint8_t)12;//gyroRaw[1]; //sensors[0].
			aux[23] = (uint8_t)rand();//gyroRaw[2]; //sensors[0].
			
			aux[24] = (uint8_t)13;//gyroRaw[2]; //sensors[0].
			aux[25] = (uint8_t)rand();//gyroRaw[0]; //sensors[0].
			aux[26] = (uint8_t)14;//gyroRaw[1]; //sensors[0].
			aux[27] = (uint8_t)rand();//gyroRaw[2]; //sensors[0].
			aux[28] = (uint8_t)15;//gyroRaw[0]; //sensors[0].
			aux[29] = (uint8_t)rand();//gyroRaw[1]; //sensors[0].

			pCharacteristic_send->setValue(aux,30);
			int64_t startDelay = esp_timer_get_time();
			while (esp_timer_get_time() - startDelay < delay);
			
		}
		printf("Numero Pacote[%d]\n", numpack);
		numpack++;
		// int64_t endT = esp_timer_get_time();
		// int64_t delta = endT - startT;
		// int64_t freq = 100/delta;
		// printf("Delay: %d\n", delay);
		// // printf("100 sends\n");
		// printf("Send time: %" PRId64 "us\n", delta/100);
		// // printf("%" PRId64 "Hz\n", freq);
		delay = delay-1998;
		aux[7] = (uint8_t)200; //Signal to end current package capture
		pCharacteristic_send->setValue(aux,30);
		int64_t startDelay = esp_timer_get_time();
		while (esp_timer_get_time() - startDelay < 250000); //DEBUG, wait 2,5 seconds between each package
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