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

#define MAX_NUM_PAGE		63U
#define FLASH_PG_SIZE		1024U
#define MIN_FLASH_ADDR		0x08000000U
#define MAX_FLASH_ADDR		(MIN_FLASH_ADDR + (MAX_NUM_PAGE + 1U) * FLASH_PG_SIZE - 1U)
typedef enum
{
	SUCCES,				///< Процедура успешная
	TIME_ERROR,			///< Ошибка таймаута
	FLESH_BLOCK,		///< Доступ к flash заблокирован
	BAD_INPUT,
	WRITE_ERROR,
	UNKNOWN				///< Неизвестная ошибка
}flrslt_t;

/**
 * @brief Разблокировка работы с flash памятью
 *
 * @return flrslt_t - результат выполениея
 */
flrslt_t flash_unlocking();

void flash_locking();


/**
 * @brief Сброс flash памяти
 *
 * @return flrslt_t результат выполения
 */
flrslt_t flash_page_erase(uint8_t npage);

flrslt_t flash_mass_erase();

flrslt_t flash_write(uint32_t addr, uint16_t* data, size_t size);

flrslt_t flash_read(uint32_t addr, uint16_t* data, size_t size);


/**
 * @brief Доступность работы с flash памятью
 *
 * @return true - в случае доступности
 * @return false - в случае если flash недоступен
 */
bool flash_available();

bool check_lock();




#endif /* INC_FL_LIB_H_ */
