/*
 * Copyright (C) 2005-2014 Darron Broad
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name `Darron Broad' nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "k8048.h"

/*****************************************************************************
 *
 * Hardware operations
 *
 *****************************************************************************/

struct pic_ops pic32_ops = {
	.arch			   = ARCH32BIT,
	.selector		   = pic32_selector,
	.read_config_memory	   = pic32_read_config_memory,
	.get_program_size	   = pic32_get_program_size,
	.get_data_size		   = NULL,
	.get_executive_size	   = NULL,
	.get_boot_size             = pic32_get_boot_size,
	.read_program_memory_block = pic32_read_program_memory_block,
	.read_data_memory_block    = NULL,
	.write_panel               = pic32_write_panel,
	.program		   = pic32_program,
	.verify			   = pic32_verify,
	.bulk_erase		   = pic32_bulk_erase,
	.row_erase		   = NULL,
	.dumpdeviceid		   = pic32_dumpdeviceid,
	.dumpconfig		   = pic32_dumpconfig,
	.dumposccal		   = NULL,
	.dumpdevice                = NULL,
	.dumphexcode               = pic32_dumphexcode,
	.dumpinhxcode              = pic32_dumpinhxcode,
	.dumphexdata               = NULL,
	.dumpinhxdata              = NULL,
};

uint32_t
pic32_arch(struct k8048 *k)
{
	k->pic = &pic32_ops;

	return k->pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic32_config pic32_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic32_dsmap pic32_map[] =
{
/*Device name		Device id	 App. Data-Sheet	BOOT/PROG Size	Row/Page Size*/

/* PIC32MX1XX/2XX DS60001168F */
{"PIC32MX110F016B",	PIC32MX110F016B, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX110F016C",	PIC32MX110F016C, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX110F016D",	PIC32MX110F016D, DS60001168F,		768, 4096,	32, 256},

{"PIC32MX120F032B",	PIC32MX120F032B, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX120F032C",	PIC32MX120F032C, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX120F032D",	PIC32MX120F032D, DS60001168F,		768, 8192,	32, 256},

{"PIC32MX130F064B",	PIC32MX130F064B, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX130F064C",	PIC32MX130F064C, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX130F064D",	PIC32MX130F064D, DS60001168F,		768, 16384,	32, 256},

{"PIC32MX150F128B",	PIC32MX150F128B, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX150F128C",	PIC32MX150F128C, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX150F128D",	PIC32MX150F128D, DS60001168F,		768, 32768,	32, 256},

{"PIC32MX170F256B",	PIC32MX170F256B, DS60001168F,		768, 65536,	32, 256},
{"PIC32MX170F256D",	PIC32MX170F256D, DS60001168F,		768, 65536,	32, 256},

{"PIC32MX210F016B",	PIC32MX210F016B, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX210F016C",	PIC32MX210F016C, DS60001168F,		768, 4096,	32, 256},
{"PIC32MX210F016D",	PIC32MX210F016D, DS60001168F,		768, 4096,	32, 256},

{"PIC32MX220F032B",	PIC32MX220F032B, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX220F032C",	PIC32MX220F032C, DS60001168F,		768, 8192,	32, 256},
{"PIC32MX220F032D",	PIC32MX220F032D, DS60001168F,		768, 8192,	32, 256},

{"PIC32MX230F064B",	PIC32MX230F064B, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX230F064C",	PIC32MX230F064C, DS60001168F,		768, 16384,	32, 256},
{"PIC32MX230F064D",	PIC32MX230F064D, DS60001168F,		768, 16384,	32, 256},

{"PIC32MX250F128B",	PIC32MX250F128B, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX250F128C",	PIC32MX250F128C, DS60001168F,		768, 32768,	32, 256},
{"PIC32MX250F128D",	PIC32MX250F128D, DS60001168F,		768, 32768,	32, 256},

{"PIC32MX270F256B",	PIC32MX270F256B, DS60001168F,		768, 65536,	32, 256},
{"PIC32MX270F256D",	PIC32MX270F256D, DS60001168F,		768, 65536,	32, 256},

/* PIC32MX330/350/370/430/450/470 DS60001185C */
{"PIC32MX330F064H",	PIC32MX330F064H, DS60001185C,		3072, 16384,	128, 1024},
{"PIC32MX330F064L",	PIC32MX330F064H, DS60001185C,		3072, 16384,	128, 1024},

{"PIC32MX350F128H",	PIC32MX350F128H, DS60001185C,		3072, 32768,	128, 1024},
{"PIC32MX350F128L",	PIC32MX350F128H, DS60001185C,		3072, 32768,	128, 1024},

{"PIC32MX350F256H",	PIC32MX350F256H, DS60001185C,		3072, 65536,	128, 1024},
{"PIC32MX350F256L",	PIC32MX350F256H, DS60001185C,		3072, 65536,	128, 1024},

{"PIC32MX370F512H",	PIC32MX370F512H, DS60001185C,		3072, 131072,	128, 1024},
{"PIC32MX370F512L",	PIC32MX370F512H, DS60001185C,		3072, 131072,	128, 1024},

{"PIC32MX430F064H",	PIC32MX430F064H, DS60001185C,		3072, 16384,	128, 1024},
{"PIC32MX430F064L",	PIC32MX430F064H, DS60001185C,		3072, 16384,	128, 1024},

{"PIC32MX450F128H",	PIC32MX450F128H, DS60001185C,		3072, 32768,	128, 1024},
{"PIC32MX450F128L",	PIC32MX450F128H, DS60001185C,		3072, 32768,	128, 1024},

{"PIC32MX450F256H",	PIC32MX450F256H, DS60001185C,		3072, 65536,	128, 1024},
{"PIC32MX450F256L",	PIC32MX450F256H, DS60001185C,		3072, 65536,	128, 1024},

{"PIC32MX470F512H",	PIC32MX470F512H, DS60001185C,		3072, 131072,	128, 1024},
{"PIC32MX470F512L",	PIC32MX470F512H, DS60001185C,		3072, 131072,	128, 1024},

/* PIC32MX3XX/4XX DS61143H */
{"PIC32MX320F032H",	PIC32MX320F032H, DS61143H,		3072, 8192,	128, 1024},
{"PIC32MX320F064H",	PIC32MX320F064H, DS61143H,		3072, 16384,	128, 1024},
{"PIC32MX320F128H",	PIC32MX320F128H, DS61143H,		3072, 32768,	128, 1024},

{"PIC32MX340F128H",	PIC32MX340F128H, DS61143H,		3072, 32768,	128, 1024},
{"PIC32MX340F256H",	PIC32MX340F256H, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX340F512H",	PIC32MX340F512H, DS61143H,		3072, 131072,	128, 1024},

{"PIC32MX320F128L",	PIC32MX320F128L, DS61143H,		3072, 32768,	128, 1024},
{"PIC32MX340F128L",	PIC32MX340F128L, DS61143H,		3072, 32768,	128, 1024},

{"PIC32MX360F256L",	PIC32MX360F256L, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX360F512L",	PIC32MX360F512L, DS61143H,		3072, 131072,	128, 1024},

{"PIC32MX420F032H",	PIC32MX420F032H, DS61143H,		3072, 8192,	128, 1024},

{"PIC32MX440F128H",	PIC32MX440F128H, DS61143H,		3072, 32768,	128, 1024},
{"PIC32MX440F256H",	PIC32MX440F256H, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX440F512H",	PIC32MX440F512H, DS61143H,		3072, 131072,	128, 1024},
{"PIC32MX440F128L",	PIC32MX440F128L, DS61143H,		3072, 32768,	128, 1024},

{"PIC32MX460F256L",	PIC32MX460F256L, DS61143H,		3072, 65536,	128, 1024},
{"PIC32MX460F512L",	PIC32MX460F512L, DS61143H,		3072, 131072,	128, 1024},

/* PIC32MX5XX/6XX/7XX DS61156H */
{"PIC32MX534F064H",	PIC32MX534F064H, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F064H",	PIC32MX564F064H, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F128H",	PIC32MX564F128H, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX575F256H",	PIC32MX575F256H, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX575F512H",	PIC32MX575F512H, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX534F064L",	PIC32MX534F064L, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F064L",	PIC32MX564F064L, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX564F128L",	PIC32MX564F128L, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX575F256L",	PIC32MX575F256L, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX575F512L",	PIC32MX575F512L, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX664F064H",	PIC32MX664F064H, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX664F128H",	PIC32MX664F128H, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX675F256H",	PIC32MX675F256H, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX675F512H",	PIC32MX675F512H, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX695F512H",	PIC32MX695F512H, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX664F064L",	PIC32MX664F064L, DS61156H,		3072, 16384,	128, 1024},
{"PIC32MX664F128L",	PIC32MX664F128L, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX675F256L",	PIC32MX675F256L, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX675F512L",	PIC32MX675F512L, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX695F512L",	PIC32MX695F512L, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX764F128H",	PIC32MX764F128H, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX775F256H",	PIC32MX775F256H, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX775F512H",	PIC32MX775F512H, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX795F512H",	PIC32MX795F512H, DS61156H,		3072, 131072,	128, 1024},

{"PIC32MX764F128L",	PIC32MX764F128L, DS61156H,		3072, 32768,	128, 1024},
{"PIC32MX775F256L",	PIC32MX775F256L, DS61156H,		3072, 65536,	128, 1024},
{"PIC32MX775F512L",	PIC32MX775F512L, DS61156H,		3072, 131072,	128, 1024},
{"PIC32MX795F512L",	PIC32MX795F512L, DS61156H,		3072, 131072,	128, 1024},

/* PIC32MZ EC DS60001191C */
{"PIC32MZ1024ECG064", PIC32MZ1024ECG064, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECG100", PIC32MZ1024ECG100, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECG124", PIC32MZ1024ECG124, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECG144", PIC32MZ1024ECG144, DS60001191C,		40960, 262144,	128, 1024},

{"PIC32MZ1024ECH064", PIC32MZ1024ECH064, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECH100", PIC32MZ1024ECH100, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECH124", PIC32MZ1024ECH124, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECH144", PIC32MZ1024ECH144, DS60001191C,		40960, 262144,	128, 1024},

{"PIC32MZ2048ECG064", PIC32MZ2048ECG064, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECG100", PIC32MZ2048ECG100, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECG124", PIC32MZ2048ECG124, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECG144", PIC32MZ2048ECG144, DS60001191C,		40960, 524288,	128, 1024},

{"PIC32MZ2048ECH064", PIC32MZ2048ECH064, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECH100", PIC32MZ2048ECH100, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECH124", PIC32MZ2048ECH124, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECH144", PIC32MZ2048ECH144, DS60001191C,		40960, 524288,	128, 1024},

{"PIC32MZ1024ECM064", PIC32MZ1024ECM064, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECM100", PIC32MZ1024ECM100, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECM124", PIC32MZ1024ECM124, DS60001191C,		40960, 262144,	128, 1024},
{"PIC32MZ1024ECM144", PIC32MZ1024ECM144, DS60001191C,		40960, 262144,	128, 1024},

{"PIC32MZ2048ECM064", PIC32MZ2048ECM064, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECM100", PIC32MZ2048ECM100, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECM124", PIC32MZ2048ECM124, DS60001191C,		40960, 524288,	128, 1024},
{"PIC32MZ2048ECM144", PIC32MZ2048ECM144, DS60001191C,		40960, 524288,	128, 1024},

{"(null)",		0,		 0,			0, 0,		0, 0},
/*Device name		Device id	 App. Data-Sheet	BOOT/PROG Size	Row/Page Size*/
};
#define PIC32_SIZE ((sizeof(pic32_map) / sizeof(struct pic32_dsmap)) - 1)

/* Default device (null) */
uint32_t pic32_index = PIC32_SIZE;

void
pic32_selector(void)
{
	uint32_t i;
	char *dnames[PIC32_SIZE];

	for (i = 0; i < PIC32_SIZE; ++i) {
		dnames[i] = pic32_map[i].devicename;
	}
	qsort(dnames, PIC32_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC32_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%-19s", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %u\n", (uint32_t)PIC32_SIZE);
}

/*****************************************************************************
 *
 * Program/Verify mode
 *
 *****************************************************************************/

/*
 * ENTER LVP PROGRAM/VERIFY MODE
 *
 * ENTRY - VDD FIRST
 */
void
pic32_program_verify(struct k8048 *k)
{
	/* RESET & ACQUIRE GPIO */
	io_set_vpp(k, LOW);
	/* DS60001145M P6(100ns) */
	io_usleep(k, 1000);
	/* PGD + PGC + PGM(N/A) LOW */
	io_set_pgd(k, LOW);
	io_set_pgc(k, LOW);
	io_set_pgm(k, LOW);
	io_usleep(k, 1000);

	/* LVP(KEY) */
	if (k->key == LVPKEY) {
		/* VPP HIGH */
		io_set_vpp(k, HIGH);
		/* DS60001145M P20(500us) */
		io_usleep(k, 500);

		/* VPP LOW */
		io_set_vpp(k, LOW);
		/* DS60001145M P18(40ns) */
		io_usleep(k, 500);

		/* PROGRAM/VERIFY ENTRY CODE */
		io_program_out(k, PHCMKEY, 32);
		/* DS60001145M P19(40ns) */
		io_usleep(k, 500);

		/* VPP HIGH */
		io_set_vpp(k, HIGH);
		/* DS60001145M P7(500ns) */
		io_usleep(k, 500);

		/* ENTER RUN-TEST/IDLE STATE */
		pic32_setmode(k, PIC32_IDLE6);
	}
	/* UNSUPPORTED */
	else {
		printf("%s: fatal error: unsupported programming mode\n", __func__);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
}

/*
 * EXIT LVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 */
void
pic32_standby(struct k8048 *k)
{
	/* ENTER TEST-LOGIC-RESET STATE */
	pic32_setmode(k, PIC32_RESET);
	/* DS60001145M P16(0s) */

	/* PGD + PGC + VPP + PGM(N/A) LOW */
	io_set_pgd(k, LOW);
	io_set_pgc(k, LOW);
	io_set_vpp(k, LOW);
	io_set_pgm(k, LOW);
}

/*****************************************************************************
 *
 * Pseudo Operations
 *
 *****************************************************************************/

/*
 * SetMode Pseudo Operation
 *
 *  To set the EJTAG state machine to a specific state
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 13
 */
void
pic32_setmode(struct k8048 *k, uint8_t nbits, uint32_t tms)
{
	/* SET MODE */
	io_clock_bits_4phase(k, nbits, tms, /* tdi */ 0);
}

/*
 * SendCommand Pseudo Operation
 *
 *  To send a command to select a specific TAP register
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 14
 */
void
pic32_sendcommand(struct k8048 *k, uint8_t nbits, uint32_t tdi)
{
	/*
	 * 1. The TMS Header is clocked into the device to select the
	 *    Shift IR state.
	 */

	/* ENTER SHIFT-IR STATE */
	io_clock_bit_4phase(k, 1, 0);		/* SELECT-DR		*/
	io_clock_bit_4phase(k, 1, 0);		/* SELECT-IR		*/
	io_clock_bit_4phase(k, 0, 0);		/* CAPTURE-IR		*/
	io_clock_bit_4phase(k, 0, 0);		/* SHIFT-IR		*/

	/*
	 * 2. The command is clocked into the device on TDI while
	 *    holding signal TMS low.
	 *
	 * 3. The last Most Significant bit (MSb) of the command is
	 *    clocked in while setting TMS high.
	 */

	/* SEND COMMAND */
	io_clock_bits_4phase(k, nbits,  	/* SHIFT-IR .. EXIT1-IR	*/
						1 << (nbits - 1), tdi);
	/*
	 * 4. The TMS Footer is clocked in on TMS to return the TAP
	 *    controller to the Run/Test Idle state.
	 */

	/* ENTER RUN-TEST/IDLE STATE */ 
	io_clock_bit_4phase(k, 1, 0);		/* UPDATE-IR		*/
	io_clock_bit_4phase(k, 0, 0);		/* RUN-TEST/IDLE 	*/
}

/*
 * XferData Pseudo Operation
 *
 *  To clock data to and from the register selected by the command
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 15
 */
uint32_t
pic32_xferdata(struct k8048 *k, uint8_t nbits, uint32_t tdi)
{
	uint32_t tdo;

	/*
	 * 1. The TMS Header is clocked into the device to select the
	 *    Shift DR state.
	 */

	/* ENTER SHIFT-DR STATE */
	io_clock_bit_4phase(k, 1, 0);		/* SELECT-DR		*/
	io_clock_bit_4phase(k, 0, 0);		/* CAPTURE-DR		*/
	tdo = io_clock_bit_4phase(k, 0, 0);	/* SHIFT-DR		*/

	/*
	 * 2. The data is clocked in/out of the device on TDI/TDO while
	 *    holding signal TMS low.
	 *
	 * 3. The last MSb of the data is clocked in/out while setting
	 *    TMS high.
	 */

	/* TRANSFER DATA */
	tdo |= io_clock_bits_4phase(k, nbits,	/* SHIFT-DR .. EXIT1-DR */
						1 << (nbits - 1), tdi) << 1;
	/*
	 * 4. The TMS Footer is clocked in on TMS to return the TAP
	 *    controller to the Run/Test Idle state.
	 */

	/* ENTER RUN-TEST/IDLE STATE */ 
	io_clock_bit_4phase(k, 1, 0);		/* UPDATE-DR		*/
	io_clock_bit_4phase(k, 0, 0);		/* RUN-TEST/IDLE	*/

	return tdo;
}

/*
 * XferFastData Pseudo Operation
 *
 *  To quickly send 32 bits of data in/out of the device
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 16
 */
uint32_t
pic32_xferfastdata(struct k8048 *k, uint32_t tdi)
{
	uint8_t pracc;
	uint32_t tdo;

	/*
	 * 1. The TMS Header is clocked into the device to select the
	 *    Shift DR state.
	 */

	/* ENTER SHIFT-DR STATE */
	io_clock_bit_4phase(k, 1, 0);		/* SELECT-DR 		*/
	io_clock_bit_4phase(k, 0, 0);		/* CAPTURE-DR		*/

	pracc = io_clock_bit_4phase(k, 0, 0);	/* SHIFT-DR		*/
	if (!pracc) {
		printf("%s: fatal error: processor access invalid\n", __func__);
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}

	/*
	 * 2. The input value of the PrAcc bit, which is ‘0’, is clocked
	 *    in.
	 */

	/* PROCESSOR ACCESS */
	tdo = io_clock_bit_4phase(k, 0, 0);	/* SHIFT-DR		*/

	/* TRANSFER DATA */
	tdo |= io_clock_bits_4phase(k, 32,	/* SHIFT-DR .. EXIT1-DR */
						0x80000000, tdi) << 1;
	/*
	 * 3. TMS Footer = 10 is clocked in to return the TAP controller
	 *    to the Run/Test Idle state.
	 */

	/* ENTER RUN-TEST/IDLE STATE */ 
	io_clock_bit_4phase(k, 1, 0);		/* UPDATE-DR		*/
	io_clock_bit_4phase(k, 0, 0);		/* RUN-TEST/IDLE	*/

	return tdo;
}

/*
 * XferInstruction Pseudo Operation
 *
 *  To send 32 bits of data for the device to execute
 *
 * 2-WIRE 4-PHASE
 *
 * DS60001145M-page 18
 */
void
pic32_xferinstruction(struct k8048 *k, uint32_t instruction)
{
	uint32_t controlVal;
	struct timeval tv1, tv2, tv3;

	/* Select control register */
	pic32_sendcommand(k, PIC32_ETAP_CONTROL);

	/* Wait until CPU is ready */
	gettimeofday(&tv1, NULL);
	do {
	 	/* Check if processor access bit is set */
		controlVal = pic32_xferdata(k, 32, PIC32_EJTAG_CONTROL_PROBEN |
			PIC32_EJTAG_CONTROL_PROBTRAP | PIC32_EJTAG_CONTROL_PRACC);
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while (!(controlVal & PIC32_EJTAG_CONTROL_PRACC) && (tv3.tv_sec < PIC_TIMEOUT));

	if (!(controlVal & PIC32_EJTAG_CONTROL_PRACC)) {
		printf("%s: fatal error: instruction transfer failed\n", __func__);
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}

	/* Select data register */
	pic32_sendcommand(k, PIC32_ETAP_DATA);

	/* Send the instruction */
	pic32_xferdata(k, 32, instruction);

	/* Select control register */
	pic32_sendcommand(k, PIC32_ETAP_CONTROL);

	/* Tell CPU to execute instruction */
	pic32_xferdata(k, 32, PIC32_EJTAG_CONTROL_PROBEN | PIC32_EJTAG_CONTROL_PROBTRAP);
}

/*****************************************************************************
 *
 * Memory Operations
 *
 *****************************************************************************/

/*
 * Kernel KSEG1 address to physical address
 *
 * DS61115F-page 3-15
 */
static inline uint32_t
pic32_phy(uint32_t addr)
{
	return addr & 0x1FFFFFFF;
}

/*
 * Physical address to kernel KSEG1 address
 *
 * DS61115F-page 3-15
 */
static inline uint32_t
pic32_kseg1(uint32_t addr)
{
	return addr | 0xA0000000;
}

/*****************************************************************************
 *
 * Utility Operations
 *
 *****************************************************************************/

/*
 * Check device status
 *
 * DS60001145M-page 20
 */
uint32_t
pic32_check_device_status(struct k8048 *k)
{
	pic32_sendcommand(k, PIC32_MTAP_SW_MTAP);
	pic32_sendcommand(k, PIC32_MTAP_COMMAND);

	return pic32_xferdata(k, PIC32_MCHP_STATUS);
}

/*
 * Erase Device
 *
 * DS60001145M-page 21
 */
void
pic32_erase_device(struct k8048 *k)
{
	uint32_t statusVal;
	struct timeval tv1, tv2, tv3;

	/*
	 * 1. SendCommand (MTAP_SW_MTAP).
	 *
	 * 2. SendCommand (MTAP_COMMAND).
	 *
	 * 3. XferData (MCHP_ERASE).
	 */

	pic32_sendcommand(k, PIC32_MTAP_SW_MTAP);
	pic32_sendcommand(k, PIC32_MTAP_COMMAND);
	pic32_xferdata(k, PIC32_MCHP_ERASE);

	/*
	 * 4. XferData (MCHP_DE_ASSERT_RST), PIC32MZ devices only.
	 */

	if (0) {
		pic32_xferdata(k, PIC32_MCHP_DE_ASSERT_RST);
	}

	/*
	 * 5. Delay 10 ms.
	 *
	 * 6. statusVal = XferData (MCHP_STATUS).
	 *
	 * 7. If CFGRDY (statusVal<3>) is not ‘1’ and FCBUSY (statusVal<2>) is not ‘0’, GOTO to step 5.
	 */

	gettimeofday(&tv1, NULL);
	do {
		io_usleep(k, 10000); /* 10ms */
		statusVal = pic32_xferdata(k, PIC32_MCHP_STATUS) &
			(PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FCBUSY);
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);
	}
	while ((statusVal != PIC32_MCHP_STATUS_CFGRDY) && (tv3.tv_sec < PIC_TIMEOUT));

	if (statusVal != PIC32_MCHP_STATUS_CFGRDY) {
		printf("%s: fatal error: erase device failed\n", __func__);
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}
}

/*
 * Enter Serial Execution Mode
 *
 * DS60001145M-page 22
 */
void
pic32_enter_serial_execution_mode(struct k8048 *k)
{
	uint32_t statusVal;

	statusVal = pic32_check_device_status(k);
	if (statusVal != (PIC32_MCHP_STATUS_CPS | PIC32_MCHP_STATUS_CFGRDY)) {
		printf("%s: fatal error: status invalid [0x%02X]\n", __func__, statusVal);
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}

	pic32_xferdata(k, PIC32_MCHP_ASSERT_RST);

	pic32_sendcommand(k, PIC32_MTAP_SW_ETAP);
	pic32_sendcommand(k, PIC32_ETAP_EJTAGBOOT);

	pic32_sendcommand(k, PIC32_MTAP_SW_MTAP);
	pic32_sendcommand(k, PIC32_MTAP_COMMAND);

	pic32_xferdata(k, PIC32_MCHP_DE_ASSERT_RST);

	/*
	 * Flash Enable, PIC32MX1XX/2XX devices only
	 */

	if (pic32_map[pic32_index].datasheet == DS60001168F) {
		pic32_xferdata(k, PIC32_MCHP_FLASH_ENABLE);
		statusVal = pic32_xferdata(k, PIC32_MCHP_STATUS);
		if (statusVal != (PIC32_MCHP_STATUS_CPS | PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FAEN)) {
			printf("%s: fatal error: status invalid [0x%02X]\n", __func__, statusVal);
			pic32_standby(k);
			io_exit(k, EX_SOFTWARE); /* Panic */
		}
	}

	pic32_sendcommand(k, PIC32_MTAP_SW_ETAP);
}

/*
 * Download Data Block
 *
 * DS60001145M-page 25
 */
void
pic32_download_data_block(struct k8048 *k, uint32_t *panel, uint32_t panel_size)
{
	/*
	 * 1. Initialize SRAM Base Address to 0xA0000000.
	 */

	/* LUI S0, 0xA000	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(k, 0x3C10A000);

	/*
	 * 2. Write the entire row of data to be programmed into system SRAM.
	 *
	 * 3. Repeat Step 2 until one row of data has been loaded.
	 */

	uint32_t offset = 0;
	uint16_t datal, datah;
	do {
		/* Get low and high word data */
		datal = panel[offset + 0] | panel[offset + 1] << 8;
		datah = panel[offset + 2] | panel[offset + 3] << 8;

		/* LUI T0, <Data31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
		pic32_xferinstruction(k, 0x3C080000 + datah);

		/* ORI T0, <Data15:0>	OR LOWER IMMEDIATE */
		pic32_xferinstruction(k, 0x35080000 + datal);

		/* SW T0, <OFFSET>(S0)	STORE WORD */
		pic32_xferinstruction(k, 0xAE080000 + offset);

		/* OFFSET increments by 4 */
		offset += 4;
	}
	while (offset < panel_size);
}

/*
 * Flash Row Write
 *
 * DS60001145M-page 26
 */
void
pic32_flash_row_write(struct k8048 *k, uint32_t address, uint16_t ramaddr)
{
	/*
	 * 1. All PIC32 devices: Initialize constants.
	 *    
	 *    Registers a1, a2, and a3 are set for WREN = 1 or NVMOP<3:0> = 0011,
	 *    WR = 1 and WREN = 1, respectively.
	 *
	 *    Registers s1 and s2 are set for the unlock data values and S0 is
	 *    initialized to ‘0’.
	 */

	/* NVMOP = 0011 = WRITE ROW */
	pic32_xferinstruction(k, 0x34054003); /* ori a1,$0,0x4003 */
	pic32_xferinstruction(k, 0x34068000); /* ori a2,$0,0x8000 */
	pic32_xferinstruction(k, 0x34074000); /* ori a3,$0,0x4000 */

	pic32_xferinstruction(k, 0x3c11aa99); /* lui s1,0xaa99    */
	pic32_xferinstruction(k, 0x36316655); /* ori s1,s1,0x6655 */
	pic32_xferinstruction(k, 0x3c125566); /* lui s2,0x5566    */
	pic32_xferinstruction(k, 0x365299aa); /* ori s2,s2,0x99aa */

	pic32_xferinstruction(k, 0x3c100000); /* lui s0,0x0000    */

	/*
	 * 2. PIC32MX devices only: Set register a0 to the base address of the NVM
	 *    register (0xBF80_F400).
	 */

	if (1) {
		pic32_xferinstruction(k, 0x3c04bf80); /* lui a0,0xbf80	  */
		pic32_xferinstruction(k, 0x3484f400); /* ori a0,a0,0xf400 */
	}

	/*
	 * 2. PIC32MZ EC devices only: Set register a0 to the base address of the
	 *    NVM register (0xBF80_0600).
	 *
	 *    Register s3 is set for the value used to disable write protection in
	 *    NVMBPB.
	 *
	 * 3. PIC32MZ EC devices only: Unlock and disable Boot Flash write
	 *    protection.
	 */

	else {
		pic32_xferinstruction(k, 0x3c04bf80); /* lui a0,0xbf80    */
		pic32_xferinstruction(k, 0x34840600); /* ori a0,a0,0x0600 */

		pic32_xferinstruction(k, 0x34138080); /* ori s3,$0,0x8080 */

		pic32_xferinstruction(k, 0xac910010); /* sw s1,16(a0)     */
		pic32_xferinstruction(k, 0xac920010); /* sw s2,16(a0)     */
		pic32_xferinstruction(k, 0xac930090); /* sw s3,144(a0)    */

		pic32_xferinstruction(k, 0x00000000); /* nop              */
	}

	/*
	 * 4. All PIC32 devices: Set the NVMADDR register with the address of the
	 *    Flash row to be programmed.
	 */

	uint32_t addrh = pic32_phy(address) >> 16, addrl = pic32_phy(address) & 0xFFFF;

	pic32_xferinstruction(k, 0x3c080000 + addrh); /* lui t0,<FLASH_ROW_ADDR(31:16)>   */
	pic32_xferinstruction(k, 0x35080000 + addrl); /* ori t0,t0,<FLASH_ROW_ADDR(15:0)> */
	pic32_xferinstruction(k, 0xac880020);         /* sw t0,32(a0)                     */

	/*
	 * 5. PIC32MX devices only: Set the NVMSRCADDR register with the physical
	 *    source SRAM address (offset is 64).
	 */

	if (1) {

		pic32_xferinstruction(k, 0x36100000 + ramaddr); /* ori s0,s0,<RAM_ADDR(15:0)> */
		pic32_xferinstruction(k, 0xac900040);           /* sw s0,64(a0)               */
	}

	/*
	 * 5. PIC32MZ EC devices only: Set the NVMSRCADDR register with the
	 *    physical source SRAM address (offset is 112).
	 */

	else {
		pic32_xferinstruction(k, 0x36100000 + ramaddr); /* ori s0,s0,<RAM_ADDR(15:0)> */
		pic32_xferinstruction(k, 0xac900070);           /* sw s0,112(a0)              */
	}

	/*
	 * 6. All PIC32 devices: Set up the NVMCON register for write operation.
	 */

	pic32_xferinstruction(k, 0xac850000); /* sw a1,0(a0) */

	/*
	 * 7. PIC32MX devices only: Poll the LVDSTAT register.
	 */

	if (1) {
		/* here1: */
		pic32_xferinstruction(k, 0x8C880000); /* lw t0,0(a0)       */
		pic32_xferinstruction(k, 0x31080800); /* andi t0,t0,0x0800 */
		pic32_xferinstruction(k, 0x1500fffd); /* bne t0,$0,here1   */
		pic32_xferinstruction(k, 0x00000000); /* nop               */
	}

	/*
	 * 8. All PIC32 devices: Unlock the NVMCON register and start the write
	 *    operation.
	 */

	pic32_xferinstruction(k, 0xac910010); /* sw s1,16(a0) */
	pic32_xferinstruction(k, 0xac920010); /* sw s2,16(a0) */
	pic32_xferinstruction(k, 0xac860008); /* sw a2,8(a0)  */

	/*
	 * 9. All PIC32 devices: Loop until the WR bit (NVMCON<15>) is clear.
	 */

	/* here2: */
	pic32_xferinstruction(k, 0x8c880000); /* lw t0,0(a0)     */
	pic32_xferinstruction(k, 0x01064024); /* and t0,t0,a2    */
	pic32_xferinstruction(k, 0x1500fffd); /* bne t0,$0,here2 */
	pic32_xferinstruction(k, 0x00000000); /* nop             */

	/*
	 * 10. All PIC32 devices: Wait at least 500 ns after seeing a ‘0’ in
	 *     the WR bit (NVMCON<15>) before writing to any of the NVM registers.
	 *     This requires inserting a NOP in the execution. The following example
	 *     assumes that the core is executing at 8 MHz; therefore, four NOP
	 *     instructions equate to 500 ns.
	 */

	pic32_xferinstruction(k, 0x00000000); /* nop */
	pic32_xferinstruction(k, 0x00000000); /* nop */
	pic32_xferinstruction(k, 0x00000000); /* nop */
	pic32_xferinstruction(k, 0x00000000); /* nop */

	/*
	 * 11. All PIC32 devices: Clear the WREN bit (NVMCONM<14>).
	 */

	pic32_xferinstruction(k, 0xac870004); /* sw a3,4(a0) */
#if 0
	/*
	 * 12. All PIC32 devices: Check the WRERR bit (NVMCON<13>) to ensure that
	 *     the program sequence has completed successfully. If an error occurs,
	 *     jump to the error processing routine.
	 */

	pic32_xferinstruction(k, 0x8c880000);          /* lw t0,0(a0)                   */
	pic32_xferinstruction(k, 0x30082000);          /* andi t0,zero,0x2000           */
	pic32_xferinstruction(k, 0x15000000 + offset); /* bne t0, $0, <err_proc_offset> */
	pic32_xferinstruction(k, 0x00000000);          /* nop */
#endif
}

/*
 * Read Memory Location
 *
 * DS60001145M-page 28
 */
uint32_t
pic32_readmemorylocation(struct k8048 *k, uint32_t addr)
{
	uint32_t data;

	/*
	 * 1. Initialize some constants
	 */

	/* LUI S3, 0xFF20 	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(k, 0x3C13FF20);

	/*
	 * 2. Read memory location
	 */

	/* LUI T0, <Addr31:16>	LOAD UPPER IMMEDIATE, CLEAR LOWER */
	pic32_xferinstruction(k, 0x3C080000 | (addr >> 16));
	/* ORI T0, <Addr15:0>	OR LOWER IMMEDIATE */
	pic32_xferinstruction(k, 0x35080000 | (addr & 0xFFFF));

	/*
	 * 3. Write to Fastdata location
	 */

	/* LW T1, 0(T0)		LOAD WORD    */
	pic32_xferinstruction(k, 0x8D090000);
	/* SW T1, 0(S3)		STORE WORD   */
	pic32_xferinstruction(k, 0xAE690000);
	/* NOP			NO OPERATION */
	pic32_xferinstruction(k, 0x00000000);

	/*
	 * 4. Read data from Fastdata register 0xFF200000
	 */

	/* Select the Fastdata Register */
	pic32_sendcommand(k, PIC32_ETAP_FASTDATA);

	/* Send/Receive 32-bit Data */
	data = pic32_xferfastdata(k, 0);

	return data;
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

void
pic32_bulk_erase(struct k8048 *k, uint16_t osccal __attribute__((unused)), uint16_t bandgap __attribute__((unused)))
{
	pic32_program_verify(k);

	pic32_erase_device(k);

	pic32_standby(k);
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * GET CONFIGURATION
 */
void
pic32_read_config_memory(struct k8048 *k, int flag __attribute__((unused)))
{
	/* NULL device */
	pic32_index = PIC32_SIZE;

	/* Reset configuraton */
	memset(&pic32_conf, 0, sizeof(pic32_conf));

	pic32_program_verify(k);

	/* Device id/rev */
	pic32_conf.deviceid = pic32_xferdata(k, 32, 0);

	uint32_t dev = 0;
	while (pic32_map[dev].deviceid) {
		if (pic32_map[dev].deviceid == pic32_conf.deviceid)
			break;
		++dev;
	}
	if (!pic32_map[dev].deviceid) {
		if (pic32_conf.deviceid == 0x00000000 || pic32_conf.deviceid == 0xFFFFFFFF) {
			printf("%s: fatal error: %s.\n",
				__func__, io_fault(k, pic32_conf.deviceid));
		} else {
			printf("%s: fatal error: device unknown: [%08X]\n",
				__func__, pic32_conf.deviceid);
		}
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}

	/* Device recognised */
	pic32_index = dev;

	/* Device status */
	pic32_conf.status = pic32_check_device_status(k);
	if (PIC32_MCHP_STATUS_CFGRDY !=
		(pic32_conf.status & (PIC32_MCHP_STATUS_CFGRDY | PIC32_MCHP_STATUS_FCBUSY))) {
		printf("%s: fatal error: status invalid [0x%02X]\n",
			__func__, pic32_conf.status);
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}

	/* Device id/rev validation */
	pic32_enter_serial_execution_mode(k);

	uint32_t deviceid = pic32_readmemorylocation(k, PIC32_DEVID);
	if (deviceid != pic32_conf.deviceid) {
		printf("%s: fatal error: device id invalid [0x%08X] != [0x%08X]\n",
			__func__, deviceid, pic32_conf.deviceid);
		pic32_standby(k);
		io_exit(k, EX_SOFTWARE); /* Panic */
	}

	/* Config */
	pic32_conf.configaddr = PIC32_BOOT + (pic32_map[pic32_index].boot << 2) - (4 << 2);
	for (uint32_t i = 0; i < 4; ++i)
		pic32_conf.config[3 - i] = pic32_readmemorylocation(k, pic32_conf.configaddr + 4 * i);

	pic32_standby(k);
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic32_get_program_size(uint32_t *addr)
{
	*addr = PIC32_CODE;

	return pic32_map[pic32_index].prog;
}

/*
 * GET BOOT FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic32_get_boot_size(uint32_t *addr)
{
	*addr = PIC32_BOOT;

	return pic32_map[pic32_index].boot;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic32_read_program_memory_block(struct k8048 *k, uint32_t *data, uint32_t addr, uint32_t size)
{
	pic32_program_verify(k);

	pic32_enter_serial_execution_mode(k);

	for (uint32_t i = 0; i < size; ++i) {
		data[i] = pic32_readmemorylocation(k, addr);
		addr += 4;
	}

	pic32_standby(k);

	return addr;
}

/*****************************************************************************
 *
 * Program Code
 *
 *****************************************************************************/

/*
 * WRITE PANEL TO 32-BIT PROGRAM FLASH
 */
void
pic32_write_panel(struct k8048 *k, uint32_t region, uint32_t address, uint32_t *panel, uint32_t panel_size)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		pic32_download_data_block(k, panel, panel_size);
		pic32_flash_row_write(k, address, 0x0000);
		break;
	default:printf("%s: warning: region unsupported [%d]\n", __func__, region);
		break;
	}
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE OR BOOT
 *
 *  RETURN PIC_REGIONCODE:
 *	0x1D000000 .. 0x1D1FFFFF (2048KB)
 *
 *  RETURN PIC_REGIONBOOT:
 *  	0x1FC00000 .. 0x1FC13FFF (80KB)
 *
 * DS60001145N-page 10
 */
uint32_t
pic32_getregion(uint32_t address)
{
	uint32_t vaddr = pic32_kseg1(address);

	if (vaddr >= PIC32_CODE && vaddr < (PIC32_CODE + (1024 * 2048))) {
		return PIC_REGIONCODE;
	}
	if (vaddr >= PIC32_BOOT && vaddr < (PIC32_BOOT + (1024 * 80))) {
		return PIC_REGIONBOOT;
	}
	printf("%s: warning: address unsupported [%06X]\n", __func__, address);
	return PIC_REGIONNOTSUP;
}

/*
 * INIT REGION FOR WRITING
 *
 *  RETURN REGION IF WRITING SUPPORTED
 */
uint32_t
pic32_init_writeregion(struct k8048 *k, uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		pic_write_panel(k, PIC_PANEL_BEGIN, region, pic32_map[pic32_index].row << 2);
		return region;
		break;
	}
	printf("%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * WRITE REGION
 */
void
pic32_writeregion(struct k8048 *k, uint32_t address, uint32_t region, uint32_t data)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONBOOT:
		pic_write_panel(k, PIC_PANEL_UPDATE, address, data);
		break;
	default:printf("%s: warning: region unsupported [%d]\n", __func__, region);
		break;
	}
}

/*
 * INIT REGION FOR VERIFY
 *
 *  RETURN REGION IF VERIFY SUPPORTED
 */
uint32_t
pic32_init_verifyregion(struct k8048 *k, uint32_t region)
{
	if (region != PIC_REGIONNOTSUP) {
		return region;
	}
	printf("%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * VERIFY REGION
 *
 *  RETURN BYTE FAILURE COUNT
 */
uint32_t
pic32_verifyregion(struct k8048 *k, uint32_t address, uint32_t region, uint16_t index, uint32_t data)
{
	uint32_t vdata = 0;

	if (region == PIC_REGIONNOTSUP) {
		printf("%s: warning: region unsupported [%d]\n", __func__, region);
		return 4;
	}
	vdata = pic32_readmemorylocation(k, pic32_kseg1(address));
	if (vdata != data) {
		printf("%s: error: read [%08X] expected [%08X] at [%08X]\n",
			__func__, vdata, data, pic32_phy(address));
		return 4;
	}
	return 0;
}

/*
 * PROGRAM FILE
 */
void
pic32_program(struct k8048 *k, char *filename, int blank)
{
	uint32_t PC_address;
	uint32_t new_region, current_region = PIC_REGIONNOTSUP;
	uint32_t total = 0;

	/* Get HEX */
	if (!inhx32(k, filename, 4))
		return;

	/* Initialise device for programming */
	if (blank)
		pic32_bulk_erase(k, PIC_VOID, PIC_VOID);

	/*
	 * Program device
	 */

	pic32_program_verify(k);

	pic32_enter_serial_execution_mode(k);

	/* For each line */
	for (uint32_t i = 0; i < k->count; i++) {
		PC_address = k->pdata[i]->address;
		new_region = pic32_getregion(PC_address);
		if (new_region == PIC_REGIONNOTSUP)
			continue;
		if (new_region != current_region) {
			pic_write_panel(k, PIC_PANEL_END, PIC_VOID, PIC_VOID);
			current_region = pic32_init_writeregion(k, new_region);
			if (current_region == PIC_REGIONNOTSUP)
				continue;
		}

		/* For each byte in line */
		for (uint32_t j = 0; j < k->pdata[i]->nbytes; ++j) {
			pic32_writeregion(k, PC_address++, current_region, k->pdata[i]->bytes[j]);
			total++;
		}
	}
	pic_write_panel(k, PIC_PANEL_END, PIC_VOID, PIC_VOID);

	pic32_standby(k);

	printf("Total: %u\n", total);

	inhx32_free(k);
}

/*
 * VERIFY FILE
 */
uint32_t
pic32_verify(struct k8048 *k, char *filename)
{
	uint32_t PC_address, fail = 0, total = 0, wdata = 0;
	uint32_t new_region, current_region = PIC_REGIONNOTSUP;

	/* Get HEX */
	if (!inhx32(k, filename, 4))
		return 1;

	/*
	 * Verify device
	 */

	pic32_program_verify(k);

	pic32_enter_serial_execution_mode(k);

	/* For each line */
	for (uint32_t i = 0; i < k->count; i++) {
		PC_address = k->pdata[i]->address;
		new_region = pic32_getregion(PC_address);
		if (new_region == PIC_REGIONNOTSUP)
			continue;
		if (new_region != current_region) {
			current_region = pic32_init_verifyregion(k, new_region);
			if (current_region == PIC_REGIONNOTSUP)
				continue;
		}

		/* For each 32-bit word in line */
		for (uint32_t j = 0; j < k->pdata[i]->nbytes; j += 4) {
			wdata = k->pdata[i]->bytes[j] |
				(k->pdata[i]->bytes[j + 1] << 8) |
				(k->pdata[i]->bytes[j + 2] << 16) |
				(k->pdata[i]->bytes[j + 3] << 24);
			fail += pic32_verifyregion(k, PC_address, current_region, j, wdata);
			PC_address += 4;
			total += 4;
		}
	}

	pic32_standby(k);

	printf("Total: %u Pass: %u Fail: %u\n", total, total - fail, fail);

	inhx32_free(k);

	return fail;
}

/*****************************************************************************
 *
 * Diagnostic functions
 *
 *****************************************************************************/

/*
 * DUMP DEVICE ID DETAILS
 */
void
pic32_dumpdeviceid(struct k8048 *k)
{
	printf("[%08X] [PROGRAM]  %08X WORDS\n", pic32_phy(PIC32_CODE), pic32_map[pic32_index].prog);
	printf("[%08X] [DEVICEID] %08X %s\n",
		pic32_phy(PIC32_DEVID), pic32_conf.deviceid, pic32_map[pic32_index].devicename);
	printf("[%08X] [BOOT]     %08X WORDS\n", pic32_phy(PIC32_BOOT), pic32_map[pic32_index].boot);
	pic32_dumpconfig(k, PIC_BRIEF);
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic32_dumpconfig(struct k8048 *k, int mode)
{
	for (uint32_t i = 0; i < 4; ++i)
		printf("[%08X] [DEVCFG%d]  %08X\n", pic32_phy(pic32_conf.configaddr + 4 * i),
			3 - i, pic32_conf.config[3 - i]);
}

/*
 * DUMP HEX FLASH WORDS
 */
void
pic32_dumphexcode(struct k8048 *k, uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 4) {
#ifndef DEBUG
		if (pic_mtcode(PIC32_MASK, 4, &data[i]))
			continue;
#endif
		printf("[%08X] ", pic32_phy(address));
		for (j = 0; j < 4; ++j) {
			printf("%08X ", data[i + j]);
		}
		for (j = 0; j < 4; ++j) {
			putchar(PIC_CHAR(0xFF & data[i + j]));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 8)));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 16)));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 24)));
		}
		putchar('\n');
		nlines++;
	}
	if (!nlines)
		printf("%s: information: flash empty\n", __func__);
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic32_dumpinhxcode(struct k8048 *k, uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* PIC32: Physical addressing */
	address = pic32_phy(address);

	/* PIC32: Extended address */
	pic_dumpaddr(address, 1);

	for (i = 0; i < size; address += 16, i += 4) {
		if (pic_mtcode(PIC32_MASK, 4, &data[i]))
			continue;

		/* PIC32: Extended address */
		pic_dumpaddr(address, 0);

		uint8_t cc, b0, b1, b2, b3;
		b0 = address >> 8;
		b1 = address;
		printf(":%02X%02X%02X00", 16, b0, b1);

		cc = 16 + b0 + b1 + 0x00;
		for (j = 0; j < 4; ++j) {
			b0 = data[i + j];
			b1 = data[i + j] >> 8;
			b2 = data[i + j] >> 16;
			b3 = data[i + j] >> 24;
			printf("%02X%02X%02X%02X", b0, b1, b2, b3);
			cc = cc + b0 + b1 + b2 + b3;
		}
		printf("%02X\n", (0x0100 - cc) & 0xFF);
	}
}
