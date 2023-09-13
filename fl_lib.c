/*
 * fl_lib.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Максим
 */
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "flash_lib.h"


//TODO добавить программирование опциональных байтов (Option bytes)
//TODO добавить флеш мемкопи


#define STM_FLESH_KEY1 FLASH_KEY1
#define STM_FLESH_KEY2 FLASH_KEY2

flrslt_t flesh_unlocking()
{
	if(!check_lock())
	{
		return SUCCES;
	}

	FLASH->KEYR = STM_FLESH_KEY1;
	FLASH->KEYR = STM_FLESH_KEY2;

	if(check_lock()){
		return FLESH_BLOCK;
	}

	return SUCCES;
}

void flash_locking()
{
	flash_available();
	FLASH->CR |= FLASH_CR_LOCK;
}

bool flash_available()
{
	while(FLASH->SR & FLASH_SR_BSY){};
	//TODO вернуть TIME_ERROR;
	return true;
}

flrslt_t flesh_write(uint32_t addr, uint16_t* data, size_t size)
{
	if(MAX_FLASH_ADDR < addr || MIN_FLASH_ADDR > addr || MAX_FLASH_ADDR < addr + size || addr & 1){
		return BAD_INPUT;
	}

	if(check_lock()){
		return FLESH_BLOCK;
	}

	flash_available();

	FLASH->CR |= FLASH_CR_PG;
	uint32_t end = addr + size;

	flash_available();

	for(; addr < end; addr += 2, data++)
	{
		*(uint16_t*)addr = *data;

		if(!flash_available()){
			CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
			return TIME_ERROR;
		}

		if(*data != *(uint16_t*)addr){
			CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
			return WRITE_ERROR;
		}
	}

	CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	return SUCCES;
}


flrslt_t flesh_read(uint32_t addr, uint16_t* data, size_t size)
{
	if(MAX_FLASH_ADDR < addr || MIN_FLASH_ADDR > addr || MAX_FLASH_ADDR < addr + size || addr & 1){
		return BAD_INPUT;
	}

	uint32_t end = addr + size;

	flash_available();

	for(; addr < end; addr += 2, data++)
	{
		*(uint16_t*)data = *(uint16_t*)addr;

		if(!flash_available()){
			return TIME_ERROR;
		}
	}

	return SUCCES;
}


bool check_lock()
{
	return (bool)(FLASH->CR & FLASH_CR_LOCK);
}


flrslt_t flesh_page_erase(uint8_t npage)
{
	if(MAX_NUM_PAGE < npage){
		return  BAD_INPUT;
	}

	flash_available();

	if(check_lock()){
		return FLESH_BLOCK;
	}

	SET_BIT(FLASH->CR, FLASH_CR_PER);
	FLASH->AR = MIN_FLASH_ADDR + npage * FLASH_PG_SIZE;
	SET_BIT(FLASH->CR, FLASH_CR_STRT);

	while(FLASH->CR & FLASH_CR_STRT);

	//TODO проверить, что страница стала 0xFFFF

	flash_available();

	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);

	return SUCCES;
}

flrslt_t flesh_mass_erase()
{
	flash_available();

	if(check_lock()){
		return FLESH_BLOCK;
	}

	SET_BIT(FLASH->CR, FLASH_CR_MER);
	SET_BIT(FLASH->CR, FLASH_CR_STRT);

	while(FLASH->CR & FLASH_CR_STRT);

	//TODO проверить, что все страницы стали 0xFFFF

	flash_available();

	CLEAR_BIT(FLASH->CR, FLASH_CR_MER);

	return SUCCES;
}
