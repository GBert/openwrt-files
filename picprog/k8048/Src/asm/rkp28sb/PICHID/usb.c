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
#include <pic18fregs.h>
#include <string.h>
#include <stdio.h>
#include "usb.h"
#include "callbacks.h"

#define ALLOW_SUSPEND 0
// It appears that you need at least 6 loops that are replaced by memcpy()
// before it is an advantage.
#define USE_MEMCPY 0

// Device and configuration descriptors.  These are used as the
// host enumerates the device and discovers what class of device
// it is and what interfaces it supports.
#define DEVICE_DESCRIPTOR_SIZE 0x12
#define CONFIG_HEADER_SIZE  0x09
#define HID_DESCRIPTOR_SIZE 0x20
#define HID_HEADER_SIZE 0x09
typedef struct _configStruct
{
	byte configHeader[CONFIG_HEADER_SIZE];
	byte HIDDescriptor[HID_DESCRIPTOR_SIZE];
} ConfigStruct;

// Global variables
byte deviceState;
byte remoteWakeup;
byte deviceAddress;
byte selfPowered;
byte currentConfiguration;

// Control Transfer Stages - see USB spec chapter 5
#define SETUP_STAGE    0	// Start of a control transfer (followed by 0 or more data stages)
#define DATA_OUT_STAGE 1	// Data from host to device
#define DATA_IN_STAGE  2	// Data from device to host
#define STATUS_STAGE   3	// Unused - if data I/O went ok, then back to Setup

byte ctrlTransferStage;		// Holds the current stage in a control transfer

byte HIDPostProcess;		// Set to 1 if HID needs to process after the data stage
byte requestHandled;		// Set to 1 if request was understood and processed.

byte *outPtr;			// Data to send to the host
byte *inPtr;			// Data from the host
word wCount;			// Number of bytes of data

// HID Class variables
byte hidIdleRate;
byte hidProtocol;		// [0] Boot Protocol [1] Report Protocol
byte hidRxLen;			// # of bytes put into buffer

#ifndef idVendor
#define idVendor  (0x04D8)	// Vendor Microchip
#endif
#ifndef idProduct
#define idProduct (0x285B)	// Product 285B
#endif
__code byte deviceDescriptor[] = {
	0x12, 0x01,	// bLength, bDescriptorType
	0x00, 0x02,	// bcdUSB (low byte), bcdUSB (high byte)
	0x00, 0x00,	// bDeviceClass, bDeviceSubClass
	0x00, E0SZ,	// bDeviceProtocl, bMaxPacketSize
	LSB(idVendor),	// idVendor (low byte)
	MSB(idVendor),	// idVendor (high byte)
	LSB(idProduct),	// idProduct (low byte)
	MSB(idProduct),	// idProduct (high byte)
	0x01, 0x00,	// bcdDevice (low byte), bcdDevice (high byte)
	0x01, 0x02,	// iManufacturer, iProduct
	0x00, 0x01	// iSerialNumber (none), bNumConfigurations
};

// Total config size is 0x09 + 0x09 + 0x09 + 0x07 + 0x07 = 0x29
#define CFSZ 0x29
#define HISZ HID_INPUT_REPORT_BYTES
#define HOSZ HID_OUTPUT_REPORT_BYTES

__code ConfigStruct configDescriptor = {
	{
	 // Configuration descriptor
	 0x09, 0x02,	// bLength, bDescriptorType (Configuration)
	 CFSZ, 0x00,	// wTotalLength (low), wTotalLength (high)
	 0x01, 0x01,	// bNumInterfaces, bConfigurationValue
	 0x00, 0xA0,	// iConfiguration, bmAttributes ()
	 0x32,	// bMaxPower
	 },
	{
	 // HID Interface descriptor
	 0x09, 0x04,	// bLength, bDescriptorType (Interface)
	 0x00, 0x00,	// bInterfaceNumber, bAlternateSetting
	 0x02, 0x03,	// bNumEndpoints, bInterfaceClass (HID)
	 0x00, 0x00,	// bInterfaceSubclass, bInterfaceProtocol,
	 0x00,	// iInterface
	 // Hid descriptor
	 0x09, 0x21,	// bLength, bDescriptorType (HID)
	 0x01, 0x01,	// bcdHID (low), bcdHID (high)
	 0x00, 0x01,	// bCountryCode, bNumDescriptors
	 0x22, 0x2F,	// bDescriptorType, wDescriptorLength (low)
	 0x00,	// wDescriptorLength (high)
	 // HID Endpoint 1 In
	 0x07, 0x05,	// bLength, bDescriptorType (Endpoint)
	 0x81, 0x03,	// bEndpointAddress, bmAttributes (Interrupt)
	 HISZ, 0x00,	// wMaxPacketSize (low), wMaxPacketSize (high)
	 0x01,	// bInterval (1 millisecond)
	 // HID Endpoint 1 Out
	 0x07, 0x05,	// bLength, bDescriptorType (Endpoint)
	 0x01, 0x03,	// bEndpointAddress, bmAttributes (Interrupt)
	 HOSZ, 0x00,	// wMaxPacketSize (low), wMaxPacketSize (high)
	 0x01,	// bInterval (1 millisecond)
	 }
};

#define HIRB HID_INPUT_REPORT_BYTES
#define HORB HID_OUTPUT_REPORT_BYTES
#define HFRB HID_FEATURE_REPORT_BYTES

#define HID_REPORT_SIZE 0x2f	// Size is from HID Descriptor tool
__code byte HIDReport[HID_REPORT_SIZE] = {
	0x06, 0xa0, 0xff,	// USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,	// USAGE (Vendor Usage 1)
	0xa1, 0x01,	// COLLECTION (Application)
	0x09, 0x02,	//   USAGE (Vendor Usage 1)
	0x15, 0x00,	//   LOGICAL_MINIMUM (0)
	0x26, 0xFF, 0x00,	//   LOGICAL_MAXIMUM (255)
	0x75, 0x08,	//   REPORT_SIZE (8)
	0x95, HIRB,	//   REPORT_COUNT (HIRB)
	0x81, 0x02,	//   INPUT (Data,Var,Abs)
	0x09, 0x02,	//   USAGE (Vendor Usage 1)
	0x15, 0x00,	//   LOGICAL_MINIMUM (0)
	0x26, 0xFF, 0x00,	//   LOGICAL_MAXIMUM (255)
	0x75, 0x08,	//   REPORT_SIZE (8)
	0x95, HORB,	//   REPORT_COUNT (HORB)
	0x91, 0x02,	//   OUTPUT (Data,Var,Abs)
	0x09, 0x01,	//   USAGE (Vendor Usage 1)
	0x15, 0x00,	//   LOGICAL_MINIMUM (0)
	0x26, 0xFF, 0x00,	//   LOGICAL_MAXIMUM (255)
	0x75, 0x08,	//   REPORT_SIZE (8)
	0x95, HFRB,	//   REPORT_COUNT (HFRB)
	0xb1, 0x02,	//   FEATURE (Data,Var,Abs)
	0xc0	// END_COLLECTION
};

// EN-US
__code byte stringDescriptor0[] = {
	0x04, STRING_DESCRIPTOR,	// bLength, bDscType
	0x09, 0x04,
};
// RKE
__code byte stringDescriptor1[] = {
	0x08, STRING_DESCRIPTOR,	// bLength, bDscType
	'R', 0x00, 'K', 0x00, 'E', 0x00
};
// RKP28sb
__code byte stringDescriptor2[] = {
	0x10, STRING_DESCRIPTOR,	// bLength, bDscType
	'R', 0x00, 'K', 0x00, 'P', 0x00,
	'2', 0x00, '8', 0x00, 's', 0x00, 'b', 0x00
};

volatile BDT __at 0x0400 ep0Bo;	//Endpoint #0 BD Out
volatile BDT __at 0x0404 ep0Bi;	//Endpoint #0 BD In
volatile BDT __at 0x0408 ep1Bo;	//Endpoint #1 BD Out
volatile BDT __at 0x040C ep1Bi;	//Endpoint #1 BD In

// TBD: add definitions for additional endpoints (2-16).

// Put endpoint 0 buffers into dual port RAM
#pragma udata usbram5 SetupPacket controlTransferBuffer
volatile setupPacketStruct SetupPacket;
volatile byte controlTransferBuffer[E0SZ];

// Put USB I/O buffers into dual port RAM.
#pragma udata usbram5 HIDRxBuffer HIDTxBuffer

// HID specific buffers
volatile byte HIDRxBuffer[HID_OUTPUT_REPORT_BYTES];
volatile byte HIDTxBuffer[HID_INPUT_REPORT_BYTES];

// I/O buffers
volatile byte rxBuffer[HID_OUTPUT_REPORT_BYTES];
volatile byte txBuffer[HID_INPUT_REPORT_BYTES];
volatile byte HIDFeatureBuffer[HID_FEATURE_REPORT_BYTES];

//
// Start of code for HID specific code.
//

// Send up to len bytes to the host.  The actual number of bytes sent is returned
// to the caller.  If the send failed (usually because a send was attempted while
// the SIE was busy processing the last request), then 0 is returned.
byte
HIDTxReport(byte * buffer, byte len)
{
#if !USE_MEMCPY
	byte i;
#endif
#if DEBUG_PRINT
	printf("HIDTxReport: %d\r\n", (word) len);
#endif
	// If the CPU still owns the SIE, then don't try to send anything.
	if (ep1Bi.Stat & UOWN)
		return 0;

	// Truncate requests that are too large.  TBD: send 
	if (len > HID_INPUT_REPORT_BYTES)
		len = HID_INPUT_REPORT_BYTES;

	// Copy data from user's buffer to dual-ram buffer
#if USE_MEMCPY
	memcpy(HIDTxBuffer, buffer, len);
#else
	for (i = 0; i < len; i++)
		HIDTxBuffer[i] = buffer[i];
#endif

	// Toggle the data bit and give control to the SIE
	ep1Bi.Cnt = len;
	if (ep1Bi.Stat & DTS)
		ep1Bi.Stat = UOWN | DTSEN;
	else
		ep1Bi.Stat = UOWN | DTS | DTSEN;

	return len;
}

// Read up to len bytes from HID output buffer.  Actual number of bytes put into
// buffer is returned.  If there are fewer than len bytes, then only the available
// bytes will be returned.  Any bytes in the buffer beyond len will be discarded.
byte
HIDRxReport(byte * buffer, byte len)
{
	hidRxLen = 0;
#if DEBUG_PRINT
	// printf("HIDRxReport: %d\r\n", len);
#endif

	// If the SIE doesn't own the output buffer descriptor, then it is safe to
	// pull data from it.
	if (!(ep1Bo.Stat & UOWN))
	{
		// See if the host sent fewer bytes that we asked for.
		if (len > ep1Bo.Cnt)
			len = ep1Bo.Cnt;

		// Copy data from dual-ram buffer to user's buffer
#if USE_MEMCPY
		memcpy(buffer, HIDRxBuffer, len);
#else
		for (hidRxLen = 0; hidRxLen < len; hidRxLen++)
		{
			buffer[hidRxLen] = HIDRxBuffer[hidRxLen];
		}
#endif

#if DEBUG_PRINT
		// printf("HIDRxReport: %d: 0x%hx 0x%hx 0x%hx 0x%hx\r\n",
		//    (word)len, (byte)buffer[0], (byte)buffer[1], (byte)buffer[2], (byte)buffer[3]);
#endif
		// Reset the HID output buffer descriptor so the host
		// can send more data.
		ep1Bo.Cnt = sizeof(HIDRxBuffer);
		if (ep1Bo.Stat & DTS)
			ep1Bo.Stat = UOWN | DTSEN;
		else
			ep1Bo.Stat = UOWN | DTS | DTSEN;
	}

	return hidRxLen;

}

// After configuration is complete, this routine is called to initialize
// the endpoints (e.g., assign buffer addresses).
void
HIDInitEndpoint(void)
{
	hidRxLen = 0;
#if DEBUG_PRINT
	// printf("HIDInitEndpoint\r\n");
#endif

	// Turn on both in and out for this endpoint
	UEP1 = 0x1E;

	ep1Bo.Cnt = sizeof(HIDRxBuffer);
	ep1Bo.ADDR = PTR16(&HIDRxBuffer);
	ep1Bo.Stat = UOWN | DTSEN;

	ep1Bi.ADDR = PTR16(&HIDTxBuffer);
	ep1Bi.Stat = DTS;
}

// If the setup stage indicates that this is a GET_REPORT request, we vector
// to user supplied routines to determine what (if any) data should be sent
// back.
void
HIDGetReport(void)
{
	byte reportID = SetupPacket.wValue0;
#if DEBUG_PRINT
	// printf("HIDGetReport: 0x%x\r\n", (word)SetupPacket.wValue1);
#endif

	// Determine which kind of report is being requested
	if (SetupPacket.wValue1 == 0x01)
		requestHandled = GetInputReport(reportID);
	else if (SetupPacket.wValue1 == 0x03)
		requestHandled = GetFeatureReport(reportID);
}

// Process SET_REPORT transaction
void
HIDSetReport(void)
{
	byte reportID = SetupPacket.wValue0;
	// The report type is in the high byte of the Setup packet's Value field.
	// 3 = Feature; 2 = Output.
#if DEBUG_PRINT
	// printf("HIDSetReport: 0x%ux\r\n", (word)SetupPacket.wValue1);
#endif
	if (SetupPacket.wValue1 == 0x02)
		requestHandled = SetupOutputReport(reportID);
	else if (SetupPacket.wValue1 == 0x03)
		requestHandled = SetupFeatureReport(reportID);
}

// Process HID specific requests
void
ProcessHIDRequest(void)
{
	byte bRequest;

	// Has to be to the HID interface
	if ((SetupPacket.bmRequestType & 0x1F) != 0x01 || (SetupPacket.wIndex0 != 0x00))
		return;

	bRequest = SetupPacket.bRequest;

	if (bRequest == GET_DESCRIPTOR)
	{
		// Request for a descriptor.
		byte descriptorType = SetupPacket.wValue1;
		if (descriptorType == HID_DESCRIPTOR)
		{
#if DEBUG_PRINT
			printf("HID: HID_DESCRIPTOR\r\n");
#endif
			// HID descriptor.
			requestHandled = 1;
			outPtr = (byte *) & configDescriptor.HIDDescriptor;
			wCount = HID_HEADER_SIZE;
		}
		else if (descriptorType == REPORT_DESCRIPTOR)
		{
#if DEBUG_PRINT
			printf("HID: REPORT_DESCRIPTOR\r\n");
#endif
			// Report descriptor.
			requestHandled = 1;
			outPtr = (byte *) HIDReport;
			wCount = HID_REPORT_SIZE;
		}
		else if (descriptorType == PHYSICAL_DESCRIPTOR)
		{
#if DEBUG_PRINT
			printf("HID: PHYSICAL_DESCRIPTOR\r\n");
#endif
			// Physical descriptor.
		}
		else
		{
#if DEBUG_PRINT
			printf("HID: Unsupported descriptor: 0x%x\r\n", SetupPacket.wValue1);
#endif
		}
	}

	if ((SetupPacket.bmRequestType & 0x60) != 0x20)
	{
#if DEBUG_PRINT
		// printf("Not CLASS request: 0x%x, 0x%x\r\n", (word)(SetupPacket.bmRequestType & 0x60), (word)SetupPacket.bRequest);
#endif
		return;
	}

	// HID-specific requests.
	if (bRequest == GET_REPORT)
	{
#if DEBUG_PRINT
		// printf("HID: GET_REPORT\r\n");
#endif
		HIDGetReport();
	}
	else if (bRequest == SET_REPORT)
	{
#if DEBUG_PRINT
		// printf("HID: SET_REPORT\r\n");
#endif
		HIDPostProcess = 1;
		HIDSetReport();
	}
	else if (bRequest == GET_IDLE)
	{
#if DEBUG_PRINT
		printf("HID: GET_IDLE\r\n");
#endif
		requestHandled = 1;
		outPtr = &hidIdleRate;
		wCount = 1;
	}
	else if (bRequest == SET_IDLE)
	{
#if DEBUG_PRINT
		printf("HID: SET_IDLE\r\n");
#endif
		requestHandled = 1;
		hidIdleRate = SetupPacket.wValue1;
	}
	else if (bRequest == GET_PROTOCOL)
	{
#if DEBUG_PRINT
		printf("HID: GET_PROTOCOL\r\n");
#endif
		requestHandled = 1;
		outPtr = &hidProtocol;
		wCount = 1;
	}
	else if (bRequest == SET_PROTOCOL)
	{
#if DEBUG_PRINT
		printf("HID: SET_PROTOCOL\r\n");
#endif
		requestHandled = 1;
		hidProtocol = SetupPacket.wValue0;
	}
	else
	{
#if DEBUG_PRINT
		printf("HID: Unknown request: 0x%x\r\n", SetupPacket.bRequest);
#endif
	}
}

//
// Start of code to process standard requests (USB chapter 9)
//

// Process GET_DESCRIPTOR
static void
GetDescriptor(void)
{
#if DEBUG_PRINT
	// printf("GetDescriptor\r\n");
#endif
	if (SetupPacket.bmRequestType == 0x80)
	{
		byte descriptorType = SetupPacket.wValue1;
		byte descriptorIndex = SetupPacket.wValue0;

		if (descriptorType == DEVICE_DESCRIPTOR)
		{
#if DEBUG_PRINT
			printf("DEVICE_DESCRIPTOR\r\n");
#endif
			requestHandled = 1;
			outPtr = (byte *) & deviceDescriptor;
			wCount = DEVICE_DESCRIPTOR_SIZE;
		}
		else if (descriptorType == CONFIGURATION_DESCRIPTOR)
		{
#if DEBUG_PRINT
			printf("CONFIGURATION_DESCRIPTOR: %d\r\n", descriptorIndex);
#endif
			requestHandled = 1;
#if 0
			outPtr = (byte *) & configDescriptor;
			// wCount = *(outPtr + 2);
			wCount = CFSZ;
#else
			// SDCC 2.5 makes bad code with the following two lines.  The
			// unusual thing is that it is the first of the two lines that
			// is compiled incorrectly (outPtr gets the contents of
			// configDescriptor rather than the address), even though it
			// is the second line that changes from above.
			outPtr = (byte *) & configDescriptor;
			wCount = configDescriptor.configHeader[2];	// Note: SDCC makes bad code with this
#endif
#if DEBUG_PRINT
			printf("Total config size: %d\r\n", wCount);
#endif
		}
		else if (descriptorType == STRING_DESCRIPTOR)
		{
#if DEBUG_PRINT
			printf("STRING_DESCRIPTOR: %d\r\n", (word) descriptorIndex);
#endif
			requestHandled = 1;
			if (descriptorIndex == 0)
				outPtr = (byte *) & stringDescriptor0;
			else if (descriptorIndex == 1)
				outPtr = (byte *) & stringDescriptor1;
			else
				outPtr = (byte *) & stringDescriptor2;
			wCount = *outPtr;
		}
		else
		{
#if DEBUG_PRINT
			printf("Unknown Descriptor: 0x%ux\r\n", (word) descriptorType);
#endif
		}
	}
}

// Process GET_STATUS
static void
GetStatus(void)
{
	// Mask off the Recipient bits
	byte recipient = SetupPacket.bmRequestType & 0x1F;
#if DEBUG_PRINT
	printf("GetStatus\r\n");
#endif
	controlTransferBuffer[0] = 0;
	controlTransferBuffer[1] = 0;

	// See where the request goes
	if (recipient == 0x00)
	{
		// Device
		requestHandled = 1;
		// Set bits for self powered device and remote wakeup.
		if (selfPowered)
			controlTransferBuffer[0] |= 0x01;
		if (remoteWakeup)
			controlTransferBuffer[0] |= 0x02;
	}
	else if (recipient == 0x01)
	{
		// Interface
		requestHandled = 1;
	}
	else if (recipient == 0x02)
	{
		// Endpoint
		byte endpointNum = SetupPacket.wIndex0 & 0x0F;
		byte endpointDir = SetupPacket.wIndex0 & 0x80;
		requestHandled = 1;
		// Endpoint descriptors are 8 bytes long, with each in and out taking 4 bytes
		// within the endpoint. (See PIC datasheet.)
		inPtr = (byte *) & ep0Bo + (endpointNum * 8);
		if (endpointDir)
			inPtr += 4;
		if (*inPtr & BSTALL)
			controlTransferBuffer[0] = 0x01;
	}

	if (requestHandled)
	{
		outPtr = (__data byte *) & controlTransferBuffer;
		wCount = 2;
	}
}

// Process SET_FEATURE and CLEAR_FEATURE
static void
SetFeature(void)
{
	byte recipient = SetupPacket.bmRequestType & 0x1F;
	byte feature = SetupPacket.wValue0;
#if DEBUG_PRINT
	// printf("SetFeature\r\n");
#endif

	if (recipient == 0x00)
	{
		// Device
		if (feature == DEVICE_REMOTE_WAKEUP)
		{
			requestHandled = 1;
			if (SetupPacket.bRequest == SET_FEATURE)
				remoteWakeup = 1;
			else
				remoteWakeup = 0;
		}
		// TBD: Handle TEST_MODE
	}
	else if (recipient == 0x02)
	{
		// Endpoint
		byte endpointNum = SetupPacket.wIndex0 & 0x0F;
		byte endpointDir = SetupPacket.wIndex0 & 0x80;
		if ((feature == ENDPOINT_HALT) && (endpointNum != 0))
		{
			// Halt endpoint (as long as it isn't endpoint 0)
			requestHandled = 1;
			// Endpoint descriptors are 8 bytes long, with each in and out taking 4 bytes
			// within the endpoint. (See PIC datasheet.)
			inPtr = (byte *) & ep0Bo + (endpointNum * 8);
			if (endpointDir)
				inPtr += 4;

			if (SetupPacket.bRequest == SET_FEATURE)
				*inPtr = 0x84;
			else
			{
				if (endpointDir == 1)
					*inPtr = 0x00;
				else
					*inPtr = 0x88;
			}
		}
	}
}

void
ProcessStandardRequest(void)
{
	byte request = SetupPacket.bRequest;

	if ((SetupPacket.bmRequestType & 0x60) != 0x00)
		// Not a standard request - don't process here.  Class or Vendor
		// requests have to be handled seperately.
		return;

	if (request == SET_ADDRESS)
	{
		// Set the address of the device.  All future requests
		// will come to that address.  Can't actually set UADDR
		// to the new address yet because the rest of the SET_ADDRESS
		// transaction uses address 0.
#if DEBUG_PRINT
		printf("SET_ADDRESS: %uhx\r\n", SetupPacket.wValue0);
#endif
		requestHandled = 1;
		deviceState = ADDRESS;
		deviceAddress = SetupPacket.wValue0;
	}
	else if (request == GET_DESCRIPTOR)
	{
		GetDescriptor();
	}
	else if (request == SET_CONFIGURATION)
	{
#if DEBUG_PRINT
		printf("SET_CONFIGURATION\r\n");
#endif
		requestHandled = 1;
		currentConfiguration = SetupPacket.wValue0;
		// TBD: ensure the new configuration value is one that
		// exists in the descriptor.
		if (currentConfiguration == 0)
			// If configuration value is zero, device is put in
			// address state (USB 2.0 - 9.4.7)
			deviceState = ADDRESS;
		else
		{
			// Set the configuration.
			deviceState = CONFIGURED;

			// Initialize the endpoints for all interfaces
			HIDInitEndpoint();

			// TBD: Add initialization code here for any additional
			// interfaces beyond the one used for the HID
		}
	}
	else if (request == GET_CONFIGURATION)
	{
#if DEBUG_PRINT
		printf("GET_CONFIGURATION\r\n");
#endif
		requestHandled = 1;
		outPtr = (byte *) & currentConfiguration;
		wCount = 1;
	}
	else if (request == GET_STATUS)
	{
		GetStatus();
	}
	else if ((request == CLEAR_FEATURE) || (request == SET_FEATURE))
	{
		SetFeature();
	}
	else if (request == GET_INTERFACE)
	{
		// No support for alternate interfaces.  Send
		// zero back to the host.
#if DEBUG_PRINT
		printf("GET_INTERFACE\r\n");
#endif
		requestHandled = 1;
		controlTransferBuffer[0] = 0;
		outPtr = (__data byte *) & controlTransferBuffer;
		wCount = 1;
	}
	else if (request == SET_INTERFACE)
	{
		// No support for alternate interfaces - just ignore.
#if DEBUG_PRINT
		printf("SET_INTERFACE\r\n");
#endif
		requestHandled = 1;
	}
	else if (request == SET_DESCRIPTOR)
	{
#if DEBUG_PRINT
		printf("SET_DESCRIPTOR\r\n");
#endif
	}
	else if (request == SYNCH_FRAME)
	{
#if DEBUG_PRINT
		printf("SYNCH_FRAME\r\n");
#endif
	}
	else
	{
#if DEBUG_PRINT
		printf("Default Std Request\r\n");
#endif
	}
}

// Data stage for a Control Transfer that sends data to the host
void
InDataStage(void)
{
#if !USE_MEMCPY
	byte i;
#endif
	word bufferSize;

	// Determine how many bytes are going to the host
	if (wCount < E0SZ)
		bufferSize = wCount;
	else
		bufferSize = E0SZ;

#if DEBUG_PRINT
	// printf("USBCtrlTrfTxService: %d\r\n", byte_to_send);
#endif

	// Load the high two bits of the byte count into BC8:BC9
	ep0Bi.Stat &= ~(BC8 | BC9);	// Clear BC8 and BC9
	ep0Bi.Stat |= (byte) ((bufferSize & 0x0300) >> 8);
	ep0Bi.Cnt = (byte) (bufferSize & 0xFF);
	ep0Bi.ADDR = PTR16(&controlTransferBuffer);

	// Update the number of bytes that still need to be sent.  Getting
	// all the data back to the host can take multiple transactions, so
	// we need to track how far along we are.
	wCount = wCount - bufferSize;

	// Move data to the USB output buffer from wherever it sits now.
	inPtr = (__data byte *) & controlTransferBuffer;

#if USE_MEMCPY
	memcpy(inPtr, outPtr, bufferSize);
#else
	for (i = 0; i < bufferSize; i++)
	{
#if DEBUG_PRINT
		// printf("0x%x = 0x%x (0x%uhx) ", PTR16(ramPtr), PTR16(romPtr), *romPtr);
#endif
		*inPtr++ = *outPtr++;
	}
#endif
}

// Data stage for a Control Transfer that reads data from the host
void
OutDataStage(void)
{
	word i, bufferSize;

	bufferSize = ((0x03 & ep0Bo.Stat) << 8) | ep0Bo.Cnt;

#if DEBUG_PRINT
	// printf("OutDataStage: %d\r\n", bufferSize);
#endif

	// Accumulate total number of bytes read
	wCount = wCount + bufferSize;

	outPtr = (__data byte *) & controlTransferBuffer;
#if USE_MEMCPY
	memcpy(inPtr, outPtr, bufferSize);
#else
	for (i = 0; i < bufferSize; i++)
	{
#if DEBUG_PRINT
		// printf("0x%x = 0x%x (0x%uhx) ", PTR16(inPtr), PTR16(outPtr), *outPtr);
#endif
		*inPtr++ = *outPtr++;
	}
#if DEBUG_PRINT
	// printf("\r\n");
#endif
#endif
}

// Process the Setup stage of a control transfer.  This code initializes the
// flags that let the firmware know what to do during subsequent stages of
// the transfer.
void
SetupStage(void)
{
	// Note: Microchip says to turn off the UOWN bit on the IN direction as
	// soon as possible after detecting that a SETUP has been received.
	ep0Bi.Stat &= ~UOWN;
	ep0Bo.Stat &= ~UOWN;

	// Initialize the transfer process
	ctrlTransferStage = SETUP_STAGE;
	requestHandled = 0;	// Default is that request hasn't been handled
	HIDPostProcess = 0;	// Assume standard request until know otherwise
	wCount = 0;	// No bytes transferred

	// See if this is a standard (as definded in USB chapter 9) request
	ProcessStandardRequest();

	// See if the HID class can do something with it.
	ProcessHIDRequest();

	// TBD: Add handlers for any other classes/interfaces in the device

	if (!requestHandled)
	{
		// If this service wasn't handled then stall endpoint 0
		ep0Bo.Cnt = E0SZ;
		ep0Bo.ADDR = PTR16(&SetupPacket);
		ep0Bo.Stat = UOWN | BSTALL;
		ep0Bi.Stat = UOWN | BSTALL;
	}
	else if (SetupPacket.bmRequestType & 0x80)
	{
		// Device-to-host
		if (SetupPacket.wLength < wCount)
			wCount = SetupPacket.wLength;
		InDataStage();
		ctrlTransferStage = DATA_IN_STAGE;
		// Reset the out buffer descriptor for endpoint 0
		ep0Bo.Cnt = E0SZ;
		ep0Bo.ADDR = PTR16(&SetupPacket);
		ep0Bo.Stat = UOWN;

		// Set the in buffer descriptor on endpoint 0 to send data
		ep0Bi.ADDR = PTR16(&controlTransferBuffer);
		// Give to SIE, DATA1 packet, enable data toggle checks
		ep0Bi.Stat = UOWN | DTS | DTSEN;
	}
	else
	{
		// Host-to-device
		ctrlTransferStage = DATA_OUT_STAGE;

		// Clear the input buffer descriptor
		ep0Bi.Cnt = 0;
		ep0Bi.Stat = UOWN | DTS | DTSEN;

		// Set the out buffer descriptor on endpoint 0 to receive data
		ep0Bo.Cnt = E0SZ;
		ep0Bo.ADDR = PTR16(&controlTransferBuffer);
		// Give to SIE, DATA1 packet, enable data toggle checks
		ep0Bo.Stat = UOWN | DTS | DTSEN;
	}

	// Enable SIE token and packet processing
	UCONbits.PKTDIS = 0;
}

// Configures the buffer descriptor for endpoint 0 so that it is waiting for
// the status stage of a control transfer.
void
WaitForSetupStage(void)
{
	ctrlTransferStage = SETUP_STAGE;
	ep0Bo.Cnt = E0SZ;
	ep0Bo.ADDR = PTR16(&SetupPacket);
	ep0Bo.Stat = UOWN | DTSEN;	// Give to SIE, enable data toggle checks
	ep0Bi.Stat = 0x00;	// Give control to CPU
}

// This is the starting point for processing a Control Transfer.  The code directly
// follows the sequence of transactions described in the USB spec chapter 5.  The
// only Control Pipe in this firmware is the Default Control Pipe (endpoint 0).
// Control messages that have a different destination will be discarded.
void
ProcessControlTransfer(void)
{
#if DEBUG_PRINT
	// printf("ProcessControlTransfer\r\n");
#endif
	if (USTAT == 0)
	{
		// Endpoint 0:out
		byte PID = (ep0Bo.Stat & 0x3C) >> 2;	// Pull PID from middle of BD0STAT
		if (PID == 0x0D)
			// SETUP PID - a transaction is starting
			SetupStage();
		else if (ctrlTransferStage == DATA_OUT_STAGE)
		{
			// Complete the data stage so that all information has
			// passed from host to device before servicing it.
			OutDataStage();

			// If this is a HID request, then invoke the callback to handle
			// the control out (when necessary).
			if (HIDPostProcess)
			{
				// Determine which report is being set.
				byte reportID = SetupPacket.wValue0;

				// Find out if an Output or Feature report has arrived on the control pipe.
				// Get the report type from the Setup packet.
				if (SetupPacket.wValue1 == 0x02)
				{
					// Output report
					SetOutputReport(reportID);
				}
				else if (SetupPacket.wValue1 == 0x03)
				{
					// Feature report
					SetFeatureReport(reportID);
				}
				else
				{
					// Unknown report type
				}
			}

			// Turn control over to the SIE and toggle the data bit
			if (ep0Bo.Stat & DTS)
				ep0Bo.Stat = UOWN | DTSEN;
			else
				ep0Bo.Stat = UOWN | DTS | DTSEN;
		}
		else
		{
			// Prepare for the Setup stage of a control transfer
			WaitForSetupStage();
		}
	}
	else if (USTAT == 0x04)
	{
		// Endpoint 0:in
		if ((UADDR == 0) && (deviceState == ADDRESS))
		{
			// TBD: ensure that the new address matches the value of
			// "deviceAddress" (which came in through a SET_ADDRESS).
			UADDR = SetupPacket.wValue0;
#if DEBUG_PRINT
			printf("UADDR = 0x%x\r\n", (word) UADDR);
#endif
			if (UADDR == 0)
				// If we get a reset after a SET_ADDRESS, then we need
				// to drop back to the Default state.
				deviceState = DEFAULT;
		}

		if (ctrlTransferStage == DATA_IN_STAGE)
		{
			// Start (or continue) transmitting data
			InDataStage();

			// Turn control over to the SIE and toggle the data bit
			if (ep0Bi.Stat & DTS)
				ep0Bi.Stat = UOWN | DTSEN;
			else
				ep0Bi.Stat = UOWN | DTS | DTSEN;
		}
		else
		{
			// Prepare for the Setup stage of a control transfer
			WaitForSetupStage();
		}
	}
	else
	{
#if DEBUG_PRINT
		printf("USTAT = 0x%uhx\r\n", USTAT);
#endif
	}
}


void
EnableUSBModule(void)
{
	// TBD: Check for voltage coming from the USB cable and use that
	// as an indication we are attached.
	if (UCONbits.USBEN == 0)
	{
#if DEBUG_PRINT
		printf("Enable the module\r\n");
#endif
		UCON = 0;
		UIE = 0;
		UCONbits.USBEN = 1;
		deviceState = ATTACHED;
	}

	// If we are attached and no single-ended zero is detected, then
	// we can move to the Powered state.
	if ((deviceState == ATTACHED) && !UCONbits.SE0)
	{
		UIR = 0;
		UIE = 0;
		UIEbits.URSTIE = 1;
		UIEbits.IDLEIE = 1;
		deviceState = POWERED;
#if DEBUG_PRINT
		printf("Device powered\r\n");
#endif
	}
}

// Unsuspend the device
void
UnSuspend(void)
{
#if DEBUG_PRINT
	printf("UnSuspend\r\n");
#endif

	UCONbits.SUSPND = 0;
	UIEbits.ACTVIE = 0;
	UIRbits.ACTVIF = 0;
}

// Full speed devices get a Start Of Frame (SOF) packet every 1 millisecond.
// Nothing is currently done with this interrupt (it is simply masked out).
void
StartOfFrame(void)
{
	// TBD: Add a callback routine to do something
	UIRbits.SOFIF = 0;
}

// This routine is called in response to the code stalling an endpoint.
void
Stall(void)
{
#if DEBUG_PRINT
	printf("Stall\r\n");
#endif
	if (UEP0bits.EPSTALL == 1)
	{
		// Prepare for the Setup stage of a control transfer
		WaitForSetupStage();
		UEP0bits.EPSTALL = 0;
	}
	UIRbits.STALLIF = 0;
}

// Suspend all processing until we detect activity on the USB bus
void
Suspend(void)
{
#if ALLOW_SUSPEND
#if DEBUG_PRINT
	printf("Suspend\r\n");
#endif
	UIEbits.ACTVIE = 1;
	UIRbits.IDLEIF = 0;
	UCONbits.SUSPND = 1;

	PIR2bits.USBIF = 0;
	INTCONbits.RBIF = 0;
	PIE2bits.USBIE = 1;
	INTCONbits.RBIE = 1;

	// disable the USART
	RCSTAbits.CREN = 0;
	TXSTAbits.TXEN = 0;

	Sleep();

	// enable the USART
	RCSTAbits.CREN = 1;
	TXSTAbits.TXEN = 1;

	PIE2bits.USBIE = 0;
	INTCONbits.RBIE = 0;
#endif
}

void
BusReset()
{
	UEIR = 0x00;
	UIR = 0x00;
	UEIE = 0x9f;
	UIE = 0x7b;
	UADDR = 0x00;

	// Set endpoint 0 as a control pipe
	UEP0 = 0x16;

	// Flush any pending transactions
	while (UIRbits.TRNIF == 1)
		UIRbits.TRNIF = 0;

	// Enable packet processing
	UCONbits.PKTDIS = 0;

	// Prepare for the Setup stage of a control transfer
	WaitForSetupStage();

	remoteWakeup = 0;	// Remote wakeup is off by default
	selfPowered = 0;	// Self powered is off by default
	currentConfiguration = 0;	// Clear active configuration
	deviceState = DEFAULT;
}

// Main entry point for USB tasks.  Checks interrupts, then checks for transactions.
void
ProcessUSBTransactions(void)
{
	// See if the device is connected yet.
	if (deviceState == DETACHED)
		return;

	// If the USB became active then wake up from suspend
	if (UIRbits.ACTVIF && UIEbits.ACTVIE)
		UnSuspend();

	// If we are supposed to be suspended, then don't try performing any
	// processing.
	if (UCONbits.SUSPND == 1)
		return;

	// Process a bus reset
	if (UIRbits.URSTIF && UIEbits.URSTIE)
	{
		BusReset();
	}

	if (UIRbits.IDLEIF && UIEbits.IDLEIE)
	{
		// No bus activity for a while - suspend the firmware
		Suspend();
	}
	if (UIRbits.SOFIF && UIEbits.SOFIE)
	{
		StartOfFrame();
	}
	if (UIRbits.STALLIF && UIEbits.STALLIE)
	{
		Stall();
	}

	if (UIRbits.UERRIF && UIEbits.UERRIE)
	{
		// TBD: See where the error came from.

		// Clear errors
		UIRbits.UERRIF = 0;
	}

	// Unless we have been reset by the host, no need to keep processing
	if (deviceState < DEFAULT)
		return;

	// A transaction has finished.  Try default processing on endpoint 0.
	if (UIRbits.TRNIF && UIEbits.TRNIE)
	{
		ProcessControlTransfer();

		// Turn off interrupt
		UIRbits.TRNIF = 0;
	}
}

#if 1
// RKP28sb
__code __at 0xF00000 char EEPROM[] = "RKP28sb";
#endif
