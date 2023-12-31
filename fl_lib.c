/*
 * fl_lib.c
 *
 *  Created on: Sep 11, 2023
 *      Author: Максим
 */
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "fl_lib.h"
#include <string.h>


//TODO добавить программирование опциональных байтов (Option bytes)
//TODO добавить флеш мемкопи


#define STM_FLESH_KEY1 FLASH_KEY1
#define STM_FLESH_KEY2 FLASH_KEY2

flrslt_t flash_memcpy(uint16_t* dest, uint16_t* src, size_t size);

flrslt_t flash_unlocking()
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
	FLASH->CR |= FLASH_CR_LOCK;
}

bool flash_available()
{
	while(FLASH->SR & FLASH_SR_BSY);
	//TODO вернуть TIME_ERROR;
	return true;
}

flrslt_t flash_write(uint32_t addr, uint16_t* data, size_t size)
{
	if(MAX_FLASH_ADDR < addr || MIN_FLASH_ADDR > addr || MAX_FLASH_ADDR < addr + size || addr & 1){
		return BAD_INPUT;
	}

	if(check_lock()){
		return FLESH_BLOCK;
	}

	uint8_t tmp_buff[FLASH_PG_SIZE] = {0};

	uint8_t num_page = (addr - MIN_FLASH_ADDR) / FLASH_PG_SIZE;

	uint32_t addr_pg = MIN_FLASH_ADDR + num_page* FLASH_PG_SIZE;

	flash_read(addr_pg, (uint16_t*)tmp_buff, FLASH_PG_SIZE);

	flash_page_erase(num_page);

	FLASH->CR |= FLASH_CR_PG;

	memcpy((uint16_t*)(tmp_buff + (addr - addr_pg)), data, size);

	flash_memcpy((uint16_t*)addr_pg, (uint16_t*)tmp_buff, FLASH_PG_SIZE/2);

	CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	return SUCCES;
}


flrslt_t flash_read(uint32_t addr, uint16_t* data, size_t size)
{
	if(MAX_FLASH_ADDR < addr || MIN_FLASH_ADDR > addr || MAX_FLASH_ADDR < addr + size || size & 1){
		return BAD_INPUT;
	}

	flash_available();

	flash_memcpy(data, (uint16_t*)addr, size/2);

	return SUCCES;
}


bool check_lock()
{
	return (bool)(FLASH->CR & FLASH_CR_LOCK);
}


flrslt_t flash_page_erase(uint8_t npage)
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

flrslt_t flash_mass_erase()
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

flrslt_t flash_memcpy(uint16_t* dest, uint16_t* src, size_t size)
{
	for(; size > 0; size--, dest++, src++)
	{
		*dest = *src;

		if(!flash_available()){
			CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
			return TIME_ERROR;
		}

	}

	return SUCCES;
}
