#include "driver/uart.h"
#include "esp_log.h"

#define UART_NUM UART_NUM_1
#define TXD_PIN  (GPIO_NUM_17)
#define RXD_PIN  (GPIO_NUM_16)
#define BUF_SIZE (1024)

void app_main() {
    const uart_config_t uart_config = {
        .baud_rate = 9600, // verifique a velocidade do seu XBee
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Enviar um comando de teste para o XBee
    const char* cmd = "+++";
    uart_write_bytes(UART_NUM, cmd, strlen(cmd));
    vTaskDelay(pdMS_TO_TICKS(1000)); // tempo para resposta do modo de comando

    uint8_t data[128];
    int len = uart_read_bytes(UART_NUM, data, sizeof(data), 100 / portTICK_PERIOD_MS);
    if (len > 0) {
        data[len] = '\0'; // para printar como string
        ESP_LOGI("UART", "Recebido: %s", data);
    }
}
