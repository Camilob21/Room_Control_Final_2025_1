#include "command_parser.h"
#include <string.h>
#include <stdio.h>
#include "room_control.h"
#include "main.h"

#define COMMAND_BUFFER_SIZE 64

static char esp01_buffer[COMMAND_BUFFER_SIZE];
static uint8_t esp01_index = 0;

static char debug_buffer[COMMAND_BUFFER_SIZE];
static uint8_t debug_index = 0;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern room_control_t room_system;

static void process_command(const char *cmd, UART_HandleTypeDef *source_uart);
static void send_response(UART_HandleTypeDef *uart, const char *msg);

// Función principal del parser (para ESP-01)
void command_parser_process_esp01(uint8_t byte) {
    if (byte == '\n' || byte == '\r') {
        if (esp01_index > 0) {
            esp01_buffer[esp01_index] = '\0';
            process_command(esp01_buffer, &huart3);
            esp01_index = 0;
        }
    } else if (esp01_index < COMMAND_BUFFER_SIZE - 1) {
        esp01_buffer[esp01_index++] = byte;
    }
}

// Función principal del parser (para debug UART)
void command_parser_process_debug(uint8_t byte) {
    if (byte == '\n' || byte == '\r') {
        if (debug_index > 0) {
            debug_buffer[debug_index] = '\0';
            process_command(debug_buffer, &huart2);
            debug_index = 0;
        }
    } else if (debug_index < COMMAND_BUFFER_SIZE - 1) {
        debug_buffer[debug_index++] = byte;
    }
}

// Envía una línea terminada en \r\n
static void send_response(UART_HandleTypeDef *uart, const char *msg) {
    HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    HAL_UART_Transmit(uart, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
}

// Parser real de comandos
static void process_command(const char *cmd, UART_HandleTypeDef *source_uart) {
    if (strncmp(cmd, "GET_TEMP", 8) == 0) {
        int temp = (int)(room_control_get_temperature(&room_system) + 0.5f);
        char msg[32];
        snprintf(msg, sizeof(msg), "TEMP: %d C", temp);
        send_response(source_uart, msg);

    } else if (strncmp(cmd, "GET_STATUS", 10) == 0) {
        room_state_t state = room_control_get_state(&room_system);
        fan_level_t fan = room_control_get_fan_level(&room_system);
        char msg[64];
        snprintf(msg, sizeof(msg), "STATUS: %s, FAN=%d",
                 state == ROOM_STATE_LOCKED ? "LOCKED" :
                 state == ROOM_STATE_UNLOCKED ? "UNLOCKED" :
                 state == ROOM_STATE_UNLOCKED ? "ACCESS_GRANTED" :
                 state == ROOM_STATE_ACCESS_DENIED ? "ACCESS_DENIED" : "UNKNOWN",
                 fan);
        send_response(source_uart, msg);

    } else if (strncmp(cmd, "SET_PASS:", 9) == 0) {
        const char *new_pass = cmd + 9;
        if (strlen(new_pass) == 4) {
            room_control_change_password(&room_system, new_pass);
            send_response(source_uart, "Password changed");
        } else {
            send_response(source_uart, "Invalid password format");
        }

    } else if (strncmp(cmd, "FORCE_FAN:", 10) == 0) {
        int level = cmd[10] - '0';
        if (level >= 0 && level <= 3) {
            room_control_force_fan_level(&room_system, (fan_level_t)level);
            send_response(source_uart, "Fan level forced");
        } else {
            send_response(source_uart, "Invalid fan level");
        }

    } else {
        send_response(source_uart, "Unknown command");
    }
}
