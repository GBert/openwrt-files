// Firmware framework for USB I/O on PIC 18F2455 (and siblings)
// Copyright (C) 2005 Alexander Enzmann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef USB_H
#define USB_H

#define DEBUG_PRINT 0

#define PTR16(x) ((unsigned int)(((unsigned long)x) & 0xFFFF))

typedef unsigned char byte;
typedef unsigned int word;

#define LSB(x) (x & 0xFF)
#define MSB(x) ((x & 0xFF00) >> 8)

//
// Standard Request Codes USB 2.0 Spec Ref Table 9-4
//
#define GET_STATUS         0
#define CLEAR_FEATURE      1
#define SET_FEATURE        3
#define SET_ADDRESS        5
#define GET_DESCRIPTOR     6
#define SET_DESCRIPTOR     7
#define GET_CONFIGURATION  8
#define SET_CONFIGURATION  9
#define GET_INTERFACE     10
#define SET_INTERFACE     11
#define SYNCH_FRAME       12

// Descriptor Types
#define DEVICE_DESCRIPTOR        0x01
#define CONFIGURATION_DESCRIPTOR 0x02
#define STRING_DESCRIPTOR        0x03
#define INTERFACE_DESCRIPTOR     0x04
#define ENDPOINT_DESCRIPTOR      0x05

// Definitions from "Device Class Definition for Human Interface Devices (HID)",
// version 1.11
//

// Class Descriptor Types
#define HID_DESCRIPTOR      0x21
#define REPORT_DESCRIPTOR   0x22
#define PHYSICAL_DESCRIPTOR 0x23

// HID Class specific requests
#define GET_REPORT      0x01
#define GET_IDLE        0x02
#define GET_PROTOCOL    0x03
#define SET_REPORT      0x09
#define SET_IDLE        0x0A
#define SET_PROTOCOL    0x0B

// Standard Feature Selectors
#define DEVICE_REMOTE_WAKEUP    0x01
#define ENDPOINT_HALT           0x00


// Buffer Descriptor bit masks (from PIC datasheet)
#define UOWN   0x80	// USB Own Bit
#define DTS    0x40	// Data Toggle Synchronization Bit
#define KEN    0x20	// BD Keep Enable Bit
#define INCDIS 0x10	// Address Increment Disable Bit
#define DTSEN  0x08	// Data Toggle Synchronization Enable Bit
#define BSTALL 0x04	// Buffer Stall Enable Bit
#define BC9    0x02	// Byte count bit 9
#define BC8    0x01	// Byte count bit 8

// Device states (Chap 9.1.1)
#define DETACHED     0
#define ATTACHED     1
#define POWERED      2
#define DEFAULT      3
#define ADDRESS      4
#define CONFIGURED   5

typedef struct _BDT
{
	byte Stat;
	byte Cnt;
	unsigned int ADDR;
} BDT;				//Buffer Descriptor Table

//
// Global variables
extern byte deviceState;	// Visible device states (from USB 2.0, chap 9.1.1)
extern byte selfPowered;
extern byte remoteWakeup;
extern byte currentConfiguration;

extern volatile BDT __at 0x0400 ep0Bo;	//Endpoint #0 BD Out
extern volatile BDT __at 0x0404 ep0Bi;	//Endpoint #0 BD In
extern volatile BDT __at 0x0408 ep1Bo;	//Endpoint #1 BD Out
extern volatile BDT __at 0x040C ep1Bi;	//Endpoint #1 BD In

// Every device request starts with an 8 byte setup packet (USB 2.0, chap 9.3)
// with a standard layout.  The meaning of wValue and wIndex will
// vary depending on the request type and specific request.
typedef struct _setupPacketStruct
{
	byte bmRequestType;	// D7: Direction, D6..5: Type, D4..0: Recipient
	byte bRequest;		// Specific request
	byte wValue0;		// LSB of wValue
	byte wValue1;		// MSB of wValue
	byte wIndex0;		// LSB of wIndex
	byte wIndex1;		// MSB of wIndex
	word wLength;		// Number of bytes to transfer if there's a data stage
	byte extra[56];		// Fill out to same size as Endpoint 0 max buffer
} setupPacketStruct;

extern volatile setupPacketStruct SetupPacket;

// Size of the buffer for endpoint 0
#define E0SZ 64

// Size of the feature, input, and output reports
#define HID_INPUT_REPORT_BYTES   64
#define HID_OUTPUT_REPORT_BYTES  64
#define HID_FEATURE_REPORT_BYTES 64

// HID specific buffers
extern volatile byte HIDRxBuffer[HID_OUTPUT_REPORT_BYTES];	// Data received OUT from the host
extern volatile byte HIDTxBuffer[HID_INPUT_REPORT_BYTES];	// Data sent IN to host

// I/O buffers
extern volatile byte rxBuffer[HID_OUTPUT_REPORT_BYTES];
extern volatile byte txBuffer[HID_INPUT_REPORT_BYTES];
extern volatile byte HIDFeatureBuffer[HID_FEATURE_REPORT_BYTES];

// inPtr/OutPtr are used to move data from user memory (RAM/ROM/EEPROM) buffers
// from/to USB dual port buffers.
extern byte *outPtr;		// Address of buffer to send to host
extern byte *inPtr;		// Address of buffer to receive data from host
extern unsigned int wCount;	// Total # of bytes to move

// USB Functions
void EnableUSBModule(void);
void ProcessUSBTransactions(void);

// # of bytes from last HID transaction
extern byte hidRxLen;

// Functions for reading/writing the HID interrupt endpoint
byte HIDTxReport(byte * buffer, byte len);
byte HIDRxReport(byte * buffer, byte len);

#endif				// !USB_H
