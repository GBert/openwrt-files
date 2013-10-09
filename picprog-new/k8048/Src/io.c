/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "k8048.h"

/******************************************************************************
 * CTRL-C signal handler
 *****************************************************************************/

int io_stop = 0;

void
io_signal()
{
	io_stop = 1;
}

void
io_signal_on()
{
	io_stop = 0;
	signal(SIGINT, io_signal);
}

void
io_signal_off()
{
	signal(SIGINT, SIG_DFL);
}

/******************************************************************************
 * Open/Init/Close I/O
 *****************************************************************************/

int
io_open(struct k8048 *k, int standby)
{
	if (k->iot != IONONE) {
		printf("%s: fatal error: device already open.\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	k->iot = IONONE;
	k->fd  = -1;
	k->gpio.fd = -1;
	k->i2c = -1;
	k->lpicp.icsp_dev_file = -1;
	if ((strstr(k->device, "/dev/cu") == k->device) || 
		(strstr(k->device, "/dev/tty") == k->device)) {
		/* tty */
		k->iot = IOTTY;
		k->fd = open_port(k->device);
		if (k->fd < 0) {
			k->fd = -1;
			return -1;
		}
		if (standby)
			io_init(k);
		return 0;
	} else if (mystrcasestr(k->device, "rpi") == k->device) {
		/* rpi */
		k->iot = IORPI;
		gpio_open(&k->gpio);
		if (k->gpio.fd < 0) {
			k->gpio.fd = -1;
			return -1;
		}
		if (standby)
			io_init(k);
		return 0;
	} else if (strstr(k->device, "/dev/i2c") == k->device) {
		/* mcp23017 i2c */
		k->iot = IOI2C;
		k->i2c = open_i2c(k->device, k->mcp);
		if (k->i2c < 0) {
			k->i2c = -1;
			return -1;
		}
		if (standby)
			io_init(k);
		return 0;
	} else if (strstr(k->device, "/dev/icsp") == k->device) {
		/* lpicp */
		k->iot = IOLPICP;
		if (lpp_icsp_init(&k->lpicp, k->device) <= 0) {
			k->lpicp.icsp_dev_file = -1;
			return -1;
		}
		if (standby)
			io_init(k);
		return 0;
	}
	return -1;
}

void
io_init(struct k8048 *k)
{
	if (k->iot == IONONE) {
		printf("%s: fatal error: device not open.\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	switch (k->iot) {
	case IOTTY:	/* tty */
		break;
	case IORPI:	/* rpi */
		gpio_init(k);
		break;
	case IOI2C:	/* mcp23017 i2c */
		init_i2c(k->i2c);
		break;
	case IOLPICP:	/* lpicp */
	default:break;
	}
	io_standby(k);
}

void
io_close(struct k8048 *k, int standby)
{
	if (k->iot == IONONE) {
		printf("%s: fatal error: device not open.\n", __func__);
		exit(EX_SOFTWARE); /* Panic */
	}
	if (standby) {
		io_standby(k);
		if (k->run) /* Run: take out of reset */
			io_set_vpp_pgm(k, HIGH, LOW);
	}
	switch (k->iot) {
	case IOTTY:	/* tty */
		close(k->fd);
		k->fd = -1;
		break;
	case IORPI:	/* rpi */
		gpio_close(&k->gpio);
		k->gpio.fd = -1;
		break;
	case IOI2C:	/* mcp23017 i2c */
		close_i2c(k->i2c);
		k->i2c = -1;
		break;
	case IOLPICP:	/* lpicp */
		lpp_icsp_destroy(&k->lpicp);
	default:break;
	}
	k->iot = IONONE;
}

/******************************************************************************
 * K8048 BIT BANGING (nb. output is positive when feature is enabled)
 *****************************************************************************/

/*
 * Set PGD (DATA OUT)
 */
void
io_set_pgd(struct k8048 *k, int pgd)
{
	if (k->bitrules & PGD_OUT_FLIP) {
		/* Eg. k8048/tty: inverting, k8076/tty: non-inverting */
		pgd = HIGH - pgd;
	}
	switch (k->iot) {
	case IOTTY:	/* tty */
		set_dtr(k->fd, pgd);
		break;
	case IORPI:	/* rpi */
		gpio_set(&k->gpio, k->gpio.pgdo, pgd);
		break;
	case IOI2C:	/* mcp23017 i2c */
		mcp_set_pgd(k->i2c, pgd);
		break;
	case IOLPICP:	/* lpicp */
	default:break;
	}
}

/*
 * Set PGC (CLOCK OUT)
 */
void
io_set_pgc(struct k8048 *k, int pgc)
{
	if (k->bitrules & PGC_OUT_FLIP) {
		pgc = HIGH - pgc;
	}
	switch (k->iot) {
	case IOTTY:	/* tty */
		set_rts(k->fd, pgc);
		break;
	case IORPI:	/* rpi */
		gpio_set(&k->gpio, k->gpio.pgc, pgc);
		break;
	case IOI2C:	/* mcp23017 i2c */
		mcp_set_pgc(k->i2c, pgc);
		break;
	case IOLPICP:	/* lpicp */
	default:break;
	}
}

/*
 * Set PGD/PGC (DATA OUT/CLOCK OUT) 
 */
void
io_set_pgd_pgc(struct k8048 *k, int pgd, int pgc)
{
	if (k->bitrules & PGD_OUT_FLIP) {
		pgd = HIGH - pgd;
	}
	if (k->bitrules & PGC_OUT_FLIP) {
		pgc = HIGH - pgc;
	}
	switch (k->iot) {
	case IOTTY:	/* tty */
		set_dtr_rts(k->fd, pgd, pgc);
		break;
	case IORPI:	/* rpi */
		gpio_set(&k->gpio, k->gpio.pgdo, pgd);
		gpio_set(&k->gpio, k->gpio.pgc,  pgc);
		break;
	case IOI2C:	/* mcp23017 i2c */
		mcp_set_pgd_pgc(k->i2c, pgd, pgc);
		break;
	case IOLPICP:	/* lpicp */
	default:break;
	}
}

/*
 * Set VPP/PGM (VPP OUT/PGM OUT)
 */
void
io_set_vpp_pgm(struct k8048 *k, int vpp, int pgm)
{
	if (k->bitrules & VPP_OUT_FLIP) {
		vpp = HIGH - vpp;
	}
	if (k->bitrules & PGM_OUT_FLIP) {
		pgm = HIGH - pgm;
	}
	switch (k->iot) {
	case IOTTY:	/* tty (pgm not supported) */
		set_tx(k->fd, vpp);
		break;
	case IORPI:	/* rpi */
		gpio_set(&k->gpio, k->gpio.pgm, pgm);
		gpio_set(&k->gpio, k->gpio.vpp, vpp);
		break;
	case IOI2C:	/* mcp23017 i2c */
		mcp_set_vpp_pgm(k->i2c, vpp, pgm);
		break;
	case IOLPICP:	/* lpicp */
		lpp_icsp_mclr_set(&k->lpicp, vpp);
	default:break;
	}
}

/*
 * Get PGD (DATA IN)
 */
unsigned char
io_get_pgd(struct k8048 *k)
{
	unsigned char pgd = 0;

	switch (k->iot) {
	case IOTTY:	/* tty */
		pgd = get_cts(k->fd) ? HIGH : LOW;
		break;
	case IORPI:	/* rpi */
		gpio_get(&k->gpio, k->gpio.pgdi, &pgd);
		break;
	case IOI2C:	/* mcp23017 i2c */
		pgd = mcp_get_pgd(k->i2c) ? HIGH : LOW;
		break;
	case IOLPICP:	/* lpicp */
	default:break;
	}
	if (k->bitrules & PGD_IN_FLIP) {
		pgd = HIGH - pgd;
	}
	return pgd;
}

/******************************************************************************
 * K8048 BIT BANG SHORTCUTS
 *****************************************************************************/

/*
 * Sleep for a few microseconds...
 */
void
io_usleep(struct k8048 *k, int n)
{
	/* No sleep */
	if (n <= 0) {
		return;
	}
	/* I/O sleep */
	if (k->iot == IOTTY && n == 1) {
		/* 1us ttyS, 3ms ttyUSB */
		get_cts(k->fd);
		return;
	}
	/* Busy sleep */
	if (n < 100) {
		struct timeval tv1, tv2;
		int diff;
		gettimeofday(&tv1, NULL);
		do {
			gettimeofday(&tv2, NULL);
			diff = tv2.tv_usec - tv1.tv_usec;
			if (diff < 0)
				diff += 1000000;	
		} while (diff < n);
		return;
	}
	/* System sleep */
	usleep(n);
}

/*
 * Put board in standby
 */
void
io_standby(struct k8048 *k)
{
	io_usleep(k, 10000); /* 10ms */

	io_set_pgd_pgc(k, LOW, LOW);

	if (k->key == MCHPKEY) {
		io_set_vpp_pgm(k, HIGH, HIGH);
		io_usleep(k, 1000); /* 1ms */
	}

	/* Reset */
	io_set_vpp_pgm(k, LOW, LOW);

	io_usleep(k, 1000); /* 1ms */

	io_cursor(k, '-');
}

/*
 * Initialise board and device for Program/Verify Mode
 */
void
io_init_program_verify(struct k8048 *k)
{
	io_cursor(k, '+');

	io_standby(k);

	if (k->key != NOKEY) {
		io_set_vpp_pgm(k, HIGH, HIGH);
		io_usleep(k, 1000); /* 1ms */
		io_set_vpp_pgm(k, LOW, LOW);
		io_usleep(k, 1000); /* 1ms */

		io_word_out32(k, k->key);
	}

	if (k->key == MCHPKEY) {
		if (k->iot == IOLPICP) {
			printf("%s: fatal error: unsupported\n", __func__);
               		exit(EX_SOFTWARE); /* Panic */
		}
		/* PIC16F1XXX 33 clock pulses */
		io_clock_in(k, k->sleep);
	} else {
		io_set_vpp_pgm(k, HIGH, HIGH);
	}

	io_usleep(k, 1000); /* 1ms */
}

/*
 * Acquire data input
 *
 * Flip data direction on R-PI when data in = data out,
 * else pull data output high for data input.
 */
void
io_data_input_acquire(struct k8048 *k)
{
	if (k->iot == IORPI && k->gpio.pgdi == k->gpio.pgdo) {
		gpio_reselect_input(&k->gpio, k->gpio.pgdi);
	} else {
		if (k->bitrules & PGD_IN_PULLUP) {
			io_set_pgd(k, HIGH);
		} else {
			io_set_pgd(k, LOW);
		}
	}
}

/*
 * Release data input
 *
 * Flip data direction on R-PI when data in = data out,
 * else do nothing.
 */
void
io_data_input_release(struct k8048 *k)
{
	if (k->iot == IORPI && k->gpio.pgdi == k->gpio.pgdo) {
		gpio_reselect_output(&k->gpio, k->gpio.pgdo);
	}
}

/*
 * Write: Clock a bit out (in to chip)
 */
void
io_clock_out(struct k8048 *k, int t, int bit)
{
	io_set_pgd(k, bit);	/* output bit */

	io_set_pgc(k, HIGH);	/* clock high */
	io_usleep(k, t);

	io_set_pgc(k, LOW);	/* clock low */
	io_usleep(k, t);
}

/*
 * Read: Clock a bit in (out of chip)
 */
unsigned char
io_clock_in(struct k8048 *k, int t)
{
	unsigned char clkin;

	io_set_pgc(k, HIGH);	/* clock high */
	io_usleep(k, t);

	io_set_pgc(k, LOW);	/* clock low */
	io_usleep(k, t);

	clkin = io_get_pgd(k);	/* input bit */

	return clkin;
}

/*
 * Output command word
 */
void
io_command_out(struct k8048 *k, char *command)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		printf("%s: fatal error: unsupported\n", __func__);
               	exit(EX_SOFTWARE); /* Panic */
	} else {
		int i, l = strlen(command) - 1;

		for (i = l; i >= 0; i--)
			io_clock_out(k, k->sleep, command[i] - '0');
#if 1
		io_usleep(k, k->sleep);	/* 1us Tdly1|Tdly2 / P5 */
#endif
	}
	if (k->debug >= 15) {
		printf("%s: command [%s]\n", __func__, command);
	}
}

/*
 * Output 4-bit command and send 16-bit data (pic18f)
 */
void
io_command_out16(struct k8048 *k, unsigned char command, unsigned short word)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		lpp_icsp_write_16(&k->lpicp, command, word);
	} else {
		int i;

		for (i = 0; i < 4; ++i)
			io_clock_out(k, k->sleep, (command >> i) & 1);
#if 1
		io_usleep(k, k->sleep);	/* 1us Tdly1|Tdly2 / P5 */
#endif
        	io_word_out16(k, word);
	}
	if (k->debug >= 15) {
		printf("%s: command [0x%02X] data [0x%04X]\n", __func__,
			command, word);
	}

}

/*
 * Output programming NOP (pic18f)
 */
void
io_command_program(struct k8048 *k, unsigned int delay)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		struct mc_icsp_cmd_only_t cmd_config = {
                	.command = 0x00,
                	.pgc_value_after_cmd = 1,
                	.pgd_value_after_cmd = 0,
			.msleep = 0,
                	.mdelay = 0,
                	.udelay = 0
        	};
		if (k->sleep)
			cmd_config.msleep = 1 + (delay / 1000);
		else
			cmd_config.udelay = delay;
		lpp_icsp_command_only(&k->lpicp, &cmd_config);
	} else {
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_set_pgc(k, HIGH);  		/* 0 CLOCK HIGH */
		io_usleep(k, delay);		/* P9/P9A       */
		io_set_pgc(k, LOW);		/* 0 CLOCK LOW  */
#if 1
		io_usleep(k, k->sleep);         /* 1us Tdly1|Tdly2 / P5 */
#endif
	}
	if (k->debug >= 15) {
		printf("%s: command [NOP]\n", __func__);
	}

	io_word_out16(k, 0);
}

/*
 * Output erase NOP (pic18f)
 */
void
io_command_erase(struct k8048 *k, unsigned int delay)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		struct mc_icsp_cmd_only_t cmd_config = {
                	.command = 0x00,
                	.pgc_value_after_cmd = 0,
                	.pgd_value_after_cmd = 0,
			.msleep = 0,
                	.mdelay = 0,
                	.udelay = 0
        	};
		if (k->sleep)
			cmd_config.msleep = 1 + (delay / 1000);
		else
			cmd_config.udelay = delay;
		lpp_icsp_command_only(&k->lpicp, &cmd_config);
	} else {
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_clock_out(k, k->sleep, 0);	/* 0 */
		io_usleep(k, delay);		/* P11 + P10 */
#if 0
		io_usleep(k, k->sleep);         /* 1us Tdly1|Tdly2 / P5 */
#endif
	}
	if (k->debug >= 15) {
		printf("%s: command [NOP]\n", __func__);
	}

	io_word_out16(k, 0);
}

/*
 * Output 4-bit command and receive 8-bit data (pic18f)
 */
unsigned char
io_command_in8(struct k8048 *k, unsigned char command)
{
	unsigned char byte;

	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		lpp_icsp_read_8(&k->lpicp, command, &byte);
	} else {
		int i;

		for (i = 0; i < 4; ++i)
			io_clock_out(k, k->sleep, (command >> i) & 1);
#if 1
		io_usleep(k, k->sleep);	/* 1us Tdly1|Tdly2 / P5 */
#endif
		byte = io_byte_in(k);
	}
	if (k->debug >= 15) {
		printf("%s: command [0x%02X] data [0x%02X]\n", __func__,
			command, byte);
	}

	return byte;
}

/*
 * Output [START bit:14 bit word out:STOP bit] (pic16f/12f/10f)
 */
void
io_word_out14(struct k8048 *k, unsigned short word)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		printf("%s: fatal error: unsupported\n", __func__);
               	exit(EX_SOFTWARE); /* Panic */
	} else {
		int i;

		/* Start bit */
		io_clock_out(k, k->sleep, HIGH);

		/* 14-bit word */
		for (i = 0; i < 14; i++)
			io_clock_out(k, k->sleep, (word >> i) & 1);
	
		/* Stop bit */
		io_clock_out(k, k->sleep, HIGH);
	
		io_usleep(k, k->sleep);	/* 1us Tdly1|Tdly2 */
	}
}

/*
 * Input [START bit:14 bit word in:STOP bit] (pic16f/12f/10f)
 */
unsigned short
io_word_in14(struct k8048 *k)
{
	unsigned short word = 0;

	io_cursor(k, 'i');

	if (k->iot == IOLPICP) {
		printf("%s: fatal error: unsupported\n", __func__);
               	exit(EX_SOFTWARE); /* Panic */
	} else {
		int i;

		/* Prepare data input */
		io_data_input_acquire(k);

		/* Start bit */
		io_clock_in(k, k->sleep);

		/* 14-bit word */
		for (i = 0; i < 14; i++)
			word |= (io_clock_in(k, k->sleep) << i);

		/* Stop bit */
		io_clock_in(k, k->sleep);
	
		io_usleep(k, k->sleep);	/* 1us Tdly1|Tdly2 */
	
		/* Release data input */
		io_data_input_release(k);
	}

	return word;
}

/*
 * Output [16 bit word out] (pic18f)
 */
void
io_word_out16(struct k8048 *k, unsigned short word)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		lpp_icsp_data_only_16(&k->lpicp, word);
	} else {
		int i;

		for (i = 0; i < 16; i++) {
			io_clock_out(k, k->sleep, (word >> i) & 1);
		}
#if 0
		io_usleep(k, k->sleep);	/* P5A */
#endif
	}
}

/*
 * Output [32 bit word out] (pic16f, pic18f)
 */
void
io_word_out32(struct k8048 *k, unsigned long word)
{
	io_cursor(k, 'o');

	if (k->iot == IOLPICP) {
		lpp_icsp_data_only_32(&k->lpicp, word);
	} else {
		int i;

		for (i = 0; i < 32; i++) {
			io_clock_out(k, k->sleep, (word >> i) & 1);
		}
	}
}

/*
 * Input [8 bit zero out:8 bit byte in] (pic18f)
 */
unsigned char
io_byte_in(struct k8048 *k)
{
	unsigned char byte = 0;

	io_cursor(k, 'i');

	if (k->iot == IOLPICP) {
		printf("%s: fatal error: unsupported\n", __func__);
               	exit(EX_SOFTWARE); /* Panic */
	} else {
		int i;

		for (i = 0; i < 8; i++)
			io_clock_out(k, k->sleep, LOW);

		/* Prepare data input */
		io_data_input_acquire(k);
#if 0
		io_usleep(k, k->sleep);	/* P6 */
#else
		io_usleep(k, k->sleep);	/* **TTYUSB CRITICAL** */
#endif
		for (i = 0; i < 8; i++)
			byte |= (io_clock_in(k, k->sleep) << i);
#if 0
		io_usleep(k, k->sleep);	/* P5A */
#endif
		/* Release data input */
		io_data_input_release(k);
	}

	return byte;
}

/*
 * I/O busy cursor
 */
#define ARROWLENGTH (4)
void
io_cursor(struct k8048 *k, char c)
{
	static int c_count = 0;
	static const char arrow[] = "|/-\\";

	if (!k->busy) {
		return; /* Disabled BUSY=0 */
	}
	if (++c_count % (k->busy * ARROWLENGTH - 1) == 0) {
		fprintf(stderr, "%c[%c]\r",
			arrow[ARROWLENGTH - (c_count % ARROWLENGTH) - 1], c);
	}
}
#undef ARROWLENGTH

/******************************************************************************
 * Hardware test routines for `ktest'
 *****************************************************************************/

/*
 * Test VPP, PGC or PGD
 */
#ifdef KTEST
void
io_test0(struct k8048 *k, int pin, int t)
{
	printf("\nTEST MODE VPP|PGD|PGC|PGM\n\n");

	switch (pin) {
	case 0: printf("VPP LOW  (ICSP 1) (D-SUB-9 TX 3) [3 seconds]\n");
		break;
	case 1: printf("PGC LOW  (ICSP 5) (D-SUB-9 RTS 7) [3 seconds]\n");
		break;
	case 2: printf("PGD LOW  (ICSP 4) (D-SUB-9 DTR 3) [3 seconds]\n");
		break;
	case 3: printf("PGM LOW  [3 seconds]\n");
		break;
	}
	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);
	sleep(3);

	switch (pin) {
	case 0: printf("VPP HIGH (ICSP 1) (D-SUB-9 TX 3) [%d seconds]\n", t);
		io_set_vpp_pgm(k, HIGH, LOW);
		break;
	case 1: printf("PGC HIGH (ICSP 5) (D-SUB-9 RTS 7) [%d seconds]\n", t);
		io_set_pgd_pgc(k, LOW, HIGH);
		break;
	case 2: printf("PGD HIGH (ICSP 4) (D-SUB-9 DTR 3) [%d seconds]\n", t);
		io_set_pgd_pgc(k, HIGH, LOW);
		break;
	case 3: printf("PGM HIGH [%d seconds]\n", t);
		io_set_vpp_pgm(k, LOW, HIGH);
		break;
	}
	sleep(t);

	switch (pin) {
	case 0: printf("VPP LOW  (ICSP 1) (D-SUB-9 TX 3) [3 seconds]\n");
		break;
	case 1: printf("PGC LOW  (ICSP 5) (D-SUB-9 RTS 7) [3 seconds]\n");
		break;
	case 2: printf("PGD LOW  (ICSP 4) (D-SUB-9 DTR 3) [3 seconds]\n");
		break;
	case 3: printf("PGM LOW  [3 seconds]\n");
		break;
	}
	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);
	sleep(3);
	
	printf("\nTEST DONE\n\n");
}

/*
 * Test D-SUB-9
 */
void
io_test1(struct k8048 *k, int t)
{
	printf("\nTEST MODE 1 [D-SUB-9]\n\n");

	k->bitrules = 0; /* Disable rules */

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);

	printf("Tx  SET  (+VE) (D-SUB-9 3) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp_pgm(k, HIGH, LOW);
	sleep(t);
	printf("CTS IN: %d\n", io_get_pgd(k));

	printf("Tx  CLR  (-VE) (D-SUB-9 3) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp_pgm(k, LOW, LOW);
	sleep(t);
	printf("CTS IN: %d\n\n", io_get_pgd(k));

	printf("DTR SET  (+VE) (D-SUB-9 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(k, HIGH);
	sleep(t);
	printf("CTS IN: %d\n", io_get_pgd(k));

	printf("DTR CLR  (-VE) (D-SUB-9 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(k, LOW);
	sleep(t);
	printf("CTS IN: %d\n\n", io_get_pgd(k));
	
	printf("RTS SET  (+VE) (D-SUB-9 7) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(k, HIGH);
	sleep(t);
	printf("CTS IN: %d\n", io_get_pgd(k));

	printf("RTS CLR  (-VE) (D-SUB-9 7) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(k, LOW);
	sleep(t);
	printf("CTS IN: %d\n\n", io_get_pgd(k));
	
	printf("TEST DONE\n\n");
	
	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);
}

/*
 * Test ICSP
 */
void
io_test2(struct k8048 *k, int t)
{
	printf("\nTEST MODE 2 [ICSP]\n\n");

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);

	printf("VPP LOW  (0V)  (ICSP 1) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp_pgm(k, LOW, LOW);
	sleep(t);
	printf("PGD IN: %d\n", io_get_pgd(k));

	printf("VPP HIGH (12V) (ICSP 1) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp_pgm(k, HIGH, LOW);
	sleep(t);
	printf("PGD IN: %d\n\n", io_get_pgd(k));

	printf("PGD LOW  (0V)  (ICSP 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(k, LOW);
	sleep(t);
	printf("PGD IN: %d\n", io_get_pgd(k));

	printf("PGD HIGH (5V)  (ICSP 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(k, HIGH);
	sleep(t);
	printf("PGD IN: %d\n\n", io_get_pgd(k));

	printf("PGC LOW  (0V)  (ICSP 5) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(k, LOW);
	sleep(t);
	printf("PGD IN: %d\n", io_get_pgd(k));

	printf("PGC HIGH (5V)  (ICSP 5) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(k, HIGH);
	sleep(t);
	printf("PGD IN: %d\n\n", io_get_pgd(k));

	printf("TEST DONE\n\n");
	
	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);
}

/*
 * Test D-SUB-9 RTS 7 (PGC) DTR 4 (PGD)
 */
void
io_test3(struct k8048 *k, int t)
{
	int data = LOW, clk = HIGH - data;

	printf("\nTEST MODE 3 [D-SUB-9 RTS 7 (PGC) DTR 4 (PGD)] CTRL-C TO STOP\n\n");

	io_signal_on();

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);

	while (!io_stop) {
		io_set_pgd_pgc(k, data, clk);
		switch (t) {
		case 0:	/* None */
			break;
		case 1:	/* Sleep */
			io_usleep(k, k->sleep);
			break;
		default:/* Microseconds */
			io_usleep(k, t);
			break;
		}
		data = clk;
		clk  = HIGH - data;
	}

	printf("\nTEST DONE\n\n");

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);
	
	io_signal_off();
}

/*
 * Test 16F627
 *
 * Program:
 *	k14 p asm/pic16f627/debug.hex
 *
 * Note: This is the PIC which came with the K8048.
 */
void
io_test4(struct k8048 *k, int t)
{
	int i, j = 0;
	unsigned char c, line[STRLEN];

	printf("\nTEST MODE 4 [16F627 debug.asm] CTRL-C TO STOP\n\n");

	io_signal_on();

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);

	/* Prepare data input */
	io_data_input_acquire(k);

	while (!io_stop) {
		/* Handshake */
		io_set_pgc(k, LOW);			/* PGC LOW */

		while (io_get_pgd(k) != HIGH && !io_stop)/* WAIT FOR PGD HIGH */
			io_usleep(k, 1);
		
		io_set_pgc(k, HIGH);			/* PGC HIGH */
		
		while (io_get_pgd(k) != LOW && !io_stop)/* WAIT FOR PGD LOW */
			io_usleep(k, 1);

		/* Byte input */
		c = 0;
		for (i = 0; i < 8; i++) {
			io_set_pgc(k, LOW);		/* PGC LOW */
			io_usleep(k, t);

			c = c >> 1;			/* CLOCK BIT IN */
			if (io_get_pgd(k))
				c |= 0x80;

			io_set_pgc(k, HIGH);		/* PGC HIGH */
			io_usleep(k, t);
		}

		printf("%s: read byte: %02X %c\n", __func__,
			c, (c >= ' ' && c < 128) ? c : '.');

		if (c == 0x03) {			/* ETX */
			if (j > 1 && line[0] == 0x02) {	/* STX */
				line[j] = '\0';
				printf("%s: read line: %s", __func__,
					&line[1]);
			}
			j = 0;
		}
		else if (j < STRMAX)
			line[j++] = c;
	}

	/* Release data input */
	io_data_input_release(k);
	
	printf("\nTEST DONE\n\n");

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);

	io_signal_off();
}
#endif /* KTEST */

/******************************************************************************
 * Software test routines for `kio' (ICSP I/O)
 *****************************************************************************/

/*
 * Start or stop firmware (Raspberry Pi only)
 */
#ifdef KIO
void
io_test_run(struct k8048 *k, int run)
{
	if (run) {
		io_set_vpp_pgm(k, HIGH, LOW);
	} else {
		io_set_vpp_pgm(k, LOW, LOW);
	}
}

/*
 * Send an 8-bit byte
 */
int
io_test_out(struct k8048 *k, int t1, int t2, unsigned char byte)
{
	int i, bit;
	int parity = 0; /* 0=even 1=odd */

	if (k->debug >= 10)
		fprintf(stderr, "%s(k, %d, %d, 0x%02X)\n",
			__func__, t1, t2, byte);

	/* Start bit */
	io_clock_out(k, t1, LOW);

	/* Send 8-bit byte */
	for (i = 0; !io_stop && i < 8; ++i) {
		bit = (byte >> i) & 1;
		parity ^= bit;
		io_clock_out(k, t1, bit);
	}

	/* Send parity bit */	
	io_clock_out(k, t1, parity);

	/* Send stop bit */
	io_clock_out(k, t1, HIGH);

	/* Firmware delay */
	io_usleep(k, t2);
	
	return ERRNONE;
}

/*
 * Get an 8-bit byte
 */
int
io_test_in(struct k8048 *k, int t1, int t2, unsigned char *byte)
{
	int i, bit;
	int parity = 0; /* 0=even 1=odd */
	
	*byte = '\0';

	if (k->debug >= 10)
		fprintf(stderr, "%s(k, %d, %d, 0x%02X)\n",
			__func__, t1, t2, *byte);

	/* Prepare data input */
	io_data_input_acquire(k);

	/* Get start bit */
	if (io_clock_in(k, t1) != LOW) {
		if (k->debug >= 10)
			fprintf(stderr, "%s: INVALID START BIT\n", __func__);
		return ERRPROTOCOL;
	}

	/* Get 8-bit byte */
	for (i = 0; !io_stop && i < 8; i++) {
		bit = io_clock_in(k, t1);
		parity ^= bit;
		*byte |= (bit << i);
	}

	/* Get parity bit */	
	bit = io_clock_in(k, t1);
	if (bit != parity) {
		if (k->debug >= 10)
			fprintf(stderr, "%s: INVALID PARITY BIT\n", __func__);
		return ERRPARITY;
	}

	/* Get stop bit */
	if (io_clock_in(k, t1) != HIGH) {
		if (k->debug >= 10)
			fprintf(stderr, "%s: INVALID STOP BIT\n", __func__);
		return ERRPROTOCOL;
	}

	/* Firmware processing delay */
	io_usleep(k, t2);

	/* Release data input */
	io_data_input_release(k);

	return ERRNONE;
}

/*
 * Send command and arg(s), get result
 */
int
io_test_command(struct k8048 *k, int t1, int t2, unsigned char *cmdarg, int cmdargc, unsigned int *res, int resw)
{
	int i, err;
	unsigned char byte;

	if (k->debug >= 10)
		fprintf(stderr, "%s(k, t1=%d, t2=%d, cmd=%02X, %d, %p, %d)\n", __func__,
			t1, t2, cmdarg[0], cmdargc, res, resw);

	if (cmdargc < 0) {
		printf("%s: fatal error: invalid length: %d\n", __func__, cmdargc);
		exit(EX_SOFTWARE); /* Panic */
	}
	
	if (resw < 0 || resw > 4) {
		printf("%s: fatal error: invalid width: %d\n", __func__, resw);
		exit(EX_SOFTWARE); /* Panic */
	}

	if (resw > 0 && res == NULL) {
		printf("%s: fatal error: invalid result pointer: %p\n", __func__, res);
		exit(EX_SOFTWARE); /* Panic */
	}

	/* Send command */
	io_test_out(k, t1, t2, cmdarg[0]);

	/* Get command ACK */
	err = io_test_in(k, t1, t2, &byte);
	if (err != ERRNONE) {
		return err;
	}
	if (byte != ACK) {
		return ERRNOTSUP;
	}

	/* Send arg(s), get data byte(s) */
	if (cmdargc > 0 || resw > 0)
	{
		unsigned char checksum = cmdarg[0] + byte;

		/* Send arg(s) */
		if (cmdargc > 0) {
			for (i = 1; !io_stop && i <= cmdargc; ++i) {
				io_test_out(k, t1, t2, cmdarg[i]);
				checksum += cmdarg[i];
			}
		}

		/* Get data byte(s) */
		if (resw > 0) {
			*res = 0;
			for (i = 0; !io_stop && i < resw; ++i) {
				err = io_test_in(k, t1, t2, &byte);
				if (err != ERRNONE) {
					return err;
				}
				*res = (*res << 8) | byte;
				checksum += byte;
			}
		}

		/* Get check sum */
		err = io_test_in(k, t1, t2, &byte);
		if (err != ERRNONE) {
			return err;
		}
		if (byte != checksum) {
			if (k->debug >= 10)
				fprintf(stderr, "%s: INVALID CHECKSUM 0x%02X != 0x%02X\n",
					__func__, byte, checksum);
			return ERRINVALID;
		}
	}

	return ERRNONE; /* OK */
}

/*
 * Get error string
 */
char *
io_test_err(int err)
{
	static char *errmsg;

	switch (err) {
	case ERRNONE:
		errmsg = "ERRNONE";
		break;
	case ERRTIMEOUT:
		errmsg = "ERRTIMEOUT";
		break;
	case ERRPROTOCOL:
		errmsg = "ERRPROTOCOL";
		break;
	case ERRPARITY:
		errmsg = "ERRPARITY";
		break;
	case ERRNOTSUP:
		errmsg = "ERRNOTSUP";
		break;
	case ERRINVALID:
		errmsg = "ERRINVALID";
		break;
	default:errmsg = "ERRUNKNOWN";
		break;
	}

	return errmsg;
}
#endif /* KIO */

/******************************************************************************
 * Software test routines for `ktest'
 *****************************************************************************/

#if defined(KTEST) && defined(KIO)
/*
 * Read and output switches.
 */
void
io_test_switch(struct k8048 *k, int t)
{
	int err;
	unsigned char cmd[1];

	unsigned int sw;
	static unsigned int lastsw = 0xFF;

	cmd[0] = CMD_SWITCH;
	err = io_test_command(k, t, t << 1, cmd, 0, &sw, 1);
	if (err != ERRNONE) {
		if (k->debug >= 1) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, io_test_err(err), err);
		}
		io_usleep(k, RESYNCTIME);
	} else if (!io_stop && sw != lastsw) {
		printf("%s: SW=0x%02X SW1=%d SW2=%d SW3=%d SW4=%d\n",
			__func__, sw,
			(sw & 0x01) >> 0, (sw & 0x02) >> 1,
			(sw & 0x04) >> 2, (sw & 0x08) >> 3);
		lastsw = sw;
	}
}

/*
 * Read and output last error.
 */
void
io_test_lasterror(struct k8048 *k, int t)
{
	int err;
	unsigned char cmd[1];

	unsigned int le;
	static unsigned int lastle = 0;

	cmd[0] = CMD_ERROR;
	err = io_test_command(k, t, t << 1, cmd, 0, &le, 1);
	if (err != ERRNONE) {
		if (k->debug >= 1) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, io_test_err(err), err);
		}
		io_usleep(k, RESYNCTIME);
	} else if (!io_stop && le != lastle) {
		printf("%s: last error: %s [0x%02X]\n",
			__func__, io_test_err(le), le);
		lastle = le;
	}
}

/*
 * Test LEDs and Switches
 */
void
io_test5(struct k8048 *k, int t)
{
	int err;
	unsigned char cmd[2];
	unsigned int ld = 0;

	printf("\nTEST MODE 5 [ICSPIO] CTRL-C TO STOP\n\n");

	io_signal_on();

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);

	while (!io_stop) {
		cmd[0] = CMD_LED;
		cmd[1] = ld++;
		err = io_test_command(k, t, t << 1, cmd, 1, NULL, 0);
		if (err != ERRNONE) {
			if (k->debug >= 1) {
				fprintf(stderr, "%s: error: %s [0x%02X]\n",
					__func__, io_test_err(err), err);
			}
			io_usleep(k, RESYNCTIME);
		}
		io_test_switch(k, t);
		io_test_lasterror(k, t);
	}

	printf("\nTEST DONE\n\n");

	io_set_vpp_pgm(k, LOW, LOW);
	io_set_pgd_pgc(k, LOW, LOW);
	
	io_signal_off();
}
#endif /* KTEST && KIO */
