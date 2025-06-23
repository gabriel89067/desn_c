#define MY_ID 1 // Altere para 2 e 3 nos outros ESPs

void send_message(uint8_t target_id, const char *msg) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "MSG|from=%d|to=%d|text=%s", MY_ID, target_id, msg);
    uart_write_bytes(UART_NUM_1, buffer, strlen(buffer));
}

void task_receive_uart(void *arg) {
    uint8_t data[128];
    while (true) {
        int len = uart_read_bytes(UART_NUM_1, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
        if (len > 0) {
            data[len] = 0;
            // simples parsing
            int from = -1, to = -1;
            char text[64] = {0};
            if (sscanf((char*)data, "MSG|from=%d|to=%d|text=%63[^\n]", &from, &to, text) == 3) {
                if (to == MY_ID || to == 0) { // 0 = broadcast
                    printf("Mensagem recebida de %d: %s\n", from, text);
                }
            }
        }
    }
}

void app_main() {
    uart_init(); // configure UART como vimos antes
    xTaskCreate(task_receive_uart, "uart_rx", 4096, NULL, 10, NULL);

    vTaskDelay(2000 / portTICK_PERIOD_MS); // espera inicial

    while (1) {
        send_message(2, "Oi 2, sou o 1");
        send_message(3, "Oi 3, sou o 1");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
