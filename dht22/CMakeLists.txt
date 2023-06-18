include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#pico_sdk_init()



add_library(dht22
    dht22.h
    dht22.c)


target_link_libraries(dht22 pico_stdlib)
    
target_include_directories(dht22 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")