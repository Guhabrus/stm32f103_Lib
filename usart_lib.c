/*
 * uart_lib.c
 *
 *  Created on: Oct 29, 2023
 *      Author: Максим
 */
#include <uart_lib.h>
#include <string.h>
#include <stm32f1xx.h>

uart_res_t uart_init(uart_init_t* _init_sntg)
{
	if(NULL == _init_sntg){
		return U_BAD_INPUT;
	}

	memcpy((uint8_t*)USART1->CR1, (uint8_t*)&_init_sntg->cr1_t, sizeof(_init_sntg->cr1_t));
	memcpy((uint8_t*)USART1->CR2, (uint8_t*)&_init_sntg->cr2_t, sizeof(_init_sntg->cr1_t));
	memcpy((uint8_t*)USART1->CR3, (uint8_t*)&_init_sntg->cr3_t, sizeof(_init_sntg->cr1_t));

	USART1->BRR |= (uint32_t)(_init_sntg->_system_clock/_init_sntg->_buad_rate*16);

	return U_SUCCES;
}



uart_res_t uart_send(uart_data* _data_pt, size_t _size)
{
	size_t runner = 0;

	while(runner<_size)
	{
		USART1->DR = *(_data_pt + _size);
		++runner;
		while(!(USART1->SR & USART_SR_TXE)){}
	}

	return U_SUCCES;
}

uart_res_t uart_read(uart_data* _data_pt, size_t _size)
{
	size_t runner = 0;

	while(runner<_size)
	{
		*(_data_pt + runner) = USART1->DR;
		++runner;
		while(!uart_empty()){}
	}

	return U_SUCCES;
}

// 0 - если данных нет, 1 - если данные есть
bool uart_empty()
{
	return (USART1->SR &= USART_SR_RXNE);
}

