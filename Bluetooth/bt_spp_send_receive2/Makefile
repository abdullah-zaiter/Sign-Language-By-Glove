#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := bt_spp_acceptor_demo

COMPONENT_ADD_INCLUDEDIRS := components

# Path to MPU Driver
EXTRA_COMPONENT_DIRS += $(abspath ./components/esp32-MPU-driver)
EXTRA_COMPONENT_DIRS += ${HOME}/esp/libraries/I2Cbus

include $(IDF_PATH)/make/project.mk
