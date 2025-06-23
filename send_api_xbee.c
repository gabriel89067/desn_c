#include "driver/uart.h"
#include "esp_log.h"
#include <string.h>

#define UART_NUM UART_NUM_1
#define TXD_PIN  (GPIO_NUM_17)
#define RXD_PIN  (GPIO_NUM_16)
#define BUF_SIZE 256

// Endereço de 64 bits do destino (obtido no XCTU -> SL + SH)
uint8_t dest_addr[8] = { 0x00, 0x13, 0xA2, 0x00, 0x41, 0x52, 0x13, 0xA1 }; // exemplo

void uart_init() {
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void send_api_message(const char *msg) {
    uint8_t frame[128];
    int i = 0;

    frame[i++] = 0x7E; // Start delimiter

    uint16_t data_len = 14 + strlen(msg); // total data after length
    frame[i++] = (data_len >> 8) & 0xFF;
    frame[i++] = data_len & 0xFF;

    frame[i++] = 0x10; // Frame type
    frame[i++] = 0x01; // Frame ID (optional, 0x00 = no response)

    // 64-bit dest addr
    memcpy(&frame[i], dest_addr, 8); i += 8;

    frame[i++] = 0xFF; // 16-bit address unknown
    frame[i++] = 0xFE;

    frame[i++] = 0x00; // Broadcast radius
    frame[i++] = 0x00; // Options

    // Payload
    int msg_len = strlen(msg);
    memcpy(&frame[i], msg, msg_len); i += msg_len;

    // Checksum
    uint8_t sum = 0;
    for (int j = 3; j < i; j++) {
        sum += frame[j];
    }
    frame[i++] = 0xFF - sum;

    uart_write_bytes(UART_NUM, (const char *)frame, i);
    ESP_LOGI("XBee", "Mensagem enviada via API: %s", msg);
}
