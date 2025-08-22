/*
 * init_usart1.h
 *
 * Created on: Aug 22, 2025
 * Author: Eng: Ruthiele das Graças de Oliveira
 *
 * @brief Arquivo de cabeçalho para o módulo de inicialização da USART1.
 * Declara a função de configuração da interface USART1.
 */

#ifndef INC_INIT_USART1_H_
#define INC_INIT_USART1_H_

/**
 * @brief Configura e inicializa a interface USART1.
 * @param baudRate A taxa de transmissão desejada para a comunicação serial (ex: 115200).
 * @retval None
 */
void init_usart1(uint32_t baudRate);


#endif /* INC_INIT_USART1_H_ */
