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


extern void TIM1_10_UP_IRQHandler(void)
{

	if(NULL != user_handler) //TODO уйти от конкретного TIM
	{
		user_handler();
		TIM1->SR &= ~TIM_SR_UIF;
	}
}

uint8_t init_timX(timX_t *tim_stng)
{
	if(NULL == tim_stng->_TIMx){
		return 1;
	}

	tim_stng->_TIMx->CR1 |= TIM_CR1_ARPE;			///< Разрешаем автоперезагрузку счетчика

	tim_stng->_TIMx->CR1 |= (tim_stng->_mode<<4);

	tim_stng->_TIMx->PSC |= tim_stng->_prclr;

	tim_stng->_TIMx->ARR |= tim_stng->_arr;

	tim_stng->_TIMx->CNT |= tim_stng->_cnt;

	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, tim_stng->_priority);

	user_handler = tim_stng->_handler;

	tim_stng->_TIMx->DIER |= TIM_DIER_UIE;

	tim_stng->_TIMx->CR1 |= TIM_CR1_CEN;

	return 0;
}


void stop_interapt(TIM_TypeDef* _TIMx)
{

	if(NULL != _TIMx)
		_TIMx->DIER &= (~TIM_DIER_UIE);
}


uint8_t init_timX_mcrsc(timX_time_t* tim_stng)
{
	if(tim_stng->_time > 0xffff){	//TODO убрать данный костыль
		return 1;
	}

	timX_t tim = {
		._TIMx 		= tim_stng->_TIMx,
		._mode 		= tim_stng->_mode,
		._handler 	= tim_stng->_handler,
		._priority 	= tim_stng->_priority,
		._arr		= tim_stng->_time,
		._prclr		= (SYSTEM_CORE_CLOCK/ 1000000) -1

	};

	return init_timX(&tim);
}

uint8_t init_timX_ms(timX_time_t* tim_stng)
{
	if(tim_stng->_time > 0xffff){	//TODO убрать данный костыль
			return 1;
		}

		timX_t tim = {
			._TIMx 		= tim_stng->_TIMx,
			._mode 		= tim_stng->_mode,
			._handler 	= tim_stng->_handler,
			._priority 	= tim_stng->_priority,
			._arr		= tim_stng->_time,
			._prclr		= (SYSTEM_CORE_CLOCK/1000) -1

		};

		return init_timX(&tim);
}




