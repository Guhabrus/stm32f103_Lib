/*
 * fl_lib.h
 *
 *  Created on: Sep 11, 2023
 *      Author: Максим
 */

#ifndef INC_FL_LIB_H_
#define INC_FL_LIB_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define FLASH_OFFSET		0x08000000U
#define MAX_NUM_PAGE		63
#define FLASH_PG_SIZE		1024
#define MIN_FLASH_ADDR		0x08000000
#define MAX_FLASH_ADDR		(MIN_FLASH_ADDR + (MAX_NUM_PAGE + 1) * FLASH_PG_SIZE - 1)
typedef enum
{
	SUCCES,				///< Процедура успешная
	TIME_ERROR,			///< Ошибка таймаута
	FLESH_BLOCK,		///< Доступ к flesh заблокирован
	BAD_INPUT,
	UNKNOWN				///< Неизвестная ошибка
}flrslt_t;

/**
 * @brief Разблокировка работы с flesh памятью
 * 
 * @return flrslt_t - результат выполениея 
 */
flrslt_t flesh_unlocking();


/**
 * @brief Сброс flesh памяти
 * 
 * @return flrslt_t результат выполения
 */
flrslt_t flesh_page_erase(uint8_t npage);

flrslt_t flesh_write(uint32_t addr, uint16_t* data, size_t size);

flrslt_t flesh_read(uint32_t addr, uint16_t* data, size_t size);


/**
 * @brief Доступность работы с flesh памятью
 * 
 * @return true - в случае доступности
 * @return false - в случае если flesh недоступен
 */
bool flash_available();

bool check_lock();




#endif /* INC_FL_LIB_H_ */
