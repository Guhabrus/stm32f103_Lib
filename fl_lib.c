/*
 * fl_lib.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Максим
 */
#include "stm32f103xb.h"

#include "fl_lib.h"

#define STM_FLESH_KEY1 FLASH_KEY1
#define STM_FLESH_KEY2 FLASH_KEY2

flrslt_t flesh_unlocking()
{
	FLASH->KEYR = STM_FLESH_KEY1;
	FLASH->KEYR = STM_FLESH_KEY2;

	if(check_lock()){
		return FLESH_BLOCK;
	}

	return SUCCES;
}

bool flash_available()
{
	while(FLASH->SR & FLASH_SR_BSY){};
	//TODO вернуть TIME_ERROR;
	return true;
}

flrslt_t flesh_write(uint32_t addr, uint16_t* data, size_t size)
{
	if(MAX_FLASH_ADDR < (FLASH_OFFSET + addr) || MIN_FLASH_ADDR > (FLASH_OFFSET + addr) || addr & 1){
		return BAD_INPUT;
	}

	if(!flash_available()){
		return FLESH_BLOCK;
	}

	if(check_lock()){
		return FLESH_BLOCK;
	}

	FLASH->CR |= FLASH_CR_PG;
	uint32_t end = addr + size;

	for(; addr < end; addr += 2, data++)
	{
		*((uint16_t*)(FLASH_OFFSET + addr)) = *data;

		if(check_lock()){
			return TIME_ERROR;
		}
	}

	return SUCCES;
}


flrslt_t flesh_read(uint32_t addr, uint16_t* data, size_t size)
{
	if(MAX_FLASH_ADDR < (FLASH_OFFSET + addr) || MIN_FLASH_ADDR > (FLASH_OFFSET + addr)|| addr & 1){
		return BAD_INPUT;
	}

	if(!flash_available()){
		return FLESH_BLOCK;
	}

	uint32_t end = addr + size;

	for(; addr < end; addr += 2, data++)
	{
		*(uint16_t*)data = *((uint16_t*)(FLASH_OFFSET + addr));

		if(check_lock()){
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

	if(!flash_available()){
		return FLESH_BLOCK;
	}

	if(check_lock()){
		return FLESH_BLOCK;
	}

	FLASH->CR |= FLASH_CR_PER;

	FLASH->AR = MIN_FLASH_ADDR + npage * FLASH_PG_SIZE;

	FLASH->CR |= FLASH_CR_STRT;

	if(!flash_available()){
		return FLESH_BLOCK;
	}

	//TODO проверить, что страница стала 0xFFFF

	return SUCCES;
}
