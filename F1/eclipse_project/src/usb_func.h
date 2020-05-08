
#ifndef USB_FUNC_H
#define USB_FUNC_H

#include <stdint.h>
#include "usb_def.h"
#include "usb_desc.h"
#include "cdc.h"
#include "gpio.h"
#include "flash.h"

/******* Struktur des Setup-Paketes *****/
typedef struct
{
    uint8_t bmRequestType; /* siehe oben */
    uint8_t bRequest;      /* siehe Request-Tafel in USB-Doku */
    uint16_t wValue;        /* je nach Request */
    uint16_t wIndex;        /* je nach Request */
    uint16_t wLength;       /* Anzahl Bytes, wenn Data-Stage vorhanden ist */
} setupPaket_t;

/******* Struktur des Kommando- und Org-Datenblockes *******************/
typedef struct
{
    setupPaket_t setupPacket; /* das jeweils letzte Setup-Paket   */
    int transferLen;               /* noch zum Host zu sendende Anzahl Bytes */
    int packetLen;                 /* wie lang das Paket zum Host sein darf */
    uint8_t* transferPtr;              /* zeigt auf die noch zu sendenden Bytes */

    bool remoteWakeup;
    bool selfPowered;
    int configuration;
} command_t;

/* Line coding structure
 0-3 BaudRate     Data terminal rate (baudrate), in bits per second
 4   bCharFormat  Stop bits: 0 - 1 Stop bit, 1 - 1.5 Stop bits, 2 - 2 Stop bits
 5   bParityType  Parity:    0 - None, 1 - Odd, 2 - Even, 3 - Mark, 4 - Space
 6   bDataBits    Data bits: 5, 6, 7, 8, 16
 */

#define lineCoding_t usb_cdc_line_coding

extern command_t CMD;
extern lineCoding_t lineCoding;
extern uint16_t Dtr_Rts;
extern const uint8_t ZERO;

extern void Class_Start(void);
extern void EnableUsbIRQ();
extern void Setup_flash();
extern void Setup_clocks();

extern int DataBeginTransmit();
extern void DataBeginReceive(); // called when Rx data can be processed again
extern int ReadControlBlock(uint8_t* pBuffer, int maxlen);
extern int SendData(int ep, uint8_t* pBuffer, int count);
//--------------------------------------------------------------------------
static inline void ACK(void)
{
	SendData(EP_CTRL, (uint8_t*) &ZERO, 0);
	trace("-ACK\n");
}

//-----------------------------------------------------------------------------
static inline void USB_SetAddress(uint8_t adr)
{
	strace(buf,"setAddr adr=%i\n",adr);
	USB_DADDR = 0x80 | adr;
 }

/* Request-Typ im Setup-Packet testen (Standard, Class, Vendor) */
//--------------------------------------------------------------------------
static inline bool IsStandardRequest(void)
{
    return (CMD.setupPacket.bmRequestType & 0x60) == 0;
}

//--------------------------------------------------------------------------
static inline bool IsClassRequest(void)
{
    return (CMD.setupPacket.bmRequestType & 0x60) == 0x20;
}

//--------------------------------------------------------------------------
static inline bool IsVendorRequest(void)
{
    return (CMD.setupPacket.bmRequestType & 0x60) == 0x40;
}


#define LED_ON		gpio_write_pin(LED_BUILTIN, 0)
#define LED_OFF		gpio_write_pin(LED_BUILTIN, 1)

//-----------------------------------------------------------------------------
#define FLASH_BASE			(0x08000000)
#define SRAM_BASE			(0x20000000)
// Bootloader size
#define BOOTLOADER_SIZE		(4 * 1024)

// SRAM size
#define SRAM_SIZE			(20 * 1024)

// SRAM end (bottom of stack)
#define SRAM_END			(SRAM_BASE + SRAM_SIZE)

// CDC Bootloader takes 4 kb flash.
#define USER_PROGRAM		(FLASH_BASE + BOOTLOADER_SIZE)
//-----------------------------------------------------------------------------
typedef union cmd_t {
	uint8_t data[8];
	struct {
		uint16_t start; // 0x41BE
		uint8_t id;
		uint8_t page;
		uint16_t data_len;
		uint16_t crc;
	};
} __attribute((packed)) cmd_t;
extern cmd_t cmd;

#define PAGE_SIZE	1024
extern int Check_CRC(uint8_t * buff, int len);

#define BAUD_RATE 230400

typedef enum {BUF_EMPTY, BUF_RECEIVING, BUF_FULL, BUF_SENDING} buf_status_t;
typedef enum {
	NO_ERROR,
	NO_SETUP,
	NO_FREE_BUFFER,
	DATA_OVERFLOW,
	DATA_UNDEFLOW,
	CMD_WRONG_LENGTH,
	CMD_WRONG_CRC,
	CMD_WRONG_ID
} error_t;

typedef struct buf_params_t {
	buf_status_t status;
	int wr_index;
	int len;
} buf_params_t;
extern buf_params_t buf_params[2];

extern int num_pages; // number of total pages to flash
extern int crt_page, page_offset;
extern int header_ok;
//-----------------------------------------------------------------------------



#endif // USB_FUNC_H
