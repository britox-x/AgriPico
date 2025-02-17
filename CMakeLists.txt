cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializa o SDK da Raspberry Pi Pico
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()

set(PICO_BOARD pico CACHE STRING "Board type")

# Importa o SDK da Raspberry Pi Pico
include(pico_sdk_import.cmake)

project(buzzer C CXX ASM)

# Inicializa o SDK da Raspberry Pi Pico
pico_sdk_init()

# Adiciona executável
add_executable(buzzer buzzer.c)

pico_set_program_name(buzzer "buzzer")
pico_set_program_version(buzzer "0.1")

# Habilita saída sobre USB
pico_enable_stdio_uart(buzzer 0)
pico_enable_stdio_usb(buzzer 1)

# Adiciona bibliotecas necessárias
target_link_libraries(buzzer
        hardware_pwm
        pico_stdlib)

# Adiciona diretórios de inclusão
target_include_directories(buzzer PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Adiciona saídas extras
pico_add_extra_outputs(buzzer)