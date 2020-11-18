/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

static const char *TAG = "glovecomm";

#include <stdio.h>
#include <string>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "StorageConfig.h"
#include "SensorsConfig.h"
#include "GattsServer.h"

FILE* open_file(const char * filePrefix){
    std::string filenameString = std::string(std::string("") + filePrefix + std::to_string(fileCount) + ".bin");
    const char * filename = filenameString.c_str();
    ESP_LOGI(TAG, "Opening file %s", filename);
    FILE* f = fopen(filename, "wb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        fileCount--;
    }
    fileCount++;
    return f;
}

void writeToFile(HandReading* readings, const char* filename){
    FILE* f = open_file(filename);
    fwrite(readings, sizeof(HandReading), BUFFER_SIZE, f);
    ESP_LOGI(TAG, "File written, closing file");
    fclose(f);
}

MPU_t* init(void) {
    // initStorage();
    return initSensors();
}

static void run(MPU_t* MPU, int count) {
    mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
    mpud::raw_axes_t gyroRaw;      // holds x, y, z axes as int16
    HandReading sensors;

    for(int j = 0; j < SENSORS_QUANTITY; j++)
    {
        switchGyro(j);
        if (j==2)   sensors.timestamp = (uint32_t)(esp_timer_get_time());
        MPU->motion(&accelRaw, &gyroRaw);
        sensors.imu[j].accel[X] = (__int8_t)(accelRaw[X] >> 4);
        sensors.imu[j].accel[Y] = (__int8_t)(accelRaw[Y] >> 4);
        sensors.imu[j].accel[Z] = (__int8_t)(accelRaw[Z] >> 4);
        sensors.imu[j].gyro[X] = (__int8_t)(gyroRaw[X] >> 4);
        sensors.imu[j].gyro[Y] = (__int8_t)(gyroRaw[Y] >> 4);
        sensors.imu[j].gyro[Z] = (__int8_t)(gyroRaw[Z] >> 4);
    }
    data[count] = sensors;
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void runSpiffs(void) {

    
    
    for (size_t i = 0; i < 3; i++)
    {
        fillSensorDataStructure(data);
        writeToFile(data, "/spiffs/sensordata");
        ESP_LOGI(TAG, "Test:\n\t\t\t    data[%d].timestamp = %ud", 2, data[2].timestamp);
    }
    
    for (size_t i = 1; i < fileCount; i++)
    {
        std::string filenameString = std::string(std::string("") + "/spiffs/sensordata" + std::to_string(i) + ".bin");
        const char * filename = filenameString.c_str();
        ESP_LOGI(TAG, "Reopening and reading file %s", filename);
        FILE* f = fopen(filename, "rb");
        if (f == NULL) {
            ESP_LOGE(TAG, "Failed to open file for reading");
            return;
        }
        fread(data, sizeof(HandReading), BUFFER_SIZE, f);
        fclose(f);
        ESP_LOGI(TAG, "Test:\n\t\t\t    data[%d].timestamp = %ud", 2, data[2].timestamp);
        memset(data, 0, sizeof(data));
    }
    
    
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

static constexpr gpio_num_t GPIO_START = GPIO_NUM_14;

void waitForGPIO() {
    gpio_pad_select_gpio(GPIO_START);
    gpio_set_direction(GPIO_START, GPIO_MODE_INPUT_OUTPUT);
	gpio_set_level(GPIO_START,false);

    int start = 0;
    while(true) {
        ESP_LOGI(TAG, "START  = %d", start);
        start = gpio_get_level(GPIO_START);
        
        if( start == 1 ) {
            break;
        }
    }

	gpio_set_level(GPIO_START,false);
    ESP_LOGI(TAG, "START MAIN");
}

extern "C" void app_main()
{
    
    esp_timer_init();
    int64_t timeInit = esp_timer_get_time();
    ESP_LOGI(TAG, "Start Time %lld us", timeInit);
    waitForGPIO();
    
    int64_t time_init = esp_timer_get_time();

    MPU_t* MPU = init();

    int64_t time_initEND = esp_timer_get_time();
    ESP_LOGI(TAG, "MPU Init duration %lld us", time_initEND - time_init);

    time_init = esp_timer_get_time();
    int flag = 1;
    int gpio = 0;
    while(true) {
        // testSensors(MPU);
        gpio = gpio_get_level(GPIO_START);
        
        if( gpio == 1 ) {
            flag = (flag%5)+1; 
	        gpio_set_level(GPIO_START,false);
        }

        testSensor(MPU, flag);
    }

    int cnt = 0;
    while(cnt < BUFFER_SIZE) {
        run(MPU, cnt);
        cnt++;
    }
    
    time_initEND = esp_timer_get_time();
    ESP_LOGI(TAG, "MPU RUN duration %lld us", time_initEND - time_init);
    // memccpy((uint8_t*)data, currReading, 1, BUFFER_SIZE);

    // initStorage();
    // // runSpiffs();
    // fillDataStructureSequentially((uint8_t*)data);
    setDataAddress((uint8_t*)data);
    init_gatts_server();
}

