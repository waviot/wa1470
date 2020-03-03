#include "wa1470.h"

#define SPI_WAIT_TIMEOUT		100000

wa1470_HAL_st *wa1470_hal = 0;
ischeduler_st* wa1470_scheduler = 0;


_Bool send_by_dbpsk;

#ifdef WA1470_LOG
char wa1470_log_string[256];
#endif

void wa1470_spi_write(uint16_t address, uint8_t *data, uint8_t length)
{
        wa1470_hal->__wa1470_disable_pin_irq();
        wa1470_hal->__spi_cs_set(0);
	address |= 0x8000;
	wa1470_hal->__spi_tx(((uint8_t*)(&address)) + 1, 1);
	wa1470_hal->__spi_tx(((uint8_t*)(&address)), 1);
	wa1470_hal->__spi_tx(data, length);
	wa1470_hal->__spi_cs_set(1);
        wa1470_hal->__wa1470_enable_pin_irq();
}

void wa1470_spi_read(uint16_t address, uint8_t *data, uint8_t length)
{
        wa1470_hal->__wa1470_disable_pin_irq();
	wa1470_hal->__spi_cs_set(0);
	address &= 0x7fff;
	wa1470_hal->__spi_tx(((uint8_t*)(&address)) + 1, 1);
	wa1470_hal->__spi_tx(((uint8_t*)(&address)), 1);
	wa1470_hal->__spi_rx(data, length);
	wa1470_hal->__spi_cs_set(1);
	wa1470_hal->__wa1470_enable_pin_irq();
}


void wa1470_spi_write8(uint16_t address, uint8_t data)
{
	wa1470_spi_write(address, &data, 1);
}

uint8_t wa1470_spi_read8(uint16_t address)
{
	uint8_t data;
	wa1470_spi_read(address, &data, 1);
	return data;
}

_Bool wa1470_spi_wait_for(uint16_t address, uint8_t value, uint8_t mask)
{
	uint32_t timeout = 0;
	while((wa1470_spi_read8(address) & mask) != value)
	{
		if(++timeout >= SPI_WAIT_TIMEOUT) return 0;
	}
	return 1;
}


void wa1470_init(_Bool send_by_bpsk_pin, uint32_t preambule, wa1470_HAL_st* hal_ptr, ischeduler_st* scheduler)
{
        wa1470_hal = hal_ptr;
        wa1470_scheduler = scheduler;
        if((wa1470_hal == 0) || (wa1470_scheduler == 0)) while(1); //HAL and scheduler pointers must be provided 
        send_by_dbpsk = send_by_bpsk_pin;
	wa1470rfe_init();
        wa1470dem_init(preambule);
	wa1470mod_init(send_by_dbpsk); 
}

void wa1470_reinit(uint32_t preambule)
{
        wa1470rfe_deinit();
        wa1470_hal->__wa1470_nop_dalay_ms(1);
        wa1470rfe_init();
        wa1470dem_init(preambule);
  	wa1470mod_init(send_by_dbpsk); 
}

void wa1470_isr()
{
	wa1470dem_isr();
	wa1470mod_isr();
}

_Bool wa1470_cansleep()
{
	return 1;
}

