cmake_minimum_required(VERSION 3.14)

project(QDeconz2MQTT LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# https://stackoverflow.com/a/43355735
function(IndicateExternalFile _target)
    foreach(_file IN ITEMS ${ARGN})
        if ((IS_ABSOLUTE "${_file}" AND EXISTS "${_file}") OR
            (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_file}"))
            target_sources(${_target} PRIVATE "${_file}")
        endif()
    endforeach()
endfunction()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core WebSockets Mqtt)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core WebSockets Mqtt)

if(Qt${QT_VERSION_MAJOR}_VERSION LESS 5.15)
    MESSAGE(FATAL_ERROR "Minimum supported Qt version: 5.15.")
endif()

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/websocketclient.h src/websocketclient.cpp
  src/mqtthandler.h src/mqtthandler.cpp
  src/deconz2mqttconfig.h src/deconz2mqttconfig.cpp
)
target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::Mqtt)

IndicateExternalFile(${PROJECT_NAME} "res/qdeconz2mqtt.ini" "res/qdeconz2mqtt.service" "README.md")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT application)
install(FILES res/qdeconz2mqtt.ini DESTINATION ${CMAKE_INSTALL_SYSCONFDIR} COMPONENT data)
install(FILES res/qdeconz2mqtt.service DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/systemd/system/ COMPONENT data)
