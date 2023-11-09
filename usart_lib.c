/*
 * usart_lib.c
 *
 *  Created on: Oct 29, 2023
 *      Author: Максим
 */
#include <usart_lib.h>
#include <string.h>
#include <stm32f1xx.h>

usart_res_t usart_init(usart_init_t* _init_sntg)
{
	if(NULL == _init_sntg){
		return U_BAD_INPUT;
	}
	USART1->CR1 = 0;
	USART1->CR2 = 0;
	USART1->CR3 = 0;

	USART1->CR1 |= *((uint32_t*)&_init_sntg->cr1_t);
	USART1->CR2 |= *((uint32_t*)&_init_sntg->cr2_t);
	USART1->CR3 |= *((uint32_t*)&_init_sntg->cr3_t);

	USART1->BRR |= (uint32_t)(_init_sntg->_system_clock/_init_sntg->_buad_rate);

	return U_SUCCES;
}



usart_res_t usart_send(usart_data* _data_pt, size_t _size)
{
	size_t runner = 0;

	while(runner<_size)
	{
		USART1->DR = *(_data_pt + runner);
		++runner;
		while(!(USART1->SR & USART_SR_TXE)){}

	}

	return U_SUCCES;
}

usart_res_t usart_read(usart_data* _data_pt, size_t _size)
{
	size_t runner = 0;

	while(runner<_size)
	{
		while(!usart_empty()){}

		*(_data_pt + runner) = USART1->DR;
		++runner;
	}

	return U_SUCCES;
}

// 0 - если данных нет, 1 - если данные есть
bool usart_empty()
{
	return (USART1->SR &= USART_SR_RXNE);
}

