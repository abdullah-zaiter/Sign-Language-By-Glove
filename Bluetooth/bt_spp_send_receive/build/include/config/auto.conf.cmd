deps_config := \
	/home/skira/esp/esp-idf/components/app_trace/Kconfig \
	/home/skira/esp/esp-idf/components/aws_iot/Kconfig \
	/home/skira/esp/esp-idf/components/bt/Kconfig \
	/home/skira/esp/esp-idf/components/driver/Kconfig \
	/home/skira/esp/esp-idf/components/esp32/Kconfig \
	/home/skira/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/skira/esp/esp-idf/components/esp_event/Kconfig \
	/home/skira/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/skira/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/skira/esp/esp-idf/components/ethernet/Kconfig \
	/home/skira/esp/esp-idf/components/fatfs/Kconfig \
	/home/skira/esp/esp-idf/components/freemodbus/Kconfig \
	/home/skira/esp/esp-idf/components/freertos/Kconfig \
	/home/skira/esp/esp-idf/components/heap/Kconfig \
	/home/skira/esp/esp-idf/components/libsodium/Kconfig \
	/home/skira/esp/esp-idf/components/log/Kconfig \
	/home/skira/esp/esp-idf/components/lwip/Kconfig \
	/home/skira/esp/esp-idf/components/mbedtls/Kconfig \
	/home/skira/esp/esp-idf/components/mdns/Kconfig \
	/home/skira/esp/esp-idf/components/mqtt/Kconfig \
	/home/skira/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/skira/esp/esp-idf/components/openssl/Kconfig \
	/home/skira/esp/esp-idf/components/pthread/Kconfig \
	/home/skira/esp/esp-idf/components/spi_flash/Kconfig \
	/home/skira/esp/esp-idf/components/spiffs/Kconfig \
	/home/skira/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/skira/esp/esp-idf/components/vfs/Kconfig \
	/home/skira/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/skira/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/skira/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/skira/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/skira/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
