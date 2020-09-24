#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := glovecomm

# Path to MPU Driver
EXTRA_COMPONENT_DIRS += $(abspath ./components/MPUdriver)
EXTRA_COMPONENT_DIRS += $(abspath ./components/MPUdriver/include)

COMPONENT_ADD_INCLUDEDIRS := components/include

#Libs
EXTRA_COMPONENT_DIRS += $(abspath ./lib)

include $(IDF_PATH)/make/project.mk


CPPFLAGS += -DI2CBUS_COMPONENT_TRUE=1
