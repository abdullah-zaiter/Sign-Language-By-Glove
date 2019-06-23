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

static constexpr gpio_num_t AD0 = GPIO_NUM_5;
static constexpr gpio_num_t SDA = GPIO_NUM_18;
static constexpr gpio_num_t SCL = GPIO_NUM_19;
static constexpr uint32_t CLOCK = 400000;  // range from 100 KHz ~ 400Hz

bool start_transmission = false;

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
	MPU.setSampleRate(250);  // in (Hz)
	MPU.setAccelFullScale(mpud::ACCEL_FS_4G);
	MPU.setGyroFullScale(mpud::GYRO_FS_500DPS);
	MPU.setDigitalLowPassFilter(mpud::DLPF_42HZ);  // smoother data
	MPU.setInterruptEnabled(mpud::INT_EN_RAWDATA_READY);  // enable I	
	gpio_pad_select_gpio(AD0);
	gpio_set_direction(AD0, GPIO_MODE_OUTPUT);
	gpio_set_level(AD0,false);
	SensorData sensors[SENSORS_QUANTITY];
	

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
	uint16_t aux;
	for (;!start_transmission;);
	for (;;)
	{
		// mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
		// mpud::raw_axes_t gyroRaw;      // holds x, y, z axes as int16

		MPU.motion(&sensors[0].accelRaw, &sensors[0].gyroRaw);
		
		// printf("accel: %+d %+d %+d\n", accelRaw.x, accelRaw.y, accelRaw.z);
		// printf("gyro: %+d %+d %+d\n", gyroRaw[0], gyroRaw[1], gyroRaw[2]);
		mpud::float_axes_t accelG = mpud::accelGravity(sensors[0].accelRaw, mpud::ACCEL_FS_4G);  // raw data to gravity
		mpud::float_axes_t gyroDPS = mpud::gyroDegPerSec(sensors[0].gyroRaw, mpud::GYRO_FS_500DPS);  // raw data to º/s
		
		// printf("accel: %+.2f %+.2f %+.2f\n", cacelG[0], accelG[1], accelG[2]);
		// printf("gyro: %+.2f %+.2f %+.2f\n", gyroDPS.x, gyroDPS.y, gyroDPS.z);
		aux = (uint16_t)sensors[0].gyroRaw[0];
		printf("________aux = %d\n", sensors[0].gyroRaw[0]);
		pCharacteristic_send->setValue(aux);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}


void SampleWrite(void)
{
	run();
} // app_main

void app_main(void) {
	SampleWrite();
} // app_main
