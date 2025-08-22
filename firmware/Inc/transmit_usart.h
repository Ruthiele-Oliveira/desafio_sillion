/*
 * transmit_usart.h
 *
 * Created on: Aug 22, 2025
 * Author: ruthi
 *
 * @brief Arquivo de cabeçalho para o módulo de transmissão USART via DMA.
 * Declara a função para envio de dados de forma não bloqueante.
 */

#ifndef INC_TRANSMIT_USART1_H_
#define INC_TRANSMIT_USART1_H_

/**
 * @brief Inicia uma transmissão de dados via USART1 usando o DMA.
 * Esta é uma função não bloqueante.
 * @param dUsart_tx Ponteiro para o buffer de dados a ser transmitido.
 * @param size O número de bytes a serem transmitidos.
 * @retval 1 se a transmissão anterior foi concluída e o canal está pronto.
 * @retval 0 se uma transmissão ainda está em andamento.
 */
uint8_t transmit_usart1(char * dUsart_tx, uint8_t size);

#endif /* INC_TRANSMIT_USART1_H_ */
