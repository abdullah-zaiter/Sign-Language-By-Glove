/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

static const char *TAG = "glovecomm";

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "SensorDataType.h"
#include "StorageConfig.h"
#include "SensorsConfig.h"

void switchGyro(int);

FILE* open_file(const char* filename){
    ESP_LOGI(TAG, "Opening file");
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
    }
    return f;
}

void writeToFile(ImuReading* readings, const char* filename){
    FILE* f = open_file(filename);
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
        fprintf(f, "%" PRId8 ",%" PRId8 ",%" PRId8 ",%" PRId8 ",%" PRId8 ",%" PRId8 ",%d", readings[i].imu.accel[X], readings[i].imu.accel[Y], readings[i].imu.accel[Z], readings[i].imu.gyro[X],readings[i].imu.gyro[Y],readings[i].imu.gyro[Z], readings[i].timestamp);
    fclose(f);
    ESP_LOGI(TAG, "File written");
}

MPU_t* init(void) {
    esp_timer_init();
    initStorage();
    return initSensors();
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

void runSpiffs(void) {
    ImuReading* data;
    data = (ImuReading*) malloc(BUFFER_SIZE * sizeof(ImuReading));
    fillSensorDataStructure(data);
    writeToFile(data, "/spiffs/sensordata.csv");
    free(data);


    ESP_LOGI(TAG, "Reading file");
    FILE* f = fopen("/spiffs/sensordata.csv", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    char line[64];
    fgets(line, sizeof(line), f);
    fclose(f);
    char* pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);

    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

extern "C" void app_main()
{
    MPU_t* MPU = init();
    int cnt = 0;
    while(cnt < 10) {
        run(MPU);
        cnt++;
    }
    runSpiffs();
}