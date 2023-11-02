/*
 * uart_lib.h
 *
 *  Created on: Oct 29, 2023
 *      Author: Максим
 */

#ifndef INC_USART_LIB_H_
#define INC_USART_LIB_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#pragma pack(push,1)

typedef uint8_t usart_data;

typedef enum
{
	U_SUCCES,				///< Процедура успешная
	U_TIME_ERROR,			///< Ошибка таймаута
	U_USART_BLOCK,		///< Доступ к usart заблокирован
	U_BAD_INPUT,
	U_UNKNOWN

}usart_res_t;

typedef struct
{
	struct
	{
		uint32_t sbk:1;
		uint32_t rwu:1;
		uint32_t re:1;
		uint32_t te:1;
		uint32_t idleie:1;
		uint32_t rxneie:1;
		uint32_t tcie:1;
		uint32_t txeie:1;
		uint32_t peie:1;
		uint32_t ps:1;
		uint32_t pce:1;
		uint32_t wake_m:1;
		uint32_t word_len:1;
		uint32_t ue:1;
		uint32_t :18;
	}cr1_t;

	struct
	{
		uint32_t add:4;
		uint32_t :1;
		uint32_t lbdl:1;
		uint32_t lbdie:1;
		uint32_t :1;
		uint32_t lbcl:1;
		uint32_t cpha:1;
		uint32_t cpol:1;
		uint32_t clken:1;
		uint32_t stop:2;
		uint32_t linen:1;
		uint32_t :17;
	}cr2_t;

	struct
	{
		uint32_t eie:1;
		uint32_t iren:1;
		uint32_t irpl:1;
		uint32_t hdsel:1;
		uint32_t nack:1;
		uint32_t scen:1;
		uint32_t dmar:1;
		uint32_t dmat:1;
		uint32_t rtse:1;
		uint32_t ctse:1;
		uint32_t ctsie:1;
		uint32_t :21;
	}cr3_t;

	uint32_t _buad_rate;
	uint32_t _system_clock;

}*usart_init_pt, usart_init_t;

//TODO
typedef struct
{
	uint16_t error;
}*usart_error_pt,usart_error_t ;

#pragma pack(pop)

usart_res_t usart_init(usart_init_t* _init_sntg);

usart_res_t usart_send(usart_data* _data_pt, size_t _size);

usart_res_t usart_read(usart_data* _data_pt, size_t _size);

// 0 - если данных нет, 1 - если данные есть
bool usart_empty();

void usart_error(usart_error_t* usart_error);

#endif /* INC_USART_LIB_H_ */
