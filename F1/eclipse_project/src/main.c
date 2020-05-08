/*
 * usb.h
 *
 *  Created on: Apr 25, 2020
 *      Author: stevestrong
 */

#include <stdio.h>

#include "usbstd.h"
#include "usb_def.h"
#include "usb_func.h"

#include "board.h"
#include "systick.h"

#include "bkp.h"
#include "nvic.h"



int flash_complete;
//-----------------------------------------------------------------------------
// Interrupt handlers
//-----------------------------------------------------------------------------
void SysTick_Handler(void)
{
	__exc_systick();
}
//-----------------------------------------------------------------------------
void IO_init()
{
    gpio_init_all(); // A, B and C

    // PC13 = Output (LED on Blue-Pill board)
	gpio_set_pin_mode(LED_BUILTIN, GPIO_OUTPUT_PP);
}
//-----------------------------------------------------------------------------
void IO_deinit()
{
    gpio_deinit_all(); // A, B and C
}
//-----------------------------------------------------------------------------
void Nop(uint32_t count)
{
    while (count)
    {
        asm volatile ("nop");
        count--;
    }
}
//-----------------------------------------------------------------------------
void USB_power_off(void)
{
	nvic_irq_disable(NVIC_USB_LP_CAN_RX0);
//	noInterrupts(); // if enabled, the USB in the user program will not work!!
	USB_CNTR = USB_CNTR_FRES;
	USB_ISTR = 0;
	USB_CNTR = USB_CNTR_FRES + USB_CNTR_PDWN;
}
//-----------------------------------------------------------------------------
void Setup_sys()
{
	crt_page = 0;
	num_pages = 0;
}
//-----------------------------------------------------------------------------
// USB-Setup
//-----------------------------------------------------------------------------
void UsbSetup(void)
{
	trace("setup\n");

    uint32_t* P = (uint32_t*) USB_RAM; // clear USB RAM memory
    while ((uint32_t) P < (USB_RAM + 1024))
    {
        *P++ = 0;
    }

    // Enable USB
	rcc_clk_enable(RCC_USB);

	Setup_sys();
	CMD.configuration = 0;

    Class_Start();            // setup LineCoding-Block with default values

	USB_power_off();

	// wait a bit for analog part to be ready (tSTARTUP)
    Nop(10);

	// turn on USB, activate interrupts
	USB_CNTR = USB_CNTR_CTRM | USB_CNTR_RESETM;

    EnableUsbIRQ();
}
//-----------------------------------------------------------------------------
void GpioToggle(void)
{
	//Reset the USB interface
	gpio_set_pin_mode(PA12, GPIO_OUTPUT_PP);
	gpio_write_pin(PA12, 0);
	for(volatile unsigned int i=0; i<72000; i++) __asm("nop"); // Only small delay seems to be needed, and USB pins will get configured in Serial.begin
	//gpio_write_pin(PA12, 1);
	gpio_set_pin_mode(PA12, GPIO_INPUT_FLOATING);
}

//-----------------------------------------------------------------------------
int Check_CRC(uint8_t * buff, int len)
{
	if (len<=2) return 0;
	len -= 2; // don't process the last two bytes, they are the CRC

	uint16_t crc = 0;
	while ( (len--)>0 )
		crc += (*buff++);

	crc ^= 0xFFFF;

	uint16_t temp = (*buff++);
	temp |= (*buff)<<8;

	return (crc==temp);
}
//-----------------------------------------------------------------------------
int Calculate_CRC(uint8_t * buff, int len)
{
	if (len==0) return 0;

	uint16 crc = 0xFFFF; // init CRC
	while ( (len--)>0 )
		crc += *buff++;

	return (crc ^ 0xFFFF);
}
//-----------------------------------------------------------------------------
void yield(void)
{
	// check number of written pages
	if ( num_pages>0 && crt_page==num_pages)
	{	// end of flashing process
		flash_complete = true;
		flash_lock();
	}
}
//-----------------------------------------------------------------------------
static bool Check_user_code(uint32_t user_address)
{
	uint32_t sp = *(volatile uint32_t *) user_address;

	// Check if the stack pointer in the vector table points somewhere in SRAM
	return ((sp & 0x2FFE0000) == SRAM_BASE) ? true : false;
}
//-----------------------------------------------------------------------------
static uint16_t Get_and_clear_magic_word(void)
{
	bkp_init();
	uint16_t value = bkp_read(10);
	if (value)
	{
		// Enable write access to the backup registers and the RTC.
		bkp_enable_writes();
		bkp_write(10, 0);
		bkp_disable_writes();
	}
	bkp_deinit();
	return value;
}
//-----------------------------------------------------------------------------
void __set_MSP(uint32_t topOfMainStack)
{
	asm volatile ("MSR msp, %0" : : "g" (topOfMainStack) );
}

//-----------------------------------------------------------------------------
void Main_init(void)
{
    GpioToggle();

    UsbSetup();

	systick_init();
}
//-----------------------------------------------------------------------------
void Main_loop()
{
	LED_ON;
	delay(50); // will call yield()

	LED_OFF;
	delay(50); // will call yield()

#ifdef USB_DEBUG

#if 1
	extern char dBuf[];
	extern int dLen;
	if ( usart_rx_available(USART1) )
		//if ( usart_getc(USART1)=='p' )
		{	// output to serial the content of logged information
			usart_getc(USART1);
			usart_putudec(USART1, dLen); usart_putc(USART1, '\n'); // number of logged data
			usart_tx(USART1, dBuf, dLen); // the logged data itself
		}
#endif
	//SendDataToSerial1();
#endif
}

//-----------------------------------------------------------------------------
int main(void)
{
    Setup_flash();
    Setup_clocks();

	// Initialize GPIOs
	IO_init();

	// Enter the bootloader if:
	// - PB2 (BOOT 1 pin) is HIGH or
	// - no User Code is uploaded to the MCU or
	// - a magic word was stored in the battery-backed RAM registers by the user program

	uint16_t magic_word = Get_and_clear_magic_word();

	if ((magic_word == 0x424C) ||
		gpio_read_pin(PB2) ||
		(Check_user_code(USER_PROGRAM) == false))
	{
		Main_init();

		// wait till flash writing process is complete
		while (flash_complete == false)
		{
			Main_loop(); // the flashing end check is performed in yield() during delay();
		}
		// turn off everything
		USB_power_off();
		systick_disable();

		// go and jump to user program
	}

	// Turn GPIO clocks off
	IO_deinit();

	voidFuncPtr UserProgram = (voidFuncPtr) *(volatile uint32_t *) (USER_PROGRAM + 0x04);

	// Setup the vector table to the final user-defined one in Flash memory
	nvic_set_vector_table(USER_PROGRAM, 0);

	// Setup the stack pointer to the user-defined one
	__set_MSP((*(volatile uint32_t *) USER_PROGRAM));

	// Jump to the user program entry point
	UserProgram();

	// Never reached
	while(1);

}
