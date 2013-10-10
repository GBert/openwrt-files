/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#include "VERSION"

#include "k8048.h"

/*
 * k8048 help
 */
void
usage_k8048(struct k8048 *k)
{
	printf("USAGE: k8048\n");
	printf("Usage information.\n\n");

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("BACKENDS:\n"
#ifdef TTY
		" TTY\n"
		"\t\tPOSIX serial.\n"
#endif
#ifdef RPI
		" RPI\n"
		"\t\tRaspberry Pi GPIO.\n"
#endif
#ifdef MCP23017
		" MCP23017\n"
		"\t\tLinux MCP23017 I2C.\n"
#endif
#ifdef LPICP
		" LPICP\n"
		"\t\tLinux PIC Programmer ICSP.\n"
#endif
		"\n");

	printf("EXAMPLES:\n"
#ifdef K12
		" k12 SELECT DEVICE OPERATION [ARG]\n"
		"\t\t12-bit word PIC10F/12F/16F operations.\n"
#endif
#ifdef K14
		" k14 [MCHP | SELECT 16F84] OPERATION [ARG]\n"
		"\t\t14-bit word PIC10F/12F/16F operations.\n"
#endif
#ifdef K16
		" k16 [MCHP] OPERATION [ARG]\n"
		"\t\t16-bit word PIC18F operations.\n"
#endif
#ifdef KTEST
		" ktest TEST [ARG]\n"
		"\t\tHardware tests.\n"
#endif
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	exit(EX_OK);
}

/*
 * ktest help
 */
#ifdef KTEST
void
usage_ktest(struct k8048 *k, char *msg)
{
	printf("USAGE: ktest TEST ARG\n");
	printf("Hardware tests.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n"
		" ktest VPP|PGC|PGD|PGM 5\n"
		"\t\tVPP, PGC, PGD or PGM LOW->HIGH->LOW test with 5 seconds high time.\n"
		" ktest 0 10\n"
		"\t\tR-PI GPIO test with 10 seconds mark time.\n"
		" ktest 1 10\n"
		"\t\tD-SUB-9 test with 10 seconds per step.\n"
		" ktest 2 10\n"
		"\t\tICSP test with 10 seconds per step.\n"
		" ktest 3 0\n"
		"\t\tD-SUB-9 RTS 7 (PGC) DTR 4 (PGD) test with no mark time.\n"
		" ktest 3 1\n"
		"\t\tD-SUB-9 RTS 7 (PGC) DTR 4 (PGD) test with SLEEP mark time.\n"
		" ktest 3 100\n"
		"\t\tD-SUB-9 RTS 7 (PGC) DTR 4 (PGD) test with 100 microseconds mark time.\n"
		" ktest 4 100\n"
		"\t\t16F627 debug test with 100 microseconds clock mark time.\n"
#if defined(KTEST) && defined(KIO)
		" ktest 5 100\n"
		"\t\tICSPIO demo test with 100 microseconds clock mark time.\n"
#endif
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		exit(EX_USAGE);
	exit(EX_OK);
}
#endif

/*
 * k12 help
 */
#ifdef K12
void
usage_k12(struct k8048 *k, char *msg)
{
	printf("USAGE: k12 SELECT DEVICE OPERATION [ARG]\n");
	printf("12-bit word PIC10F/12F/16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n");
	printf(" k12 select\n"
		"\t\tDump supported devices.\n");
	printf(" k12 select PIC1XFXXX blank\n"
		"\t\tBlank device (disable protection and bulk erase).\n");
	printf(" k12 select PIC1XFXXX config\n"
		"\t\tDisplay device configuration.\n");
	printf(" k12 select PIC1XFXXX dump\n"
		"\t\tDump device content (intel hex32 format).\n");
	printf(" k12 select PIC1XFXXX flash [n]\n"
		"\t\tDisplay all or n words of program flash content.\n");
	printf(" k12 select PIC1XFXXX id\n"
		"\t\tDisplay device identification.\n");
	printf(" k12 select PIC1XFXXX osccal\n"
		"\t\tDisplay oscillator calibration.\n");
	printf(" k12 select PIC1XFXXX osccal 0x0c1a\n"
		"\t\tRestore oscillator calibration as 0x0c1a.\n");
	printf(" k12 select PIC1XFXXX program file.hex\n"
		"\t\tProgram file.hex in flash (intel hex32 format).\n");
	printf(" k12 select PIC1XFXXX verify file.hex\n"
		"\t\tVerify file.hex in flash (intel hex32 format).\n"
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		exit(EX_USAGE);
	exit(EX_OK);
}
#endif

/*
 * k14 help
 */
#ifdef K14
void
usage_k14(struct k8048 *k, char *msg)
{
	printf("USAGE: k14 [MCHP | SELECT 16F84] OPERATION [ARG]\n");
	printf("14-bit word PIC10F/12F/16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n");
	printf(" k14 select\n"
		"\t\tDump supported devices.\n");
	printf(" k14 select 16F84 OPERATION [ARG]\n"
		"\t\tSelect device PIC16F84.\n");
	printf(" k14 mchp OPERATION [ARG]\n"
		"\t\tMCHP LVP mode.\n");
	printf(" k14 blank\n"
		"\t\tBlank device (disable protection and bulk erase).\n");
	printf(" k14 config\n"
		"\t\tDisplay device configuration.\n");
	printf(" k14 config 0x3000\n"
		"\t\tRestore band-gap configuration as 0x3000.\n");
	printf(" k14 dump\n"
		"\t\tDump device content (intel hex32 format).\n");
	printf(" k14 eeprom\n"
		"\t\tDisplay data EEPROM content.\n");
	printf(" k14 flash [n]\n"
		"\t\tDisplay all or n words of program flash content.\n");
	printf(" k14 id\n"
		"\t\tDisplay device identification.\n");
	printf(" k14 osccal\n"
		"\t\tDisplay oscillator calibration.\n");
	printf(" k14 osccal 0x343c\n"
		"\t\tRestore oscillator calibration as 0x343c.\n");
	printf(" k14 program file.hex\n"
		"\t\tProgram file.hex in flash (intel hex32 format).\n");
	printf(" k14 verify file.hex\n"
		"\t\tVerify file.hex in flash (intel hex32 format).\n"
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		exit(EX_USAGE);
	exit(EX_OK);
}
#endif

/*
 * k16 help
 */
#ifdef K16
void
usage_k16(struct k8048 *k, char *msg)
{
	printf("USAGE: k16 [MCHP] OPERATION [ARG]\n");
	printf("16-bit word PIC18F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n");
	printf(" k16 select\n"
		"\t\tDump supported devices.\n");
	printf(" k16 mchp OPERATION [ARG]\n"
		"\t\tMCHP LVP mode.\n");
	printf(" k16 blank\n"
		"\t\tBlank device (disable protection and bulk erase).\n");
	printf(" k16 config\n"
		"\t\tDisplay device configuration.\n");
	printf(" k16 dump\n"
		"\t\tDump device content (intel hex32 format).\n");
	printf(" k16 eeprom\n"
		"\t\tDisplay data EEPROM content.\n");
	printf(" k16 flash [n]\n"
		"\t\tDisplay all or n words of program flash content.\n");
	printf(" k16 id\n"
		"\t\tDisplay device identification.\n");
	printf(" k16 program file.hex\n"
		"\t\tProgram file.hex in flash (intel hex32 format).\n");
	printf(" k16 verify file.hex\n"
		"\t\tVerify file.hex in flash (intel hex32 format).\n"
		"\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		exit(EX_USAGE);
	exit(EX_OK);
}
#endif

/*
 * k12/k14/k16 help
 */
void
usage(struct k8048 *k, char *execname, char *msg)
{
#ifdef K12
	if (strcmp("k12", execname) == 0)
		usage_k12(k, msg);
#endif
#ifdef K14
	if (strcmp("k14", execname) == 0)
		usage_k14(k, msg);
#endif
#ifdef K16
	if (strcmp("k16", execname) == 0)
		usage_k16(k, msg);
#endif
	usage_k8048(k);
}

/*
 * Open device and perform command
 */
int
main(int argc, char **argv)
{
	struct k8048 k;
	char *execdup, *execname;

	/* Get exec name */
	execdup = (char *)strdup(argv[0]);
	if (execdup == NULL) {
		printf("%s: fatal error: strdup failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}
	execname = basename(execdup);
	if (execname == NULL) {
		printf("%s: fatal error: basename failed\n", __func__);
		exit(EX_OSERR); /* Panic */
	}

	/* Get configuration */
	getconf(&k, execname);
	
	/* Command: k8048 */
	if (strcmp(execname, "k8048") == 0)
		usage_k8048(&k);

	/* Open device */
	if (io_open(&k, 0) < 0) {
#ifdef KTEST
		if (strcmp(execname, "ktest") == 0)
			usage_ktest(&k, io_error(&k));
#endif
		usage(&k, execname, io_error(&k));
	}

	/* Reset uid */
	if (getuid() != geteuid()) {
		if (setuid(getuid()) < 0) {
			printf("%s: fatal error: setuid failed\n", __func__);
			exit(EX_OSERR); /* Panic */
		}
	}

#ifdef KTEST
	/* Command: ktest */
	if (strcmp(execname, "ktest") == 0) {
		if (k.iot == IOLPICP) {
			printf("%s: fatal error: lpicp icsp unsupported\n", __func__);
			exit(EX_SOFTWARE); /* Panic */
		}

		if (argc < 3)
			usage_ktest(&k, "Missing args");
		else if (argc > 3)
			usage_ktest(&k, "Too many args");
		int testarg = strtol(argv[2], NULL, 0);
		if (testarg < 0)
			usage_ktest(&k, "Invalid arg");

		io_init(&k);
		if (strcasecmp(argv[1], "VPP") == 0) 
			io_test0(&k, 0, testarg);
		else if (strcasecmp(argv[1], "PGC") == 0)
			io_test0(&k, 1, testarg);
		else if (strcasecmp(argv[1], "PGD") == 0)
			io_test0(&k, 2, testarg);
		else if (strcasecmp(argv[1], "PGM") == 0)
			io_test0(&k, 3, testarg);
		else if (argv[1][0] >= '0' && argv[1][0] <= '9') {
			int test = strtol(argv[1], NULL, 0);
			switch (test) {
#ifdef RPI
			case 0: if (k.gpio.fd < 0)
					usage_ktest(&k, "Invalid arg");
				gpio_test(&k.gpio, testarg);
				break;
#endif
			case 1: io_test1(&k, testarg); break;
			case 2: io_test2(&k, testarg); break;
			case 3: io_test3(&k, testarg); break;
			case 4: io_test4(&k, testarg); break;
#if defined(KTEST) && defined(KIO)
			case 5: io_test5(&k, testarg); break;
#endif
			default:usage_ktest(&k, "Invalid arg");break;
			}
		} else {
			usage_ktest(&k, "Invalid arg");
		}
		io_close(&k, 1);
		exit(EX_OK);
	}
#endif

	/* Command: k12 | k14 | k16 */
	if (strcmp(execname, "k12") == 0)
		k.arch = ARCH12BIT;	/* 12-bit word PIC10F/PIC12F/PIC16F */
	else if (strcmp(execname, "k14") == 0)
		k.arch = ARCH14BIT;	/* 14-bit word PIC10F/PIC12F/PIC16F */
	else if (strcmp(execname, "k16") == 0)
		k.arch = ARCH16BIT;	/* 16-bit word PIC18F */
	else
		usage_k8048(&k);
	if (argc < 2)
		usage(&k, execname, "Missing arg(s)");

	if (argv[1][0] == 's') {	/* Select device */
		if (argc < 3) {
			pic_selector(&k);
			io_close(&k, 1);
			exit(EX_OK);
		}
		if (mystrcasestr(argv[2], "pic") == argv[2]) {
			strncpy(k.devicename, argv[2], STRLEN);
		} else {
			int temp = strtol(argv[2], NULL, 0);
			if (temp < 10 || temp > 18) {
				usage(&k, execname, "Invalid arg [select]");
			}
			strcpy(k.devicename, "pic");
			strncpy(&k.devicename[3], argv[2], STRLEN - 3);
		}
		argc -= 2;
		argv += 2;
		if (argc < 2)
			usage(&k, execname, "Missing arg(s)");
	} else if (k.arch == ARCH12BIT) {
		usage(&k, execname, "Missing select");
	}

	if (argv[1][0] == 'm') {	/* MCHP LVP mode */
		if (k.arch == ARCH14BIT) {
			k.key = MCHPKEY;
		} else if (k.arch == ARCH16BIT) {
			k.key = PHCMKEY;
		} else {
			usage(&k, execname, "Invalid arg [mchp]");
		}
		argc -= 1;
		argv += 1;
		if (argc < 2)
			usage(&k, execname, "Missing arg(s)");
	} else {
		k.key = NOKEY;
	}

	int words = 0;
	char *filename;
	io_init(&k);
	switch (argv[1][0]) {
	case 'b':	if (argc > 2)
				usage(&k, execname, "Too many args [blank]");
			if (areyousure("Blank device"))
				pic_blank(&k);
			break;

	case 'c':	if (argc > 3)
				usage(&k, execname, "Too many args [config]");
			if (argc == 2)
				pic_dumpconfig(&k);
			else
				pic_writebandgap(&k, strtoul(argv[2], NULL, 0));
			break;
	
	case 'd':	if (argc > 2)
				usage(&k, execname, "Too many args [dump]");
			pic_dumpdevice(&k);
			break;

	case 'e':	if (argc > 2)
				usage(&k, execname, "Too many args [eeprom]");
			pic_dumpeeprom(&k);
			break;

	case 'f':	if (argc > 3)
				usage(&k, execname, "Too many args [flash]");
			if (argc == 3) {
				words = strtol(argv[2], NULL, 0);
				if (words <= 0)
					usage(&k, execname, "Invalid arg [flash]");
			}
			pic_dumpflash(&k, words);
			break;

	case 'i':	if (argc > 2)
				usage(&k, execname, "Too many args [id]");
			pic_dumpdeviceid(&k);
			break;

	case 'o':	if (argc > 3)
				usage(&k, execname, "Too many args [osccal]");
			if (argc == 2)
				pic_dumposccal(&k);
			else
				pic_writeosccal(&k, strtoul(argv[2], NULL, 0));
			break;

	case 'p':	if (argc < 3)
				usage(&k, execname, "Missing arg [program]");
			else if (argc > 3)
				usage(&k, execname, "Too many args [program]");

			filename = getarg(argv[2]);
			if (filename != NULL) {
				pic_program_verify(&k, filename, PROGRAM);
				free(filename);
			} else
				usage(&k, execname, "Missing filename [program]");
			break;

	case 'v':	if (argc < 3)
				usage(&k, execname, "Missing arg [verify]");
			else if (argc > 3)
				usage(&k, execname, "Too many args [verify]");

			filename = getarg(argv[2]);
			if (filename != NULL) {
				pic_program_verify(&k, filename, VERIFY);
				free(filename);
			} else
				usage(&k, execname, "Missing filename [verify]");
			break;

	default:	usage(&k, execname, "Unknown operation");
			break;
	}
	io_close(&k, 1);
	exit(EX_OK);
}
