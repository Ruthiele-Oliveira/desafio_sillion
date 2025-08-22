/*
 * motorTask.h
 *
 * Created on: Aug 22, 2025
 * Author: ruthi
 *
 * @brief Arquivo de cabeçalho para a tarefa de controle do motor.
 * Declara as funções que compõem a lógica de controle do motor.
 */

#ifndef INC_MOTORTASK_H_
#define INC_MOTORTASK_H_

/**
 * @brief Executa a máquina de estados que gerencia a operação do motor.
 * Esta função deve ser chamada repetidamente no loop principal.
 * @retval uint8_t O estado atual da máquina de estados.
 */
uint8_t motorTask(void);

/**
 * @brief Inicializa o periférico TIM2 para gerar um atraso para o desligamento do motor.
 * @retval None
 */
void init_motorTimer(void);

#endif /* INC_MOTORTASK_H_ */
