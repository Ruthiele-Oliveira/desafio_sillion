# Desafio – Controle de Motor com STM32 + Interface RS232

## Visão Geral
Projeto de firmware e PCB para acionar um motor 5V/1A por 10s ao detectar nível lógico na entrada (12 V condicionada) e reportar status via UART (RS232). Inclui arquivos de fabricação (Gerber/Drill), BOM e Pick&Place.

Funcionamneto; Quando um sinal de 10 a 14v está ativo a entrada PA1 vai a 0 e o motor é acionado por 10s. Uma mensagem é enviada via porta serial RS232. Motor on ou Motor off.
