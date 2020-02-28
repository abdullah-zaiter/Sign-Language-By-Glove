# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/bt/include $(PROJECT_PATH)/components/bt/bluedroid/api/include/api
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/bt -lbt -L $(PROJECT_PATH)/components/bt/lib -lbtdm_app
COMPONENT_LINKER_DEPS += $(PROJECT_PATH)/components/bt/lib/libbtdm_app.a
COMPONENT_SUBMODULES += $(PROJECT_PATH)/components/bt/lib
COMPONENT_LIBRARIES += bt
component-bt-build: 
