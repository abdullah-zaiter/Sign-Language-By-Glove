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
#include "SensorDataType.h"
#include "StorageConfig.h"
#include "SensorsConfig.h"

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
    ESP_LOGI(TAG, "File written, closing file %s", filename);
    fclose(f);
}

MPU_t* init(void) {
    esp_timer_init();
    initStorage();
    return initSensors();
}

static void run(MPU_t* MPU) {
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
    // vTaskDelay(100 / portTICK_PERIOD_MS);

}

void runSpiffs(void) {

    static HandReading data[BUFFER_SIZE];
    
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
        ESP_LOGI(TAG, "Reopening file %s", filename);
        FILE* f = fopen(filename, "rb");
        if (f == NULL) {
            ESP_LOGE(TAG, "Failed to open file for reading");
            return;
        }
        ESP_LOGI(TAG, "Reading file");
        fread(data, sizeof(HandReading), BUFFER_SIZE, f);
        fclose(f);
        ESP_LOGI(TAG, "Test:\n\t\t\t    data[%d].timestamp = %ud", 2, data[2].timestamp);
        memset(data, 0, sizeof(HandReading)*BUFFER_SIZE);
    }
    
    
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

extern "C" void app_main()
{
    // MPU_t* MPU = init();
    // int cnt = 0;
    // while(cnt < 10) {
    //     run(MPU);
    //     cnt++;
    // }
    
    initStorage();
    runSpiffs();
}