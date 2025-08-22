/*
 * init_usart1.c
 *
 * Created on: Aug 22, 2025
 * Author: Eng: Ruthiele das Graças de Oliveira
 *
 * @brief Módulo de inicialização para a interface USART1.
 * Configura os pinos GPIO, os parâmetros da USART e o DMA para transmissão.
 */
#include "main.h"

/**
 * @brief Configura e inicializa a interface USART1.
 * @param baudRate A taxa de transmissão desejada para a comunicação serial (ex: 115200).
 * @retval None
 */
void init_usart1(uint32_t baudRate){
	// Habilita o clock para o DMA1, USART1 e GPIOA.
	RCC->AHBENR  |= RCC_AHBENR_DMA1EN;
	RCC->APB2ENR |= (RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN);

	// Configuração dos pinos GPIOA
	// PA9 (TX) como saída de função alternativa (Alternate Function Push-pull).
	// PA10 (RX) como entrada flutuante (Input floating).
	GPIOA->CRH  &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
	GPIOA->CRH  |= (GPIO_CRH_MODE9 | GPIO_CRH_CNF9_1 |  GPIO_CRH_CNF10_0);

	// Bloqueia a configuração dos pinos PA9 e PA10 para evitar alterações acidentais.
	GPIOA->LCKR |= (GPIO_PIN_9 |GPIO_PIN_10);

	// Reseta os registradores de controle da USART1 para um estado padrão conhecido.
	USART1->CR1 = 0x00;
	USART1->CR2 = 0x00;
	USART1->CR3 = 0x00;

	// Habilita o transmissor (TE), o receptor (RE) e a interrupção de recebimento (RXNEIE).
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_RE);

	// Habilita a solicitação de DMA para o transmissor (DMAT).
	USART1->CR3 = (USART_CR3_DMAT);

	// Calcula os valores da mantissa e da fração para o registrador de Baud Rate (BRR).
	// A fórmula é baseada no clock do barramento APB2 (fCK), que é 72MHz.
	// USARTDIV = fCK / (baudRate * 16)
	uint16_t mantissa = (72000000/(baudRate*16));
	uint16_t fraction = ((72000000/(baudRate*16.00)) - mantissa)*16;

	// Define o registrador de baud rate.
	USART1->BRR = ((mantissa<<4)|fraction);

	// Configura o prescaler do Guard Time (não utilizado neste contexto).
	USART1->GTPR = 0x01;

	// Pequeno atraso para garantir a estabilização antes de habilitar a USART.
	HAL_Delay(1);

	// Habilita a USART1 setando o bit UE (USART Enable) no CR1.
	USART1->CR1 |= USART_CR1_UE;

	// Habilita as interrupções globais no NVIC para a USART1 e para o canal 4 do DMA1.
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}
