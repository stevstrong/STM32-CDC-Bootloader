/**
 * usb_cdc_defines USB CDC Type Definitions
 *
*/
#ifndef __CDC_H
#define __CDC_H

/* Definitions of Communications Device Class from
 * "Universal Serial Bus Class Definitions for Communications Devices
 * Revision 1.2"
 */

/* Table 2: Communications Device Class Code */
#define USB_CLASS_CDC					0x02

/* Table 4: Class Subclass Code */
#define USB_CDC_SUBCLASS_DLCM			0x01
#define USB_CDC_SUBCLASS_ACM			0x02
/* ... */

/* Table 5 Communications Interface Class Control Protocol Codes */
#define USB_CDC_PROTOCOL_NONE			0x00
#define USB_CDC_PROTOCOL_AT				0x01
/* ... */

/* Table 6: Data Interface Class Code */
#define USB_CLASS_DATA					0x0A

/* Table 12: Type Values for the bDescriptorType Field */
#define CS_INTERFACE					0x24
#define CS_ENDPOINT						0x25

/* Table 13: bDescriptor SubType in Communications Class Functional
 * Descriptors */
#define USB_CDC_TYPE_HEADER				0x00
#define USB_CDC_TYPE_CALL_MANAGEMENT	0x01
#define USB_CDC_TYPE_ACM				0x02
/* ... */
#define USB_CDC_TYPE_UNION				0x06
/* ... */

/* Table 15: Class-Specific Descriptor Header Format */
typedef struct usb_cdc_header_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdCDC;
} __attribute((packed)) usb_cdc_header_descriptor;

/* Table 16: Union Interface Functional Descriptor */
typedef struct usb_cdc_union_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bControlInterface;
	uint8_t bSubordinateInterface0;
	/* ... */
} __attribute((packed)) usb_cdc_union_descriptor;


/* Definitions for Abstract Control Model devices from:
 * "Universal Serial Bus Communications Class Subclass Specification for
 * PSTN Devices"
 */

/* Table 3: Call Management Functional Descriptor */
typedef struct usb_cdc_call_management_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} __attribute((packed)) usb_cdc_call_management_descriptor;

/* Table 4: Abstract Control Management Functional Descriptor */
typedef struct usb_cdc_acm_descriptor {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
} __attribute((packed)) usb_cdc_acm_descriptor;

/* Table 13: Class-Specific Request Codes for PSTN subclasses */
/* ... */
#define USB_CDC_REQ_SET_LINE_CODING			0x20
#define USB_CDC_REQ_GET_LINE_CODING			0x21
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE	0x22
#define USB_CDC_REQ_SEND_BREAK				0x23


/* Table 17: Line Coding Structure */
typedef struct usb_cdc_line_coding {
	uint32_t baudRate; // dwDTERate;
	uint8_t stopBits; //bCharFormat;
	uint8_t parityType; //bParityType;
	uint8_t dataBits; //bDataBits;
} __attribute((packed)) usb_cdc_line_coding;

enum usb_cdc_line_coding_bCharFormat {
	USB_CDC_1_STOP_BITS			= 0,
	USB_CDC_1_5_STOP_BITS		= 1,
	USB_CDC_2_STOP_BITS			= 2,
};

enum usb_cdc_line_coding_bParityType {
	USB_CDC_NO_PARITY			= 0,
	USB_CDC_ODD_PARITY			= 1,
	USB_CDC_EVEN_PARITY			= 2,
	USB_CDC_MARK_PARITY			= 3,
	USB_CDC_SPACE_PARITY		= 4,
};

/* Table 30: Class-Specific Notification Codes for PSTN subclasses */
#define USB_CDC_NOTIFY_SERIAL_STATE		0x20


/* Notification Structure */
struct usb_cdc_notification {
	uint8_t bmRequestType;
	uint8_t bNotification;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
} __attribute__((packed));

#endif

