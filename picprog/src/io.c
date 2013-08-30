/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "k8048.h"
#include "lpicp_icsp.h"

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

void
io_usleep(struct k8048 *k, int n)
{
        if (k->fd != -1) {
                /* tty */
                if (n <= 0) {
                        /* 1us ttyS, 3ms ttyUSB */
                        return;
                }
        } else {
                /* rpi */
                if (n <= 0)
                        n = 1;
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
        }
        usleep(n);
}


/*
 * I/O busy cursor
 */
void
io_cursor(struct k8048 *k, char c)
{
	static int c_count = 0;
	static const char arrow[] = "|/-\\";
	int len = strlen(arrow);

	if (++c_count % (k->busy * len - 1) == 0) {
		fprintf(stderr, "%c[%c]\r", arrow[len - (c_count % len) - 1], c);
	}
}

/*
 * Put board in standby
 */
void
io_standby(struct k8048 *k)
{
        /* io_set_pgd_pgc(k, LOW, LOW); */
        /* io_set_mclr(k, LOW); */

        io_cursor(k, '-');
}

/*
 * Initialise board and device for Program/Verify Mode
 */
void
io_init_program_verify(struct k8048 *k, unsigned long key)
{
	io_cursor(k, '+');

	io_standby(k);
	io_usleep(k, 100000);	/* 100ms */

	/* io_set_mclr(k, HIGH); */
	io_usleep(k, 100000);	/* 100ms */
	if (key != NOKEY) {
		lpp_icsp_mclr_set(k->lpp_context, 0);
		io_usleep(k, 2);
		lpp_icsp_data_only_32(k->lpp_context,key);
		io_usleep(k, 2);
		lpp_icsp_mclr_set(k->lpp_context, 0);
		/* io_word_out32(k, key); */
		/* io_clock_out(k, k->sleep, LOW); 33 CLOCKS */
	}
}

