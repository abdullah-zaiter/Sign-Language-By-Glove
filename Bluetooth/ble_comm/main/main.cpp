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

static char LOG_TAG[] = "SampleWrite";

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_RECEIVE_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_SEND_UUID "beb5483e-36e1-4688-b7f5-ea07361b26aa"

bool start_transmission = false;

class MyCallbacks: public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {
		start_transmission = true;
	}
};


static void run() {
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
	
	
	uint16_t i = 0;
	for (;!start_transmission;);
	for (;;)
	{
		pCharacteristic_send->setValue(i);
		i++;
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}


void SampleWrite(void)
{
	run();
} // app_main

void app_main(void) {
	//Sample_MLE_15();
	//Sample1();
	//SampleAsyncScan();
	// SampleClient();
	// SampleClient_Notify();
	//SampleClientAndServer();
	//SampleClientDisconnect();
	//SampleClientWithWiFi();
	//SampleNotify();
	//SampleRead();
	//SampleSensorTag();
	//SampleScan();
	// SampleServer();
	SampleWrite();
} // app_main
