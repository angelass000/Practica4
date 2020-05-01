cmake_minimum_required(VERSION 3.15)
project(P4 C)

set(CMAKE_C_STANDARD 99)

include_directories(.)
include_directories(dyn)
include_directories(dyn_test)
include_directories(hal_dyn_uart)
include_directories(joystick_emu)


add_executable(P4 main.c main.h dyn/dyn_app_common.c dyn/dyn_app_common.h dyn/dyn_app_motors.c dyn/dyn_app_motors.h
        dyn/dyn_app_sensor.c dyn/dyn_app_sensor.h dyn/dyn_frames.c dyn/dyn_frames.h dyn/dyn_instr.c dyn/dyn_instr.h
        dyn_test/b_queue.c dyn_test/b_queue.h dyn_test/dyn_emu.c dyn_test/dyn_emu.h dyn_test/fake_msp.h
        hal_dyn_uart/hal_dyn_uart_emu.c hal_dyn_uart/hal_dyn_uart_emu.h
        joystick_emu/joystick.c joystick_emu/joystick.h
        )

set( CMAKE_C_FLAGS "-pthread")