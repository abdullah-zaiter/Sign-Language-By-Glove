#include "esp_spiffs.h"
#include "SensorDataType.h"

#define BUFFER_SIZE 1000
HandReading data[BUFFER_SIZE];
uint fileCount = 1;

void initStorage(void){
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

void fillSensorDataStructure(HandReading* readings) {
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        for (uint16_t j = 0; j < SENSORS_QUANTITY; j++)
        {
            readings[i].imu[j].accel[X] = (__int8_t) esp_random();
            readings[i].imu[j].accel[Y] = (__int8_t) esp_random();
            readings[i].imu[j].accel[Z] = (__int8_t) esp_random();
            readings[i].imu[j].gyro[X] = (__int8_t) esp_random();
            readings[i].imu[j].gyro[Y] = (__int8_t) esp_random();
            readings[i].imu[j].gyro[Z] = (__int8_t) esp_random();
        }
        readings[i].timestamp = esp_random();
    }
}


void fillDataStructureSequentially(uint8_t* readings) {
    for (uint16_t i = 0; i < (BUFFER_SIZE*sizeof(HandReading)); i++)
        readings[i] = i%256;        
}