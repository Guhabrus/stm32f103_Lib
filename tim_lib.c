/*
 * tim_lib.c
 *
 *  Created on: Sep 22, 2023
 *      Author: Максим
 */

#include <stddef.h>
#include "stm32f103xb.h"
#include <tim_lib.h>

hadler_t user_handler = NULL;

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(NULL != user_handler && TIM1->SR & TIM_SR_UIF)
	{
		user_handler();
	}
}

uint8_t init_timX(timX_t *tim_stng)
{
	TIM1->CR1 |= TIM_CR1_ARPE;			///< Разрешаем автоперезагрузку счетчика

	TIM1->CR1 |= (tim_stng->_mode<<4);

	TIM1->PSC |= tim_stng->_prclr;

	TIM1->ARR |= tim_stng->_arr;

	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, tim_stng->_priority);

	user_handler = tim_stng->_handler;
	//TODO остальные настройки и самому считать делитель
	TIM1->DIER |= TIM_DIER_UIE;

	TIM1->CR1 |= TIM_CR1_CEN;

	return 0;
}


