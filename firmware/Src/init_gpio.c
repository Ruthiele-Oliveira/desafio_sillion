/*
 * init_GPIO.c
 *
 * Created on: Aug 22, 2025
 * Author: Eng: Ruthiele das Graças de Oliveira
 *
 * @brief Módulo de inicialização para os pinos GPIO específicos da aplicação.
 */
#include "main.h"

/**
 * @brief Configura os pinos GPIO necessários para a aplicação.
 * @retval None
 */
void init_gpio(void)
{
	// Habilita o clock para a porta GPIOA no barramento APB2.
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	// Configura o pino PA1 como entrada flutuante (Input Floating).
	// Este pino será usado para ler o estado de um botão.
	GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1); // Limpa as configurações atuais do pino 1.
	GPIOA->CRL |= GPIO_CRL_CNF1_0;                  // Define como entrada flutuante.

	// Configura o pino PA2 como saída push-pull de 50 MHz.
	// Este pino será usado para controlar o motor.
	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); // Limpa as configurações atuais do pino 2.
	GPIOA->CRL |= GPIO_CRL_MODE2;                   // Define como saída de 50 MHz.
}
