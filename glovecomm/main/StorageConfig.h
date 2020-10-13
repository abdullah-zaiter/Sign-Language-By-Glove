#include "esp_spiffs.h"
#include <string.h>
#define BUFFER_SIZE 1000//4000

uint8_t * getBytesDataPackage(HandReading* , uint8_t * );

void printFileSize(FILE* f) {
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    ESP_LOGI(TAG, "FILE SIZE = %d", size);
    fseek(f, 0L, SEEK_SET);
}

void initStorage(void){
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 1,
    .format_if_mount_failed = true
    };
    esp_spiffs_format(NULL);
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
        readings[i].timestamp = esp_random();
        for (uint16_t j = 0; j < SENSORS_QUANTITY; j++)
        {
            readings[i].imu[j].accel[X] = (__int8_t) esp_random();
            readings[i].imu[j].accel[Y] = (__int8_t) esp_random();
            readings[i].imu[j].accel[Z] = (__int8_t) esp_random();
            readings[i].imu[j].gyro[X] = (__int8_t) esp_random();
            readings[i].imu[j].gyro[Y] = (__int8_t) esp_random();
            readings[i].imu[j].gyro[Z] = (__int8_t) esp_random();
        }
    }
}


uint8_t * getBytesDataPackage(HandReading* readings, uint8_t * dataPkg) {
    ESP_LOGI(TAG, "Starting Data Pkg ");
    for (uint16_t i = 0; i < 1; i++)
    {
        dataPkg[i] = (__uint8_t) (readings[i].timestamp) >> 24;
        dataPkg[i+1] = (__uint8_t) (readings[i].timestamp >> 16);
        dataPkg[i+2] = (__uint8_t) (readings[i].timestamp >> 8) ;
        dataPkg[i+3] = (__uint8_t) (readings[i].timestamp >> 0) ;

        ESP_LOGI(TAG, "Finish timestamp ");

        for (uint16_t j = 0; j < SENSORS_QUANTITY; j++)
        {
            dataPkg[i+j+4] = readings[i].imu[j].accel[X];
            dataPkg[i+j+5] = readings[i].imu[j].accel[Y];
            dataPkg[i+j+6] = readings[i].imu[j].accel[Z];
            dataPkg[i+j+7] = readings[i].imu[j].gyro[X];
            dataPkg[i+j+8] = readings[i].imu[j].gyro[Y];
            dataPkg[i+j+9] = readings[i].imu[j].gyro[Z];
        }
    }


    ESP_LOGI(TAG, "Created Data Pkg %d", dataPkg[0]);
    ESP_LOGI(TAG, "Created Data Pkg %d", dataPkg[1]);
    ESP_LOGI(TAG, "Created Data Pkg %d", dataPkg[2]);
    ESP_LOGI(TAG, "Created Data Pkg %d", dataPkg[3]);
    ESP_LOGI(TAG, "Created Data Pkg %d", dataPkg[4]);

    return dataPkg;
}