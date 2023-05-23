cmake_minimum_required(VERSION 3.16)

project( SystemLib )

set(SYSTEM_LIB_SOURCES
        "System.cpp"
        "vrEmuLcd.cpp"
        "PortBus.cpp"
        "W65C02.cpp"
        "W65C22.cpp"
        "EEPROM.cpp"
        "RAM.cpp"
        "Bus.cpp"
        "Cycles.cpp"
        "Lights.cpp"
        "LCD.cpp")

add_library( SystemLib ${SYSTEM_LIB_SOURCES})

target_include_directories ( SystemLib PUBLIC ${PROJECT_SOURCE_DIR})
