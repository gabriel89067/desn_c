---

## Objetivo

**"Oi 2, sou o 1"** → via API frame para o destino correto.

---

## Visão Geral do Modo API

O **XBee API mode** usa frames binários com estrutura definida:

```
Start Delimiter (0x7E)
Length MSB
Length LSB
Frame Data (...)
Checksum
```

O frame para envio de dados é o **Transmit Request (API Frame Type = 0x10)**.

---

## ✅ 1. Estrutura de um Frame API 0x10 (Transmit Request)

| Campo             | Tamanho | Valor / Descrição                               |
| ----------------- | ------- | ----------------------------------------------- |
| 0x7E (start)      | 1 byte  | Delimitador fixo                                |
| Length (MSB, LSB) | 2 bytes | Tamanho do frame a partir do campo "Frame Type" |
| Frame Type        | 1 byte  | `0x10` = Transmit Request                       |
| Frame ID          | 1 byte  | ID do frame (0 = sem resposta)                  |
| 64-bit Dest. Addr | 8 bytes | SH+SL do XBee de destino                        |
| 16-bit Addr       | 2 bytes | Pode ser `0xFFFE` se desconhecido               |
| Broadcast Radius  | 1 byte  | 0 = máximo alcance                              |
| Options           | 1 byte  | 0 = padrão                                      |
| RF Data           | N bytes | Mensagem ("Oi 2, sou o 1")                      |
| Checksum          | 1 byte  | 0xFF - (soma de todos bytes do frame data)      |

---


---

##3. Recebendo Frames (API RX – Tipo 0x90)

Você precisa decodificar frames **0x90** para receber mensagens.

---

## 4. Organização para os 3 ESPs

Cada ESP deve ter:

* Seu próprio endereço 64-bit (configurado no XCTU);
* Um código `#define MY_ID` e os endereços 64-bit dos outros 2;
* Mensagens endereçadas corretamente usando a função `send_api_message()` com o destino correto.

---

**receptor de frames API 0x90**.
---

## ✅ Estrutura do Frame API 0x90 (Receive Packet)

| Campo                  | Tamanho | Descrição                                          |
| ---------------------- | ------- | -------------------------------------------------- |
| Start Delimiter        | 1 byte  | `0x7E`                                             |
| Length MSB + LSB       | 2 bytes | Comprimento do frame (exclui início e checksum)    |
| Frame Type             | 1 byte  | `0x90`                                             |
| 64-bit Source Address  | 8 bytes | SH + SL de quem enviou                             |
| 16-bit Network Address | 2 bytes | Pode ser ignorado                                  |
| Receive Options        | 1 byte  | Pode ser ignorado                                  |
| RF Data                | N bytes | A mensagem em si                                   |
| Checksum               | 1 byte  | `0xFF - (soma dos bytes entre Frame Type e final)` |

---

