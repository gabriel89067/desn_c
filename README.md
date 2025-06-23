✅ 1. Estrutura de um Frame API 0x10 (Transmit Request)
Campo	Tamanho	Valor / Descrição
0x7E (start)	1 byte	Delimitador fixo
Length (MSB, LSB)	2 bytes	Tamanho do frame a partir do campo "Frame Type"
Frame Type	1 byte	0x10 = Transmit Request
Frame ID	1 byte	ID do frame (0 = sem resposta)
64-bit Dest. Addr	8 bytes	SH+SL do XBee de destino
16-bit Addr	2 bytes	Pode ser 0xFFFE se desconhecido
Broadcast Radius	1 byte	0 = máximo alcance
Options	1 byte	0 = padrão
RF Data	N bytes	Mensagem ("Oi 2, sou o 1")
Checksum	1 byte	0xFF - (soma de todos bytes do frame data)
