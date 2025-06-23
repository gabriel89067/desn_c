void task_receive_xbee(void *arg) {
    uint8_t buf[128];
    int len = 0;

    while (1) {
        len = uart_read_bytes(UART_NUM, buf, sizeof(buf), 100 / portTICK_PERIOD_MS);
        if (len > 0 && buf[0] == 0x7E) {
            uint16_t length = (buf[1] << 8) | buf[2];

            if (length + 4 > len) {
                ESP_LOGW("XBee", "Frame incompleto");
                continue;
            }

            uint8_t frame_type = buf[3];
            if (frame_type == 0x90) {
                // Extrair endereço de origem (64 bits)
                char sender[20];
                snprintf(sender, sizeof(sender), "%02X%02X%02X%02X%02X%02X%02X%02X",
                         buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11]);

                // Pular até os dados
                uint8_t *data = &buf[15]; // dados começam após o campo de opções (1 byte)
                int data_len = length - 12; // total - 8 (src) - 2 (net) -1 (opt) -1 (type)

                char msg[100] = {0};
                memcpy(msg, data, data_len);
                msg[data_len] = '\0';

                ESP_LOGI("XBee", "Recebido de %s: %s", sender, msg);
            } else {
                ESP_LOGW("XBee", "Tipo de frame não suportado: 0x%02X", frame_type);
            }
        }
    }
}
