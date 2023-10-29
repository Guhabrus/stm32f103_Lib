/*
 * uart_lib.h
 *
 *  Created on: Oct 29, 2023
 *      Author: Максим
 */

#ifndef INC_UART_LIB_H_
#define INC_UART_LIB_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#pragma pack(push,1)

typedef uint8_t uart_data;

typedef enum
{
	U_SUCCES,				///< Процедура успешная
	U_TIME_ERROR,			///< Ошибка таймаута
	U_UART_BLOCK,		///< Доступ к uart заблокирован
	U_BAD_INPUT,
	U_UNKNOWN

}uart_res_t;

typedef struct
{
	struct
	{
		uint16_t ue:1;
		uint16_t word_len:1;
		uint16_t wake_m:1;
		uint16_t pce:1;
		uint16_t ps:1;
		uint16_t peie:1;
		uint16_t txeie:1;
		uint16_t tcie:1;
		uint16_t rxneie:1;
		uint16_t idleie:1;
		uint16_t te:1;
		uint16_t re:1;
		uint16_t rwu:1;
		uint16_t sbk:1;
		uint16_t :2;
	}cr1_t;

	struct
	{
		uint16_t linen:1;
		uint16_t stop:1;
		uint16_t clken:1;
		uint16_t cpol:1;
		uint16_t cpha:1;
		uint16_t lbcl:1;
		uint16_t lbdie:1;
		uint16_t lbdl:1;
		uint16_t add:3;
		uint16_t :5;
	}cr2_t;

	struct
	{
		uint16_t ctsie:1;
		uint16_t ctse:1;
		uint16_t rtse:1;
		uint16_t dmat:1;
		uint16_t dmar:1;
		uint16_t scen:1;
		uint16_t nack:1;
		uint16_t hdsel:1;
		uint16_t irpl:1;
		uint16_t iren:1;
		uint16_t eie:1;

	}cr3_t;

	uint32_t speed;

}*uart_init_pt, uart_init_t;

//TODO
typedef struct
{
	uint16_t error;
}*uart_error_pt,uart_error_t ;

#pragma pack(pop)

uart_res_t uart_init(uart_init_t* _init_sntg);

uart_res_t uart_send(uart_data* _data_pt, size_t _size);

uart_res_t uart_read(uart_data* _data_pt, size_t _size);

// 0 - если данных нет, 1 - если данные есть
bool uart_empty();

void uart_error(uart_error_t* uart_error);

#endif /* INC_UART_LIB_H_ */
