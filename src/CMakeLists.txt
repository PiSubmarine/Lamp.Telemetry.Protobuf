include("${_build_cmake_local}")

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)

set(LAMP_TELEMETRY_PROTO "${CMAKE_CURRENT_SOURCE_DIR}/PiSubmarine/Lamp/Telemetry/Protobuf/Lamp.proto")
protobuf_generate_cpp(LAMP_TELEMETRY_PROTO_SRCS LAMP_TELEMETRY_PROTO_HDRS "${LAMP_TELEMETRY_PROTO}")

message("Adding library: ${PISUBMARINE_MODULE_NAME}")
add_library(${PISUBMARINE_MODULE_NAME}
        PiSubmarine/Lamp/Telemetry/Protobuf/Deserializer.cpp
        PiSubmarine/Lamp/Telemetry/Protobuf/ErrorCode.cpp
        PiSubmarine/Lamp/Telemetry/Protobuf/Serializer.cpp
        "../public/PiSubmarine/Lamp/Telemetry/Protobuf/Deserializer.h"
        "../public/PiSubmarine/Lamp/Telemetry/Protobuf/ErrorCode.h"
        "../public/PiSubmarine/Lamp/Telemetry/Protobuf/Serializer.h"
        ${LAMP_TELEMETRY_PROTO_SRCS}
        ${LAMP_TELEMETRY_PROTO_HDRS})
target_include_directories(${PISUBMARINE_MODULE_NAME} PUBLIC "../public" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
PiSubmarineInitTarget(${PISUBMARINE_MODULE_NAME})

PiSubmarineAddDependency("https://github.com/PiSubmarine/Lamp.Telemetry.Api.git" "")
target_link_libraries(${PISUBMARINE_MODULE_NAME} PUBLIC PiSubmarine.Lamp.Telemetry.Api)

PiSubmarineAddDependency("https://github.com/PiSubmarine/Telemetry.Api.git" "")
target_link_libraries(${PISUBMARINE_MODULE_NAME} PUBLIC PiSubmarine.Telemetry.Api)

target_link_libraries(${PISUBMARINE_MODULE_NAME} PRIVATE protobuf::libprotobuf)
