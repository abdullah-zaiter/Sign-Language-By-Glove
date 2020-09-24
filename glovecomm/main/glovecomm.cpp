/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

static const char *TAG = "glovecomm";

#include <stdio.h>
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
// #include "driver/gpio.h"
// #include "SensorDataType.h"
// #include "StorageConfig.h"
// #include "SensorsConfig.h"
#include "gatts_server.h"


// FILE* open_file(const char* filename, const char* mode){
//     ESP_LOGI(TAG, "Opening file %s", filename);
//     FILE* f = fopen(filename, mode);
//     if (f == NULL)
//         ESP_LOGE(TAG, "Failed to open file for writing");
//     return f;
// }

// void writeToFile(FILE* f, HandReading* readings){
//     int written = fwrite(readings, sizeof(HandReading), BUFFER_SIZE, f);
//     if (written != BUFFER_SIZE)
//         ESP_LOGE(TAG, "Error writing data, written= %d, BUFFER_SIZE=%d", written, BUFFER_SIZE);
//     else
//         ESP_LOGI(TAG, "File written, closing file");
    
//     fclose(f);
//     f = NULL;
// }

// MPU_t* init(void) {
//     esp_timer_init();
//     initStorage();
//     return initSensors();
// }

// static void run(MPU_t* MPU) {
//     mpud::raw_axes_t accelRaw;     // holds x, y, z axes as int16
//     mpud::raw_axes_t gyroRaw;      // holds x, y, z axes as int16
//     HandReading sensors;
//     for(int j = 0; j < SENSORS_QUANTITY; j++)
//     {
//         switchGyro(j);
//         MPU->motion(&accelRaw, &gyroRaw);
//         if (j==2)   sensors.timestamp = (uint32_t)(esp_timer_get_time());
//         sensors.imu[j].accel[X] = (__int8_t)(accelRaw[X] >> 4);
//         sensors.imu[j].accel[Y] = (__int8_t)(accelRaw[Y] >> 4);
//         sensors.imu[j].accel[Z] = (__int8_t)(accelRaw[Z] >> 4);
//         sensors.imu[j].gyro[X] = (__int8_t)(gyroRaw[X] >> 4);
//         sensors.imu[j].gyro[Y] = (__int8_t)(gyroRaw[Y] >> 4);
//         sensors.imu[j].gyro[Z] = (__int8_t)(gyroRaw[Z] >> 4);
//     }
//     // vTaskDelay(100 / portTICK_PERIOD_MS);

// }

// void printFileSize(FILE* f) {
//     fseek(f, 0L, SEEK_END);
//     int size = ftell(f);
//     ESP_LOGI(TAG, "FILE SIZE = %d", size);
//     fseek(f, 0L, SEEK_SET);
// }

// void runSpiffs(void) {

//     static HandReading data[BUFFER_SIZE];
    
//     FILE* f = open_file("/spiffs/sensordata.bin", "wb");
//     printFileSize(f);
//     fillSensorDataStructure(data);
//     ESP_LOGI(TAG, "     Test:    data[%d].timestamp = %ud", 2, data[2].timestamp);
//     writeToFile(f, data);

//     f = open_file("/spiffs/sensordata.bin", "ab");
//     printFileSize(f);
//     fillSensorDataStructure(data);
//     ESP_LOGI(TAG, "     Test:    data[%d].timestamp = %ud", 2, data[2].timestamp);
//     writeToFile(f, data);

//     f = open_file("/spiffs/sensordata.bin", "ab");
//     fillSensorDataStructure(data);
//     ESP_LOGI(TAG, "     Test:    data[%d].timestamp = %ud", 2, data[2].timestamp);
//     writeToFile(f, data);
    
//     memset(data, 0, sizeof(data));


//     ESP_LOGI(TAG, "Reopening file");
//     f = fopen("/spiffs/sensordata.bin", "rb");
   
//     ESP_LOGI(TAG, "DATA SIZE = %d", sizeof(data));
//     printFileSize(f);
    
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for reading");
//         return;
//     }
    
//     ESP_LOGI(TAG, "Reading file");
    
//     fread(data, sizeof(HandReading), BUFFER_SIZE, f);
//     ESP_LOGI(TAG, "Test:    data[%d].timestamp = %ud", 2, data[2].timestamp);
    
//     memset(data, 0, sizeof(data));
//     fread(data, sizeof(HandReading), BUFFER_SIZE, f);
//     ESP_LOGI(TAG, "Test:    data[%d].timestamp = %ud", 2, data[2].timestamp);
    
//     memset(data, 0, sizeof(data));
//     fread(data, sizeof(HandReading), BUFFER_SIZE, f);
//     ESP_LOGI(TAG, "Test:    data[%d].timestamp = %ud", 2, data[2].timestamp);
    
    
//     fclose(f);    
//     f = NULL;
    
//     esp_vfs_spiffs_unregister(NULL);
//     ESP_LOGI(TAG, "SPIFFS unmounted");
// }

extern "C" void app_main()
{
    // MPU_t* MPU = init();
    // int cnt = 0;
    // while(cnt < 10) {
    //     run(MPU);
    //     cnt++;
    // }
    // initStorage();
    // runSpiffs();
    init_gatts_server();
}