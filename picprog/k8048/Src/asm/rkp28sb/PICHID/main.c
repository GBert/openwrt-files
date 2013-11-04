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
#include "main.h"
#include "callbacks.h"

// Central processing loop.  Whenever the firmware isn't busy servicing
// the USB, we will get control here to do other processing.
int
ProcessIn(void)			// Receive
{
	byte rxCnt;

	// User Application USB tasks
	if ((deviceState < CONFIGURED) || (UCONbits.SUSPND == 1))
		return 0;

	// Find out if an Output report has been received from the host.
	rxCnt = HIDRxReport(rxBuffer, HID_OUTPUT_REPORT_BYTES);

	// If no bytes in, then nothing to do
	if (rxCnt == 0)
		return 0;

	// Flag that work is to be done in caller...
	return 1;
}
void
ProcessOut(void)		// Transmit
{
	// As long as the SIE is owned by the processor, we let USB tasks continue.
	while (ep1Bi.Stat & UOWN)
		ProcessUSBTransactions();

	// The report will be sent in the next interrupt IN transfer.
	HIDTxReport(txBuffer, HID_INPUT_REPORT_BYTES);
}

// Handle a control input report
int
GetInputReport(byte reportID)
{
	// Currently only handling report 0, ignore any others.
	if (reportID != 0)
		return 0;

	// Send back the contents of the HID report
	// TBD: provide useful information...
	outPtr = (__data byte *) & HIDTxBuffer;

	// The number of bytes in the report (from usb.h).
	wCount = HID_INPUT_REPORT_BYTES;

	return 1;
}

// Handle control out.  This might be an alternate way of processing
// an output report, so all that's needed is to point the output
// pointer to the output buffer
// Initialization for a SET_REPORT request.  This routine will be
// invoked during the setup stage and is used to set up the buffer
// for receiving data from the host
int
SetupOutputReport(byte reportID)
{
	// Currently only handling report 0, ignore any others.
	if (reportID != 0)
		return 0;

	// When the report arrives in the data stage, the data will be  
	// stored in the feature buffer
	inPtr = (__data byte *) & HIDRxBuffer;

	return 1;
}

// Post processing for a SET_REPORT request.  After all the data has
// been delivered from host to device, this will be invoked to perform
// application specific processing.
int
SetOutputReport(byte reportID)
{
	// Currently only handling report 0, ignore any others.
	if (reportID != 0)
		return 0;

	// TBD: do something.  Not currently implemented because the output
	// report is being handled by an interrupt endpoint.
	return 1;
}

// Handle a feature report request on the control pipe
int
GetFeatureReport(byte reportID)
{
	// Currently only handling report 0, ignore any others.
	if (reportID != 0)
		return 0;

	// Handle report #0
	outPtr = (__data byte *) & HIDFeatureBuffer;

	// The number of bytes in the report (from usb.h).
	wCount = HID_FEATURE_REPORT_BYTES;

	// XXX FEATURE OUTPUT TO HOST XXX
	{
		// DO NOTHING YET
	}
	return 1;
}

// Initialization for a SET_FEATURE request.  This routine will be
// invoked during the setup stage and is used to set up the buffer
// for receiving data from the host
int
SetupFeatureReport(byte reportID)
{
	// Currently only handling report 0, ignore any others.
	if (reportID != 0)
		return 0;

	// When the report arrives in the data stage, the data will be  
	// stored in the feature buffer.
	inPtr = (__data byte *) & HIDFeatureBuffer;

	return 1;
}

// Post processing for a SET_FEATURE request.  After all the data has
// been delivered from host to device, this will be invoked to perform
// application specific processing.
int
SetFeatureReport(byte reportID)
{
	// Currently only handling report 0, ignore any others.
	if (reportID != 0)
		return 0;

	// Haven't read the entire packet yet.  Keep waiting...
	if (wCount != HID_FEATURE_REPORT_BYTES)
		return 1;

	// XXX FEATURE INPUT FROM HOST XXX
	{
		// DO NOTHING YET
	}
	return 1;
}
