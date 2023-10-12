cmake_minimum_required(VERSION 3.0)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

project(eCALcpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
set (OpenCV_USE_STATIC_LIBS ON)
find_package(eCAL REQUIRED)
find_package(Protobuf REQUIRED)
message(STATUS "Protobuf found: = ${Protobuf_FOUND}")
message(STATUS "Protobuf version: = ${Protobuf_VERSION}")
message(STATUS "Protobuf libs: = ${Protobuf_LIBRARIES}")
message(STATUS "Protobuf libs: = ${Protobuf_PROTOC_LIBRARIES}")
message(STATUS "Protobuf libs: = ${Protobuf_LITE_LIBRARIES}")
message(STATUS "Protobuf DIR: = ${Protobuf_DIR}")

set(TARGET_NAME_SUB ecal_receive_numeric)
set(TARGET_NAME_PUB ecal_send_numeric)
set(TARGET_NAME_PUB_WEBCAM ecal_send_webcam)
set(ecal_rcv_src
  receiveNumeric.cpp
)
set(ecal_snd_src
        sendNumeric.cpp
)
set(ecal_snd_webcam
        send_webcam.cpp
)

set(protobuf_files
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/mensaje_data.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/mensaje_enum.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/mensaje_main.proto
        ${CMAKE_CURRENT_SOURCE_DIR}/proto/mi_mensaje.proto
        )

add_executable(${TARGET_NAME_SUB} ${ecal_rcv_src})
add_executable(${TARGET_NAME_PUB} ${ecal_snd_src})
add_executable(${TARGET_NAME_PUB_WEBCAM} ${ecal_snd_webcam})

PROTOBUF_TARGET_CPP(${TARGET_NAME_SUB} ${CMAKE_CURRENT_SOURCE_DIR}/proto/ ${protobuf_files})
PROTOBUF_TARGET_CPP(${TARGET_NAME_PUB} ${CMAKE_CURRENT_SOURCE_DIR}/proto/ ${protobuf_files})
PROTOBUF_TARGET_CPP(${TARGET_NAME_PUB_WEBCAM} ${CMAKE_CURRENT_SOURCE_DIR}/proto/ ${protobuf_files})

target_link_libraries(${TARGET_NAME_SUB}
        PRIVATE
        eCAL::core
        ${Protobuf_LIBRARIES}
        )

target_link_libraries(${TARGET_NAME_PUB}
        PRIVATE
        eCAL::core
        ${Protobuf_LIBRARIES}
)

target_link_libraries(${TARGET_NAME_PUB_WEBCAM}
        PRIVATE
        eCAL::core
        ${Protobuf_LIBRARIES}
        ${OpenCV_LIBS}
)