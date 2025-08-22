/*
 * transmit_usart.c
 *
 * Created on: Aug 22, 2025
 * Author: Eng: Ruthiele das Graças de Oliveira
 *
 * @brief Módulo para transmissão de dados via USART1 utilizando DMA.
 * Permite o envio de dados de forma não bloqueante.
 */


#include "main.h"

// Variáveis de controle de transmissão DMA.
uint8_t end_usart_tx = 0; // Flag que indica a conclusão da transferência DMA (setada pela ISR).
uint8_t ax_usart1 = 1;    // Flag que indica se o canal DMA está disponível para uma nova transmissão.

/**
 * @brief Inicia uma transmissão de dados via USART1 usando o DMA (Canal 4).
 * @param dUsart_tx Ponteiro para o buffer de dados a ser transmitido.
 * @param size O número de bytes a serem transmitidos.
 * @retval 1 se a transmissão anterior foi concluída e uma nova pode ser iniciada.
 * @retval 0 se a transmissão ainda está em andamento.
 */
uint8_t transmit_usart1(char * dUsart_tx, uint8_t size)
{
	// Verifica se o canal DMA está livre para iniciar uma nova transmissão.
	if(ax_usart1)
	{
		ax_usart1 = 0;      // Marca o canal DMA como ocupado.
		end_usart_tx = 0;   // Reseta a flag de conclusão da transferência.

		// Desabilita o canal DMA para permitir a configuração.
		DMA1_Channel4->CCR = 0x00;

		// Define o endereço da memória (buffer de origem).
		DMA1_Channel4->CMAR = (uint32_t)dUsart_tx;

		// Define o endereço do periférico (registrador de dados da USART1).
		DMA1_Channel4->CPAR = (uint32_t)&USART1->DR;

		// Define o número de bytes a serem transferidos.
		DMA1_Channel4->CNDTR = size;

		// Configura o canal DMA:
		// - MINC: Modo de incremento de memória habilitado.
		// - DIR: Direção da transferência (memória para periférico).
		// - TCIE: Interrupção de conclusão de transferência habilitada.
		DMA1_Channel4->CCR |= (DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE);

		// Limpa todas as flags de interrupção do canal 4.
		DMA1->IFCR |= (DMA_IFCR_CTEIF4 | DMA_IFCR_CHTIF4 | DMA_IFCR_CTCIF4 | DMA_IFCR_CGIF4);

		// Habilita o canal DMA para iniciar a transferência.
		if((DMA1_Channel4->CCR & DMA_CCR_EN) != DMA_CCR_EN)
		    DMA1_Channel4->CCR |= DMA_CCR_EN;//enable DMA1

	}

	// Verifica se a transferência DMA foi concluída E se a USART terminou de transmitir o último byte.
	if(end_usart_tx)
	{
		// A flag TC (Transmission Complete) da USART indica que o shift register está vazio.
		if(((USART1->SR & USART_SR_TC) == USART_SR_TC))
		{
			ax_usart1 = 1;      // Libera o canal DMA para a próxima transmissão.
			end_usart_tx = 0;   // Reseta a flag de conclusão.
			return 1;           // Retorna 1 para sinalizar o fim do processo.
		}
	}

	// Retorna 0 se a transmissão ainda não foi totalmente concluída.
	return 0;
}


/**
 * @brief Rotina de tratamento de interrupção (ISR) para o Canal 4 do DMA1.
 * Acionada quando a transferência de dados do buffer de memória para a USART foi concluída.
 */
void DMA1_Channel4_IRQHandler(void){
	// Limpa todas as flags de interrupção do canal para evitar reentrância.
	DMA1->IFCR |= (DMA_IFCR_CTEIF4 | DMA_IFCR_CHTIF4 | DMA_IFCR_CTCIF4 | DMA_IFCR_CGIF4);

	// Sinaliza para a função principal que a transferência DMA terminou.
	end_usart_tx=1;
}
