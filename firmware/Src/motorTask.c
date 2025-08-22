/*
 * motorTask.c
 *
 * Created on: Aug 22, 2025
 * Author: Eng: Ruthiele das Graças de Oliveira
 * * @brief Implementa a lógica de controle do motor como uma máquina de estados
 * e a configuração do temporizador (TIM2) para o desligamento automático.
 */
#include "main.h"
#include "transmit_usart.h"

// Variáveis globais para controlar a máquina de estados.
uint8_t stepTask=0;     // Variável que armazena o estado atual da máquina de estados.

// Mensagens a serem transmitidas via USART.
char motorOn[] = "Motor on\n";
char motorOff[] = "Motor off\n";

/**
 * @brief Máquina de estados que gerencia a operação do motor.
 * @retval O estado atual da máquina de estados.
 */
uint8_t motorTask(void)
{
	switch(stepTask)
	{
		  // ESTADO 0: Ocioso, aguardando o acionamento do botão.
		  case 0:
			  // Verifica se o pino PA1 (botão) está em nível baixo (pressionado).
			  if(!(GPIOA->IDR & GPIO_PIN_1)){
				  TIM2->CNT = 0;          // Zera o contador do TIM2.
				  TIM2->CR1 |= TIM_CR1_CEN; // Inicia a contagem do TIM2.
				  stepTask=1;               // Avança para o próximo estado.
				  GPIOA->BSRR = GPIO_PIN_2; // Liga o motor (seta o pino PA2).
			  }
			  break;

		  // ESTADO 1: Envia a mensagem "Motor on".
		  case 1:
			     // Chama a função de transmissão não bloqueante.
			     // A variável stepTask será incrementada para 2 quando a transmissão for concluída.
			     stepTask += transmit_usart1(motorOn, 9);
			  break;

		  // ESTADO 2: Aguardando o timer de 10 segundos expirar.
		  // A transição para o próximo estado é feita na interrupção do TIM2.
		  case 2:
			  break;

		  // ESTADO 3: Envia a mensagem "Motor off".
		  case 3:
			  	  // Chama a função de transmissão não bloqueante.
			  	  // stepTask será incrementado para 4 quando a transmissão for concluída.
			  	  stepTask += transmit_usart1(motorOff, 10);
			  break;

		  // ESTADO PADRÃO: Reseta a máquina de estados para o estado inicial.
		  default:
			  stepTask = 0;
			  break;
	  }
	return stepTask;
}

/**
 * @brief Inicializa o TIM2 para gerar uma interrupção após 10 segundos.
 * @retval None
 */
void init_motorTimer(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Habilita o clock para o TIM2.

	// Configura o prescaler e o valor de auto-reload para atingir 10 segundos.
	// O clock do TIM2 é 72MHz.
	// Frequência de atualização = 72,000,000 / ((PSC+1) * (ARR+1))
	// Frequência de atualização = 72,000,000 / (11250 * 64000) = 0.1 Hz => Período = 10 segundos.
	TIM2->PSC = 11249;
	TIM2->ARR = 63999;

	TIM2->CNT = 0; // Zera o contador.
	TIM2->DIER |= TIM_DIER_UIE; // Habilita a interrupção de atualização (update).
	TIM2->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM2_IRQn); // Habilita a interrupção do TIM2 no NVIC.
}

/**
 * @brief Rotina de tratamento de interrupção (ISR) para o TIM2.
 * Executada quando o contador de 10 segundos expira.
 */
void TIM2_IRQHandler(void)
{
    // Verifica se a flag de interrupção de atualização (update) está setada.
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF; // Limpa a flag de interrupção.

        // Ações a serem executadas após 10 segundos:
        GPIOA->BRR = GPIO_PIN_2;    // Desliga o motor (reseta o pino PA2).
        TIM2->CR1 &= ~TIM_CR1_CEN;  // Para o contador do TIM2.
        stepTask = 3;               // Avança a máquina de estados para o passo de desligamento.
    }
}
