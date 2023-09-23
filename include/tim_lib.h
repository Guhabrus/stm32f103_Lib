/*
 * tim_lib.h
 *
 *  Created on: Sep 22, 2023
 *      Author: Максим
 */

#ifndef INC_TIM_LIB_H_
#define INC_TIM_LIB_H_

#include <stdint.h>
/**
 @brief Тип пользовательского обработчика прерывания по таймеру
 */
typedef void (*hadler_t)(void);

/**
 @brief Стурктура иницилизации таймера TIM1-8
 */
typedef struct
{
	TIM_TypeDef* _TIMx;			///< Номер таймера
	uint8_t _mode;				///< Режим работы таймера (0 - down\ 1 - up\ 2 - up-down)
	uint16_t _prclr;			///< значение делителя
	uint16_t _cnt;				///<
	uint16_t _arr;				///<
	uint8_t _priority;			///<
	hadler_t _handler;			///< Пользовательскаяя функция обработчик прерывания от таймера
}timX_t;

/**
 *  @brief Инициалилзация таймера
 *  @param [in] handler - указатель на структуру (hadler_t) с настройками таймера
 */
uint8_t init_timX(timX_t *tim_stng);

/**
 *  @brief Установка разового прерывания через определенное время
 *  Устанавливает в том случае если не таймер не запущен //TODO Сейчас сделаю только для TIM1 дальше надо подумать
 *  @param [in] time - относительное время через которое сработает прерывание
 */
uint8_t set_interapt(uint32_t time);

/**
 *  @brief Остановить работу таймера
 *  @param [in] timX - Номер TIM
 */
void stop_interapt(TIM_TypeDef* _TIMx);



#endif /* INC_TIM_LIB_H_ */
