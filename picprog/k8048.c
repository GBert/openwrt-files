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
/*
 * k8048 help
 */
void
usage_k8048(struct k8048 *k)
{
	printf("USAGE: k14 | k16\n\n");
	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n"
		" k14 [SELECT DEVICE] OPERATION [ARG]\n"
		"\t\t14-bit word PIC10F/12F/16F operations.\n"
		" k16 OPERATION [ARG]\n"
		"\t\t16-bit word PIC18F operations.\n");

	exit(EX_OK);
}

/*
 * k14 help
 */
void
usage_k14(struct k8048 *k, char *msg)
{
	printf("USAGE: k14 [SELECT DEVICE] OPERATION [ARG]\n");
	printf("14-bit word PIC10F/12F/16F operations.\n\n");
	if (msg)
		printf("Error: %s.\n\n", msg);
	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n");
	printf(" k14 select\n"
		"\t\tDump supported devices.\n");
	printf(" k14 select PIC16F84 OPERATION [ARG]\n"
		"\t\tSelect device PIC16F84.\n");
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
		"\t\tVerify file.hex in flash (intel hex32 format).\n");

	if (msg)
		exit(EX_USAGE);
	exit(EX_OK);
}

/*
 * k16 help
 */
void
usage_k16(struct k8048 *k, char *msg)
{
	printf("USAGE: k16 [SELECT] OPERATION [ARG]\n");
	printf("16-bit word PIC18F operations.\n\n");
	if (msg)
		printf("Error: %s.\n\n", msg);
	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration.\n\n", k->dotfile);

	printf("EXAMPLES:\n");
	printf(" k16 select\n"
		"\t\tDump supported devices.\n");
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
		"\t\tVerify file.hex in flash (intel hex32 format).\n");
	
	if (msg)
		exit(EX_USAGE);
	exit(EX_OK);
}

/*
 * k14/k16 help
 */
void
usage(struct k8048 *k, char *execname, char *msg)
{
	if (strcmp("k14", execname) == 0) {
		usage_k14(k, msg);
	} else if (strcmp("k16", execname) == 0) {
		usage_k16(k, msg);
	} else {
		usage_k8048(k);
	}
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
	/* TODO */
	/* getconf(&k, execname); */
	
	/* Command: k8048 */
	if (strcmp(execname, "k8048") == 0)
		usage_k8048(&k);

	/* Open device */
	if (lpp_icsp_init(k.lpp_context, "/dev/icsp0") < 0) {
		char *msg;

		if (strstr(k.device, "/dev/") == k.device)
			msg = "Can't open serial I/O";
		else if (mystrcasestr(k.device, "rpi") == k.device)
			msg = "Can't open RPI I/O";
		else
			msg = "Invalid I/O device";
		/* TODO */
		/* if (strcmp(execname, "ktest") == 0)
			usage_ktest(&k, msg);
		else */
			usage(&k, execname, msg);
	}

	/* Reset uid */
	if (getuid() != geteuid()) {
		if (setuid(getuid() < 0)) {
			printf("%s: fatal error: setuid failed\n", __func__);
			exit(EX_OSERR); /* Panic */
		}
	}
	
	/* Command: k14 | k16 */
	if (strcmp(execname, "k14") == 0)
		k.arch = ARCH14BIT;	/* 14-bit word PIC10F/PIC12F/PIC16F */
	else if (strcmp(execname, "k16") == 0)
		k.arch = ARCH16BIT;	/* 16-bit word PIC18F */
	else
		usage_k8048(&k);

	if (argc < 2)
		usage(&k, execname, "Missing arg(s)");

	if (argv[1][0] == 's') {	/* Select */
		if (argc < 3) {
			pic_selector(&k);
			lpp_icsp_destroy(k.lpp_context);
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
	}

	int words = 0;
	char *filename;
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

	lpp_icsp_destroy(k.lpp_context);
	exit(EX_OK);
}
