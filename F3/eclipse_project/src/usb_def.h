
#ifndef USB_DEF_H
#define USB_DEF_H

#include <stdbool.h>
#include <stdint.h>
#include "usb_desc.h"

//#define USB_DEBUG 1

//-----------------------------------------------------------------------------
// Drive USB as virtual COM port
//-----------------------------------------------------------------------------

/* For devices with 2 x 16 bits / uint16_t access schema (e.g. STM32L0, STM32F303xD and xE) */
//#define UMEM_SHIFT 0
//#define UMEM_FAKEWIDTH uint16_t

/* For devices with 1 x 16 bits / uint16_t access schema (e.g. STM32F103, STM32F302, STM32F303xB and xC) */
#define UMEM_SHIFT 1
#define UMEM_FAKEWIDTH uint32_t

/* The name of the IRQ handler must match startup_stm32.s */
#define NAME_OF_USB_IRQ_HANDLER USB_LP_CAN_RX0_IRQHandler

/* Take the number from the reference manual of your µC. */
#define USB_IRQ_NUMBER 20

// Enable trace messages
#define ENABLE_TRACING 0

#if ENABLE_TRACING
    #include <stdio.h>

    #define ITM_PORT0_8   (*(volatile char *)(0xE0000000UL))
    #define ITM_PORT0_32  (*(volatile unsigned long *)(0xE0000000UL))
    #define ITM_TER       (*(volatile unsigned long *)(0xE0000E00UL))
    #define ITM_TCR       (*(volatile unsigned long *)(0xE0000E80UL))

    static void trace(char *ptr)
    {
        while (*ptr)
        {
            if (((ITM_TCR & 1UL) != 0UL) &&   // ITM enabled
                ((ITM_TER & 1UL) != 0UL))     // ITM and port 0 enabled
            {
                while (ITM_PORT0_32 == 0UL)
                {
                    asm volatile("NOP");
                }
                ITM_PORT0_8 = *ptr++;
            }
        }
    }
#else
#include "libmaple/systick.h"
	#ifdef USB_DEBUG
		extern void DoTrace(char *, int);
		extern void NrTrace(int, int);
		//#define trace(s) { char* dest = &dBuf[dLen]; dLen += (sizeof(s)-1); const char* src = s; for (uint32_t i=0; i<(sizeof(s)-1); i++) *dest++ = *src++; }
		#define trace(s) DoTrace(s, sizeof(s)-1)
		#define ntrace(d,nl) NrTrace(d, nl)
		#define TIME_STAMP trace("["); NrTrace(millis(), 0); trace("]");
	#else
		#define trace(...)
		#define ntrace(d,nl)
		#define TIME_STAMP
	#endif
#define strace(...)
#endif


/*
 Attention! The following special RAM handling is not valid for STM32F303xD and xE!

 Layout of USB RAM (up to 512 Bytes)
 ================================
 The RAM goes (seen by the CPU) from 0x40006000 to 0x400063FF, = 0x400 (1024) bytes
 The RAM is made by 4 bytes slots, wherein only the lower 16 bits are used for data

 Example memory for "Hello-World"
 0x40006000: 48 65 00 00 6C 6C 00 00 6F 2D 00 00 57 6F 00 00 72 6C 00 00 64 ...
             H  e        l  l        o  -        W  o        r  l        d ...
 */
#define EP_DATA_LEN   64

#define EP_RX_LEN_ID   ((1<<15)|(1<<10))

#define EP_INT_MAX_LEN    8
#define EP_INT_LEN_ID    (4<<10)


#define USB_EpRegs(x) (*(volatile uint16_t *)(0x40005C00 + 4*(x)))

//-----------------------------------------------------------------------------
// EP table
typedef struct epTableEntry_t
{
    UMEM_FAKEWIDTH txOffset;
    UMEM_FAKEWIDTH txCount;
    UMEM_FAKEWIDTH rxOffset;
    UMEM_FAKEWIDTH rxCount;
} epTableEntry_t;

#define EP_TABLE_ADDR_OFFSET	(6*EP_DATA_LEN) // must be after the end of 6 EP data blocks

#define EP_TABLE_ADDR			(USB_PMAADDR + (EP_TABLE_ADDR_OFFSET<<UMEM_SHIFT))

#define EpTable   				((epTableEntry_t *) EP_TABLE_ADDR)

#define EP_TABLE_SIZE			(8*4*4) // 4 x uint32 for each of the 8 EPs

//-----------------------------------------------------------------------------
// EP buffer address offsets
// EP0 = control
#define EP_CTRL_TX_OFFSET   0      /* 64 Bytes ab   0 */
#define EP_CTRL_RX_OFFSET   (EP_CTRL_TX_OFFSET + EP_DATA_LEN) // 64

// EP1 = Bulk-IN+OUT for DATA
#define EP_DATA_TX_OFFSET  (EP_CTRL_RX_OFFSET + EP_DATA_LEN) //128
#define EP_DATA_RX_OFFSET  (EP_DATA_TX_OFFSET + EP_DATA_LEN) //192

// EP2 = Bulk-IN+OUT for COMM
#define EP_COMM_TX_OFFSET  (EP_DATA_RX_OFFSET + EP_DATA_LEN) //256
#define EP_COMM_RX_OFFSET  (EP_COMM_TX_OFFSET + EP_INT_MAX_LEN) //264
//-----------------------------------------------------------------------------
// EP buffer absolute addresses
#define USB_EP_BUF_START       (USB_PMAADDR)

#define EP_CTRL_TX_BUF_ADDRESS	(USB_EP_BUF_START + (EP_CTRL_TX_OFFSET<<UMEM_SHIFT))
#define EP_CTRL_RX_BUF_ADDRESS	(USB_EP_BUF_START + (EP_CTRL_RX_OFFSET<<UMEM_SHIFT))

#define EP_DATA_TX_BUF_ADDRESS	(USB_EP_BUF_START + (EP_DATA_TX_OFFSET<<UMEM_SHIFT))
#define EP_DATA_RX_BUF_ADDRESS	(USB_EP_BUF_START + (EP_DATA_RX_OFFSET<<UMEM_SHIFT))

#define EP_COMM_TX_BUF_ADDRESS	(USB_EP_BUF_START + (EP_COMM_TX_OFFSET<<UMEM_SHIFT))
#define EP_COMM_RX_BUF_ADDRESS	(USB_EP_BUF_START + (EP_COMM_RX_OFFSET<<UMEM_SHIFT))


typedef struct epTableAddress_t {
    uint32_t * txAddr;
    uint32_t * rxAddr;
} epTableAddress_t;
extern const epTableAddress_t epTableAddr[EP_LAST]; // number of EPs



#endif // USB_DEF_H
